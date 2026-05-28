// For only one include
#pragma once


// Imports
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <memory>
#include <random>


// Forward declaration (libcurl) — évite d'inclure curl.h dans le header
typedef void CURL;
struct curl_slist;


/**
 * Namespace CarScraper
 */
namespace CarScraper {


    // =========================================================================
    // Struct HttpResponse
    // =========================================================================

    /**
     * Struct HttpResponse:
     *
     * Represents the result of an HTTP request:
     *      -> statusCode    | int                              | ex: 200
     *      -> body          | string                           | raw HTML content
     *      -> headers       | map<string, string>              | response headers
     *      -> effectiveUrl  | string                           | final URL after redirects
     *      -> success       | bool                             | true if 2xx status
     *      -> errorMsg      | string                           | libcurl or HTTP error
     */
    struct HttpResponse {

        int                                 statusCode  = 0;
        std::string                         body;
        std::map<std::string, std::string>  headers;
        std::string                         effectiveUrl;
        bool                                success     = false;
        std::string                         errorMsg;

    };





    // =========================================================================
    // Struct ProxyConfig
    // =========================================================================

    /**
     * Struct ProxyConfig:
     *
     * Holds the configuration for a single HTTP proxy:
     *      -> host      | string  | ex: "proxy.example.com"
     *      -> port      | int     | ex: 8080
     *      -> username  | string  | optional, ex: "user"
     *      -> password  | string  | optional, ex: "pass"
     */
    struct ProxyConfig {

        std::string _host;
        int         _port       = 0;
        std::string _username;
        std::string _password;

        // Builds "http://user:pass@host:port" (or without auth if empty)
        std::string toUrl() const;

    };





    // =========================================================================
    // Struct AntiBlockPolicy
    // =========================================================================

    /**
     * Struct AntiBlockPolicy:
     *
     * Defines the anti-blocking strategy configured by the Site Scraper:
     * - Rate limiting:
     *      -> minDelayBetweenRequests   | milliseconds  | minimum wait between requests
     *      -> maxDelayBetweenRequests   | milliseconds  | maximum wait (random jitter)
     * - Retry:
     *      -> maxRetries                | int           | number of retry attempts
     *      -> retryOn429                | bool          | retry on Too Many Requests
     *      -> retryOn503                | bool          | retry on Service Unavailable
     *      -> retryBaseDelay            | milliseconds  | base delay, doubles each attempt
     * - Rotation:
     *      -> rotateUserAgent           | bool          | pick a random UA each request
     *      -> rotateProxy               | bool          | pick a random proxy each request
     * - Realistic headers:
     *      -> sendAcceptHeaders         | bool          | send Accept / Accept-Language
     *      -> sendReferer               | bool          | spoof a Google referer
     */
    struct AntiBlockPolicy {

        // Rate limiting
        std::chrono::milliseconds   minDelayBetweenRequests { 500 };
        std::chrono::milliseconds   maxDelayBetweenRequests { 1500 };

        // Retry
        int                         maxRetries      = 3;
        bool                        retryOn429      = true;
        bool                        retryOn503      = true;
        std::chrono::milliseconds   retryBaseDelay  { 2000 };

        // Rotation
        bool                        rotateUserAgent = true;
        bool                        rotateProxy     = false;

        // Realistic headers
        bool                        sendAcceptHeaders   = true;
        bool                        sendReferer         = false;

    };


    


    // =========================================================================
    // Class HttpClient
    // =========================================================================

    /**
     * Class HttpClient:
     *
     * Performs HTTP GET / POST requests and returns raw HTML.
     * Handles anti-blocking techniques internally:
     *      - Random delay between requests (rate limiting + jitter)
     *      - Exponential backoff retry on 429 / 503
     *      - User-Agent rotation
     *      - Proxy rotation
     *      - Realistic browser headers
     *
     * Depends on: libcurl
     * Compile with: -lcurl
     */
    class HttpClient {


        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            // libcurl handle (raw pointer, managed manually in constructor/destructor)
            CURL*                               _curl;

            // Policy (set by the Site Scraper)
            AntiBlockPolicy                     _policy;

            // User-Agents pool
            std::vector<std::string>            _userAgents;

            // Proxy pool
            std::vector<ProxyConfig>            _proxyPool;

