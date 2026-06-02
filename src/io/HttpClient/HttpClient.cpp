/**
 * @file HttpClient.cpp
 * 
 * @brief This file implements the HttpClient class, which represents an HTTP client.
 *
 * @author Adrien GRAS
 * @date 2026-06-02
 */


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

    /**
     * @brief Callback function for writing the response body.
     * @param ptr Pointer to the data buffer.
     * @param size Size of each element in the buffer.
     * @param nmemb Number of elements in the buffer.
     * @param userdata Pointer to user data.
     * @return The number of bytes written.
     */
    static size_t writeBodyCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {

        // Append the received data to the response body string
        std::string* body = static_cast<std::string*>(userdata);
        body->append(ptr, size * nmemb);


        // Return the number of bytes processed (must be size * nmemb or libcurl will consider it an error)
        return size * nmemb;
    }


    /**
     * @brief Callback function for writing response headers.
     * @param buffer Pointer to the header buffer.
     * @param size Size of each element in the buffer.
     * @param nitems Number of elements in the buffer.
     * @param userdata Pointer to user data.
     * @return The number of bytes written.
     */
    static size_t writeHeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {

        // Append the received header line to the response headers map
        auto* headers = static_cast<std::map<std::string, std::string>*>(userdata);


        // Each header line is in the format "Header-Name: Header Value\r\n"
        std::string line(buffer, size * nitems);
        auto colon = line.find(':');


        // Only process lines that contain a colon (valid header lines)
        if (colon != std::string::npos) {

            // Split the line into name and value, and trim whitespace
            std::string name  = line.substr(0, colon);
            std::string value = line.substr(colon + 2);


            // Strip trailing \r\n
            while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) {

                // Remove the last character if it's \r or \n
                value.pop_back();

            }

            // Store the header in the map (header names are case-insensitive, but we keep the original case)
            (*headers)[name] = value;
        }


        // Return the number of bytes processed (must be size * nitems or libcurl will consider it an error)
        return size * nitems;
    }





    // =========================================================================
    // ProxyConfig
    // =========================================================================

    /**
     * @brief Converts the proxy configuration to a URL string.
     * @details If username and password are provided, the URL will include them in the format
     * "http://username:password@host:port".
     * @return The proxy URL
     */
    std::string ProxyConfig::toUrl() const {

        // Build the proxy URL string
        std::ostringstream oss;
        oss << "http://";


        // If username and password are provided, include them in the URL
        if (!_username.empty()) {

            // Append username and password in the format "username:password@"
            oss << _username << ':' << _password << '@';

        }

        
        // Append the host and port
        oss << _host << ':' << _port;
        return oss.str();
    }


    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Constructs a new HttpClient instance.
     */
    HttpClient::HttpClient() {

        // Initialize members
        _curl       = nullptr;
        _rng        = std::random_device{}();
        _userAgents = defaultUserAgents();


        // Initialize libcurl globally (only needed once per application) and create an easy handle
        // for this instance
        curl_global_init(CURL_GLOBAL_ALL);
        _curl = curl_easy_init();


        // Check if initialization succeeded
        if (!_curl) {
            throw std::runtime_error("HttpClient: failed to initialize libcurl handle");
        }

    }


    /**
     * @brief Destructs the HttpClient instance.
     */
    HttpClient::~HttpClient() {

        // Clean up the libcurl easy handle and global resources
        if (_curl) { curl_easy_cleanup(_curl); }
        curl_global_cleanup();
    }





    // =========================================================================
    // Configuration
    // =========================================================================

    /**
     * @brief Enables or disables cookie handling.
     * @param enable Whether to enable cookies.
     */
    void HttpClient::enableCookies(bool enable) {

        // Set the flag
        _cookiesEnabled = enable;


        // Empty string activates in-memory cookie engine (no file on disk)
        if (enable) {
            curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, "");
        }

    }


    /**
     * @brief Clears all cookies.
     */
    void HttpClient::clearCookies() {
        curl_easy_setopt(_curl, CURLOPT_COOKIELIST, "ALL");
    }





    // =========================================================================
    // User-Agent management
    // =========================================================================

    /**
     * @brief Sets the list of user agents.
     * @param agents The list of user agents.
     */
    void HttpClient::setUserAgents(const std::vector<std::string>& agents) {

        // Set the user agents if the provided list is not empty (otherwise keep the existing list)
        if (!agents.empty()) {
            _userAgents = agents;
        }

    }


    /**
     * @brief Sets the user agent.
     * @param agent The user agent.
     */
    void HttpClient::setUserAgent(const std::string& agent) {

        // Set a single user agent (disables rotation)
        _userAgents             = { agent };
        _policy.rotateUserAgent = false;

    }





    // =========================================================================
    // Requests
    // =========================================================================

    /**
     * @brief Sends a GET request to the specified URL.
     * @param url The URL to send the request to.
     * @return The response from the server.
     */
    HttpResponse HttpClient::get(const std::string& url) {
        return executeWithRetry(url, "GET", "", "");
    }


    /**
     * @brief Sends a POST request to the specified URL.
     * @param url The URL to send the request to.
     * @param formData The form data to send.
     * @return The response from the server.
     */
    HttpResponse HttpClient::post(const std::string& url,
                                  const std::map<std::string, std::string>& formData) {

        // Build the form-encoded body string (e.g. "key1=value1&key2=value2")
        std::string body;
        for (auto it = formData.begin(); it != formData.end(); ++it) {

            // Append an '&' before each key-value pair except the first one
            if (it != formData.begin()) {
                body += '&';
            }
            body += urlEncode(it->first) + '=' + urlEncode(it->second);

        }


        // Send the POST request with the form-encoded body
        return executeWithRetry(url, "POST", body, "application/x-www-form-urlencoded");

    }


    /**
     * @brief Sends a POST request with JSON data to the specified URL.
     * @param url The URL to send the request to.
     * @param jsonBody The JSON data to send.
     * @return The response from the server.
     */
    HttpResponse HttpClient::postJson(const std::string& url, const std::string& jsonBody) {
        return executeWithRetry(url, "POST", jsonBody, "application/json");
    }





    // =========================================================================
    // Internal execution
    // =========================================================================

    /**
     * @brief Executes a request with retry logic.
     * @param url The URL to send the request to.
     * @param method The HTTP method.
     * @param body The request body.
     * @param contentType The content type of the request.
     * @return The response from the server.
     */
    HttpResponse HttpClient::executeWithRetry(const std::string& url,
                                              const std::string& method,
                                              const std::string& body,
                                              const std::string& contentType) {

        // Apply rate limiting before the first attempt (and before each retry)
        applyRateLimit();


        // Execute the request and retry on failure according to the policy
        HttpResponse response;
        int attempt = 0;
        while (attempt <= _policy.maxRetries) {

            // If this is a retry attempt, wait for the computed delay before retrying
            if (attempt > 0) {
                auto delay = computeRetryDelay(attempt);
                std::cerr << "[HttpClient] Retry " << attempt
                          << "/" << _policy.maxRetries
                          << " in " << delay.count() << "ms"
                          << " (status=" << response.statusCode << ")\n";
                std::this_thread::sleep_for(delay);
            }

            // Execute the request once and get the response
            response = executeOnce(url, method, body, contentType);
            ++_totalRequests;


            // If the request was successful or if it should not be retried based on the status code, break the loop
            if (response.success || !shouldRetry(response.statusCode)) {
                break;
            }


            // Increment
            ++attempt;

        }


        // Return the final response
        return response;

    }


    /**
     * @brief Executes a single request.
     * @param url The URL to send the request to.
     * @param method The HTTP method.
     * @param body The request body.
     * @param contentType The content type of the request.
     * @return The response from the server.
     */
    HttpResponse HttpClient::executeOnce(const std::string& url,
                                         const std::string& method,
                                         const std::string& body,
                                         const std::string& contentType) {

        // Response object to fill and return
        HttpResponse response;


        // Reset the libcurl easy handle to clear previous request data and set new options for this request
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
        if (_cookiesEnabled) {
            curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, "");
        }



        // ---- User-Agent ----
        const std::string& ua = pickUserAgent();
        curl_easy_setopt(_curl, CURLOPT_USERAGENT, ua.c_str());



        // ---- Proxy ----
        const ProxyConfig* proxy = pickProxy();


        // If a proxy is selected, set the proxy options on the libcurl handle
        if (proxy) {

            // Build the proxy URL (including credentials if provided) and set the CURLOPT_PROXY option
            std::string proxyUrl = proxy->toUrl();
            curl_easy_setopt(_curl, CURLOPT_PROXY, proxyUrl.c_str());


            // If the proxy configuration includes a username, set the proxy authentication option with the credentials
            if (!proxy->_username.empty()) {
                std::string creds = proxy->_username + ':' + proxy->_password;
                curl_easy_setopt(_curl, CURLOPT_PROXYUSERPWD, creds.c_str());
            }
        }



        // ---- Headers ----
        struct curl_slist* headerList = nullptr;


        // Add default Accept and Accept-Language headers if the policy requires it (some sites may block requests that don't include them)
        if (_policy.sendAcceptHeaders) {
            headerList = curl_slist_append(headerList,
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
            headerList = curl_slist_append(headerList,
                "Accept-Language: fr-FR,fr;q=0.9,en;q=0.8");
        }


        // Add a Referer header if the policy requires it (some sites may block requests that don't include it or that don't have a realistic Referer)
        if (_policy.sendReferer) {
            headerList = curl_slist_append(headerList, "Referer: https://www.google.com/");
        }


        // Add any custom headers set by the user
        for (const auto& [name, value] : _customHeaders) {
            std::string header = name + ": " + value;
            headerList = curl_slist_append(headerList, header.c_str());
        }


        // Add the Content-Type header if a content type is specified (e.g. for POST requests)
        if (!contentType.empty()) {
            std::string ct = "Content-Type: " + contentType;
            headerList = curl_slist_append(headerList, ct.c_str());
        }


        // Set the headers on the libcurl handle if any headers were added
        if (headerList) {
            curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headerList);
        }



        // ---- HTTP method ----
        if (method == "POST") {
            curl_easy_setopt(_curl, CURLOPT_POST,          1L);
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS,    body.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(body.size()));
        }



        // ---- Execute ----
        CURLcode res = curl_easy_perform(_curl);
        if (headerList) {
            curl_slist_free_all(headerList);
        }



        // ---- Fill response ----
        if (res != CURLE_OK) {
            response.success    = false;
            response.errorMsg   = curl_easy_strerror(res);
            return response;
        }


        // Final objects
        long    httpCode = 0;
        char*   finalUrl = nullptr;


        // Get the HTTP status code and effective URL after redirects, and fill the response object
        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,  &httpCode);
        curl_easy_getinfo(_curl, CURLINFO_EFFECTIVE_URL,  &finalUrl);


        // Fill the response object with the received data and return it
        response.statusCode     = static_cast<int>(httpCode);
        response.body           = std::move(responseBody);
        response.headers        = std::move(responseHeaders);
        response.effectiveUrl   = finalUrl ? finalUrl : url;
        response.success        = (httpCode >= 200 && httpCode < 300);


        // If the request was not successful, set the error message to include the HTTP status code
        if (!response.success) {
            response.errorMsg = "HTTP " + std::to_string(httpCode);
        }


        // HTML Response
        return response;

    }





    // =========================================================================
    // Anti-blocking helpers
    // =========================================================================

    /**
     * @brief Picks a random User-Agent from the pool
     * @return A reference to the selected User-Agent string
     */
    const std::string& HttpClient::pickUserAgent() {

        // If rotation is disabled or if there is only one user agent in the pool, return the first one
        if (!_policy.rotateUserAgent || _userAgents.size() == 1) {
            return _userAgents[0];
        }


        // Otherwise, pick a random user agent from the pool
        std::uniform_int_distribution<size_t> dist(0, _userAgents.size() - 1);
        return _userAgents[dist(_rng)];

    }


    /**
     * @brief Picks a random proxy from the pool
     * @return A pointer to the selected proxy configuration, or nullptr if no proxy is available
     */
    const ProxyConfig* HttpClient::pickProxy() {

        // If rotation is disabled or if the proxy pool is empty, return nullptr (no proxy)
        if (!_policy.rotateProxy || _proxyPool.empty()) {
            return nullptr;
        }


        // Otherwise, pick a random proxy from the pool
        std::uniform_int_distribution<size_t> dist(0, _proxyPool.size() - 1);
        return &_proxyPool[dist(_rng)];

    }


    /**
     * @brief Applies rate limiting by sleeping for a random duration within the specified range
     */
    void HttpClient::applyRateLimit() {

        // If the maximum delay is less than or equal to the minimum delay, just sleep for the minimum delay
        using Ms = std::chrono::milliseconds;
        auto minMs = _policy.minDelayBetweenRequests.count();
        auto maxMs = _policy.maxDelayBetweenRequests.count();
        if (maxMs <= minMs) {
            std::this_thread::sleep_for(Ms(minMs));
            return;
        }


        // Otherwise, sleep for a random duration between the minimum and maximum delay
        std::uniform_int_distribution<long long> dist(minMs, maxMs);
        std::this_thread::sleep_for(Ms(dist(_rng)));

    }


    /**
     * @brief Computes the delay for a retry attempt
     * @param attempt The retry attempt number
     * @return The delay as a std::chrono::milliseconds object
     */
    std::chrono::milliseconds HttpClient::computeRetryDelay(int attempt) const {

        // Exponential backoff: baseDelay * 2^(attempt-1), capped at 30s
        long long ms = _policy.retryBaseDelay.count();
        for (int i = 1; i < attempt; ++i) {
            ms *= 2;
        }


        // Add some random jitter of ±20% to avoid thundering herd problems
        ms = std::min(ms, 30000LL);
        return std::chrono::milliseconds(ms);

    }


    /**
     * @brief Determines whether a request should be retried based on its status code
     * @param statusCode The HTTP status code of the request
     * @return true if the request should be retried, false otherwise
     */
    bool HttpClient::shouldRetry(int statusCode) const {
        if (_policy.retryOn429 && statusCode == 429) return true;
        if (_policy.retryOn503 && statusCode == 503) return true;
        return false;
    }





    // =========================================================================
    // Utilities
    // =========================================================================

    /**
     * @brief URL-encodes a string
     * @param value The string to encode
     * @return The URL-encoded string
     */
    std::string HttpClient::urlEncode(const std::string& value) {

        // Use libcurl's curl_easy_escape function to URL-encode the value, which handles all special characters properly
        CURL* tmp = curl_easy_init();
        if (!tmp) return value;


        // Perform URL encoding (the function returns a newly allocated string that must be freed with curl_free)
        char* encoded = curl_easy_escape(tmp, value.c_str(), static_cast<int>(value.size()));
        std::string result = encoded ? encoded : value;


        // Free the encoded string and clean up the temporary libcurl handle
        curl_free(encoded);
        curl_easy_cleanup(tmp);
        return result;

    }





    // =========================================================================
    // Defaults
    // =========================================================================

    /**
     * @brief Returns the default user agents
     * @return A vector of default user agent strings
     */
    std::vector<std::string> HttpClient::defaultUserAgents() {
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
