// Imports
#include "HttpClient.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <sstream>
#include <thread>
#include <iostream>
#include <algorithm>





/**
 * Namespace CarScraper
 */
namespace CarScraper {


    // =========================================================================
    // libcurl callbacks (file-local)
    // =========================================================================

    static size_t writeBodyCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        std::string* body = static_cast<std::string*>(userdata);
        body->append(ptr, size * nmemb);
        return size * nmemb;
    }

    static size_t writeHeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata)
    {
        auto* headers = static_cast<std::map<std::string, std::string>*>(userdata);

        std::string line(buffer, size * nitems);
        auto colon = line.find(':');

        if (colon != std::string::npos) {
            std::string name  = line.substr(0, colon);
            std::string value = line.substr(colon + 2);

            // Strip trailing \r\n
            while (!value.empty() && (value.back() == '\r' || value.back() == '\n'))
                value.pop_back();

            (*headers)[name] = value;
        }

        return size * nitems;
    }


    // =========================================================================
    // ProxyConfig
    // =========================================================================

    std::string ProxyConfig::toUrl() const
    {
        std::ostringstream oss;
        oss << "http://";

        if (!_username.empty())
            oss << _username << ':' << _password << '@';

        oss << _host << ':' << _port;
        return oss.str();
    }


    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    HttpClient::HttpClient()
        : _curl(nullptr)
        , _rng(std::random_device{}())
        , _userAgents(defaultUserAgents())
    {
        curl_global_init(CURL_GLOBAL_ALL);
        _curl = curl_easy_init();

        if (!_curl)
            throw std::runtime_error("HttpClient: failed to initialize libcurl handle");
    }

    HttpClient::~HttpClient()
    {
        if (_curl) curl_easy_cleanup(_curl);
        curl_global_cleanup();
    }


    // =========================================================================
    // Configuration
    // =========================================================================

    void HttpClient::enableCookies(bool enable)
    {
        _cookiesEnabled = enable;

        // Empty string activates in-memory cookie engine (no file on disk)
        if (enable)
            curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, "");
    }

    void HttpClient::clearCookies()
    {
        curl_easy_setopt(_curl, CURLOPT_COOKIELIST, "ALL");
    }


    // =========================================================================
    // User-Agent management
    // =========================================================================

    void HttpClient::setUserAgents(const std::vector<std::string>& agents)
    {
        if (!agents.empty())
            _userAgents = agents;
    }

    void HttpClient::setUserAgent(const std::string& agent)
    {
        _userAgents             = { agent };
        _policy.rotateUserAgent = false;
    }


    // =========================================================================
    // Requests
    // =========================================================================

    HttpResponse HttpClient::get(const std::string& url)
    {
        return executeWithRetry(url, "GET", "", "");
    }

    HttpResponse HttpClient::post(const std::string& url,
                                  const std::map<std::string, std::string>& formData)
    {
        std::string body;

        for (auto it = formData.begin(); it != formData.end(); ++it) {
            if (it != formData.begin()) body += '&';
            body += urlEncode(it->first) + '=' + urlEncode(it->second);
        }

        return executeWithRetry(url, "POST", body, "application/x-www-form-urlencoded");
    }

    HttpResponse HttpClient::postJson(const std::string& url, const std::string& jsonBody)
    {
        return executeWithRetry(url, "POST", jsonBody, "application/json");
    }


    // =========================================================================
    // Internal execution
    // =========================================================================

    HttpResponse HttpClient::executeWithRetry(const std::string& url,
                                              const std::string& method,
                                              const std::string& body,
                                              const std::string& contentType)
    {
        applyRateLimit();

        HttpResponse response;
        int attempt = 0;

        while (attempt <= _policy.maxRetries) {

            if (attempt > 0) {
                auto delay = computeRetryDelay(attempt);
                std::cerr << "[HttpClient] Retry " << attempt
                          << "/" << _policy.maxRetries
                          << " in " << delay.count() << "ms"
                          << " (status=" << response.statusCode << ")\n";
                std::this_thread::sleep_for(delay);
            }

            response = executeOnce(url, method, body, contentType);
            ++_totalRequests;

            if (response.success || !shouldRetry(response.statusCode))
                break;

            ++attempt;
        }

        return response;
    }

    HttpResponse HttpClient::executeOnce(const std::string& url,
                                         const std::string& method,
                                         const std::string& body,
                                         const std::string& contentType)
    {
        HttpResponse response;

        curl_easy_reset(_curl);

        // ---- Reception buffers ----
        std::string                         responseBody;
        std::map<std::string, std::string>  responseHeaders;

        // ---- Base options ----
        curl_easy_setopt(_curl, CURLOPT_URL,            url.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION,  writeBodyCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA,      &responseBody);
        curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, writeHeaderCallback);
        curl_easy_setopt(_curl, CURLOPT_HEADERDATA,     &responseHeaders);
        curl_easy_setopt(_curl, CURLOPT_TIMEOUT,        _timeoutSeconds);

        // ---- SSL ----
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, _verifySSL ? 1L : 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, _verifySSL ? 2L : 0L);

        // ---- Redirects ----
        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, _followRedirects ? 1L : 0L);
        curl_easy_setopt(_curl, CURLOPT_MAXREDIRS,      static_cast<long>(_maxRedirects));

        // ---- Encoding (gzip, deflate, br) ----
        curl_easy_setopt(_curl, CURLOPT_ACCEPT_ENCODING, "");

        // ---- Cookies ----
        if (_cookiesEnabled)
            curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, "");

        // ---- User-Agent ----
        const std::string& ua = pickUserAgent();
        curl_easy_setopt(_curl, CURLOPT_USERAGENT, ua.c_str());

        // ---- Proxy ----
        const ProxyConfig* proxy = pickProxy();

        if (proxy) {
            std::string proxyUrl = proxy->toUrl();
            curl_easy_setopt(_curl, CURLOPT_PROXY, proxyUrl.c_str());

            if (!proxy->_username.empty()) {
                std::string creds = proxy->_username + ':' + proxy->_password;
                curl_easy_setopt(_curl, CURLOPT_PROXYUSERPWD, creds.c_str());
            }
        }

        // ---- Headers ----
        struct curl_slist* headerList = nullptr;

        if (_policy.sendAcceptHeaders) {
            headerList = curl_slist_append(headerList,
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
            headerList = curl_slist_append(headerList,
                "Accept-Language: fr-FR,fr;q=0.9,en;q=0.8");
        }

        if (_policy.sendReferer)
            headerList = curl_slist_append(headerList, "Referer: https://www.google.com/");

        for (const auto& [name, value] : _customHeaders) {
            std::string header = name + ": " + value;
            headerList = curl_slist_append(headerList, header.c_str());
        }

        if (!contentType.empty()) {
            std::string ct = "Content-Type: " + contentType;
            headerList = curl_slist_append(headerList, ct.c_str());
        }

        if (headerList)
            curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headerList);

        // ---- HTTP method ----
        if (method == "POST") {
            curl_easy_setopt(_curl, CURLOPT_POST,          1L);
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS,    body.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(body.size()));
        }

        // ---- Execute ----
        CURLcode res = curl_easy_perform(_curl);

        if (headerList) curl_slist_free_all(headerList);

        // ---- Fill response ----
        if (res != CURLE_OK) {
            response.success    = false;
            response.errorMsg   = curl_easy_strerror(res);
            return response;
        }

        long    httpCode = 0;
        char*   finalUrl = nullptr;

        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,  &httpCode);
        curl_easy_getinfo(_curl, CURLINFO_EFFECTIVE_URL,  &finalUrl);

        response.statusCode     = static_cast<int>(httpCode);
        response.body           = std::move(responseBody);
        response.headers        = std::move(responseHeaders);
        response.effectiveUrl   = finalUrl ? finalUrl : url;
        response.success        = (httpCode >= 200 && httpCode < 300);

        if (!response.success)
            response.errorMsg = "HTTP " + std::to_string(httpCode);

        return response;
    }


    // =========================================================================
    // Anti-blocking helpers
    // =========================================================================

    const std::string& HttpClient::pickUserAgent()
    {
        if (!_policy.rotateUserAgent || _userAgents.size() == 1)
            return _userAgents[0];

        std::uniform_int_distribution<size_t> dist(0, _userAgents.size() - 1);
        return _userAgents[dist(_rng)];
    }

    const ProxyConfig* HttpClient::pickProxy()
    {
        if (!_policy.rotateProxy || _proxyPool.empty())
            return nullptr;

        std::uniform_int_distribution<size_t> dist(0, _proxyPool.size() - 1);
        return &_proxyPool[dist(_rng)];
    }

    void HttpClient::applyRateLimit()
    {
        using Ms = std::chrono::milliseconds;

        auto minMs = _policy.minDelayBetweenRequests.count();
        auto maxMs = _policy.maxDelayBetweenRequests.count();

        if (maxMs <= minMs) {
            std::this_thread::sleep_for(Ms(minMs));
            return;
        }

        std::uniform_int_distribution<long long> dist(minMs, maxMs);
        std::this_thread::sleep_for(Ms(dist(_rng)));
    }

    std::chrono::milliseconds HttpClient::computeRetryDelay(int attempt) const
    {
        // Exponential backoff: baseDelay * 2^(attempt-1), capped at 30s
        long long ms = _policy.retryBaseDelay.count();

        for (int i = 1; i < attempt; ++i)
            ms *= 2;

        ms = std::min(ms, 30000LL);
        return std::chrono::milliseconds(ms);
    }

    bool HttpClient::shouldRetry(int statusCode) const
    {
        if (_policy.retryOn429 && statusCode == 429) return true;
        if (_policy.retryOn503 && statusCode == 503) return true;
        return false;
    }


    // =========================================================================
    // Utilities
    // =========================================================================

    std::string HttpClient::urlEncode(const std::string& value)
    {
        CURL* tmp = curl_easy_init();
        if (!tmp) return value;

        char* encoded = curl_easy_escape(tmp, value.c_str(), static_cast<int>(value.size()));
        std::string result = encoded ? encoded : value;

        curl_free(encoded);
        curl_easy_cleanup(tmp);
        return result;
    }


    // =========================================================================
    // Defaults
    // =========================================================================

    std::vector<std::string> HttpClient::defaultUserAgents()
    {
        return {
            // Chrome — Windows
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/124.0.0.0 Safari/537.36",

            // Firefox — Windows
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) "
            "Gecko/20100101 Firefox/125.0",

            // Chrome — macOS
            "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/124.0.0.0 Safari/537.36",

            // Safari — macOS
            "Mozilla/5.0 (Macintosh; Intel Mac OS X 14_4_1) "
            "AppleWebKit/605.1.15 (KHTML, like Gecko) "
            "Version/17.4.1 Safari/605.1.15",

            // Edge — Windows
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/124.0.0.0 Safari/537.36 Edg/124.0.0.0",

            // Chrome — Linux
            "Mozilla/5.0 (X11; Linux x86_64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/124.0.0.0 Safari/537.36",

            // Firefox — Linux
            "Mozilla/5.0 (X11; Linux x86_64; rv:125.0) "
            "Gecko/20100101 Firefox/125.0",
        };
    }

} // namespace CarScraper