            // Custom headers added to every request
            std::map<std::string, std::string>  _customHeaders;

            // Random engine
            std::mt19937                        _rng;

            // Options
            bool                                _cookiesEnabled = false;
            bool                                _followRedirects = true;
            int                                 _maxRedirects   = 10;
            bool                                _verifySSL      = true;
            long                                _timeoutSeconds = 30;

            // Stats
            long                                _totalRequests  = 0;


            // -------------------------------------------------------------------------
            // Anti-blocking helpers
            // -------------------------------------------------------------------------

            // Returns a randomly picked User-Agent from the pool
            const std::string&          pickUserAgent();

            // Returns a randomly picked proxy, or nullptr if pool is empty
            const ProxyConfig*          pickProxy();

            // Sleeps for a random duration between min and max delay
            void                        applyRateLimit();

            // Returns exponential backoff delay for given attempt number
            std::chrono::milliseconds   computeRetryDelay(int attempt) const;

            // Returns true if the status code should trigger a retry
            bool                        shouldRetry(int statusCode) const;


            // -------------------------------------------------------------------------
            // Internal execution
            // -------------------------------------------------------------------------

            // Executes the request with retry logic on top
            HttpResponse    executeWithRetry(
                                const std::string& url,
                                const std::string& method,
                                const std::string& body,
                                const std::string& contentType);

            // Performs a single libcurl request (no retry)
            HttpResponse    executeOnce(
                                const std::string& url,
                                const std::string& method,
                                const std::string& body,
                                const std::string& contentType);


            // -------------------------------------------------------------------------
            // Defaults
            // -------------------------------------------------------------------------

            // Returns a curated list of realistic browser User-Agents
            static std::vector<std::string> defaultUserAgents();


        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------
            HttpClient();
            ~HttpClient();

            // Non-copyable (CURL handle cannot be duplicated)
            HttpClient(const HttpClient&)               = delete;
            HttpClient& operator=(const HttpClient&)    = delete;

            // Movable
            HttpClient(HttpClient&&)                    = default;
            HttpClient& operator=(HttpClient&&)         = default;


            // -------------------------------------------------------------------------
            // Configuration
            // -------------------------------------------------------------------------

            void setPolicy(const AntiBlockPolicy& policy)               { _policy           = policy; }
            void setTimeout(long seconds)                               { _timeoutSeconds   = seconds; }
            void setVerifySSL(bool verify)                              { _verifySSL        = verify; }
            void setFollowRedirects(bool follow, int maxRedirects = 10) { _followRedirects  = follow; _maxRedirects = maxRedirects; }

            void enableCookies(bool enable = true);
            void clearCookies();


            // -------------------------------------------------------------------------
            // User-Agent management
            // -------------------------------------------------------------------------

            // Replaces the default pool with a custom list
            void setUserAgents(const std::vector<std::string>& agents);

            // Forces a single fixed User-Agent (disables rotation)
            void setUserAgent(const std::string& agent);


            // -------------------------------------------------------------------------
            // Proxy management
            // -------------------------------------------------------------------------

            void setProxyPool(const std::vector<ProxyConfig>& proxies)  { _proxyPool = proxies; }
            void addProxy(const ProxyConfig& proxy)                     { _proxyPool.push_back(proxy); }
            void clearProxies()                                         { _proxyPool.clear(); }


            // -------------------------------------------------------------------------
            // Custom headers
            // -------------------------------------------------------------------------

            void setHeader(const std::string& name, const std::string& value)  { _customHeaders[name] = value; }
            void clearHeaders()                                                 { _customHeaders.clear(); }


            // -------------------------------------------------------------------------
            // Requests
            // -------------------------------------------------------------------------

            // GET — returns the raw HTML of the page
            HttpResponse get(const std::string& url);

            // POST with form-encoded body (application/x-www-form-urlencoded)
            HttpResponse post(const std::string& url,
                              const std::map<std::string, std::string>& formData);

            // POST with a raw JSON body (application/json)
            HttpResponse postJson(const std::string& url,
                                  const std::string& jsonBody);


            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            long getTotalRequests() const { return _totalRequests; }


            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            // Percent-encodes a string for use in URLs or form bodies
            static std::string urlEncode(const std::string& value);

    };

} // namespace CarScraper
