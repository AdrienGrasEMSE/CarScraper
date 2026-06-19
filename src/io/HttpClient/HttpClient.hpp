/**
 * @file HttpClient.hpp
 * 
 * @brief This file declares the HttpClient class, which represents an HTTP client.
 *
 * @author Adrien GRAS
 * @date 2026-06-02
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <memory>
#include <random>


// Forward declaration (libcurl) — prevent from including curl.h in the header
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
     * @brief Represents the result of an HTTP request performed by HttpClient.
     */
    struct HttpResponse {

        int                                 statusCode  = 0;        ///< HTTP status code of the response
        std::string                         body;                   ///< Body of the response (e.g. HTML content)
        std::map<std::string, std::string>  headers;                ///< Map of response headers (header name -> header value)
        std::string                         effectiveUrl;           ///< Final URL after following redirects (if any)
        bool                                success     = false;    ///< True if the request was successful (HTTP 2xx), false otherwise
        std::string                         errorMsg;               ///< Error message in case of failure (libcurl error or HTTP error)

    };





    // =========================================================================
    // Struct ProxyConfig
    // =========================================================================

    /**
     * @brief Represents a proxy configuration for HTTP requests, including host, port, and optional authentication.
     */
    struct ProxyConfig {

        // Proxy details
        std::string _host;              ///< Proxy host (IP or domain)   | ex: "proxy.example.com"
        int         _port       = 0;    ///< Proxy port                  | ex: 8080
        std::string _username;          ///< Proxy username (optional)   | ex: "user"
        std::string _password;          ///< Proxy password (optional)   | ex: "pass"


        /**
         * @brief Converts the proxy configuration to a URL string.
         * @details Builds "http://username:password\@host:port" if credentials are provided,
         *          or "http://host:port" otherwise.
         * @return The proxy URL as a string.
         */
        std::string toUrl() const;

    };





    // =========================================================================
    // Struct AntiBlockPolicy
    // =========================================================================

    /**
     * @brief Represents the anti-blocking policy configuration for HttpClient, including rate limiting,
     * retry logic, rotation, and headers.
     */
    struct AntiBlockPolicy {

        // Rate limiting
        std::chrono::milliseconds   minDelayBetweenRequests { 500 };    ///< Minimum delay between requests (in milliseconds)
        std::chrono::milliseconds   maxDelayBetweenRequests { 1500 };   ///< Maximum delay between requests (in milliseconds, for random jitter)

        // Retry
        int     maxRetries          = 3;        ///< Maximum number of retry attempts on failure
        bool    retryOn429          = true;     ///< Whether to retry on HTTP 429 Too Many Requests
        bool    retryOn503          = true;     ///< Whether to retry on HTTP 503 Service Unavailable
        std::chrono::milliseconds   retryBaseDelay          { 2000 };   ///< Base delay for retries (doubled on each attempt — exponential backoff)

        // Rotation
        bool    rotateUserAgent     = true;     ///< Whether to rotate User-Agent header by picking a random one from the pool
        bool    rotateProxy         = false;    ///< Whether to rotate proxies by picking a random one from the proxy pool

        // Realistic headers
        bool    sendAcceptHeaders   = true;     ///< Whether to send realistic Accept and Accept-Language headers
        bool    sendReferer         = false;    ///< Whether to send a spoofed Referer header (e.g. "https://www.google.com/")
        bool    sendSecFetchHeaders = true;     ///< Whether to send Sec-Fetch-* headers (Dest, Mode, Site, Upgrade-Insecure-Requests)

    };


    


    // =========================================================================
    // Class HttpClient
    // =========================================================================

    /**
     * @class HttpClient
     *
     * @brief HTTP client wrapping libcurl with built-in anti-blocking techniques.
     *
     * @details HttpClient inherits from Entity (UUID tracking) and provides GET/POST
     * requests with the following built-in protections:
     * - Random delays between requests to mimic human browsing (rate limiting + jitter)
     * - Automatic retries with exponential backoff on HTTP 429 and 503
     * - User-Agent rotation from a curated pool of realistic browser strings
     * - Proxy rotation from a user-supplied pool
     * - Realistic browser headers (Accept, Accept-Language, optional Referer)
     *
     * @note Depends on libcurl. Link with: -lcurl
     *
     * @see Entity
     * @see AntiBlockPolicy
     * @see HttpResponse
     */
    class HttpClient : public Entity {


        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            // libcurl handle and configuration
            CURL*                               _curl;                  ///< libcurl easy handle for performing requests
            AntiBlockPolicy                     _policy;                ///< Anti-blocking policy configuration
            std::vector<std::string>            _userAgents;            ///< List of User-Agent strings to rotate for requests
            std::vector<ProxyConfig>            _proxyPool;             ///< List of proxies to rotate for requests (if rotation enabled)
            std::map<std::string, std::string>  _customHeaders;         ///< Custom headers to include in every request (header name -> header value)
            std::mt19937                        _rng;                   ///< Random number generator for User-Agent and proxy rotation, and rate limit jitter
            bool                                _cookiesEnabled = false;///< Whether to enable cookie handling
            std::string                         _cookieJarPath;         ///< Path to the cookie jar file (empty = in-memory only)
            bool                                _followRedirects = true;///< Whether to follow HTTP redirects (3xx) automatically
            int                                 _maxRedirects   = 10;   ///< Maximum number of redirects to follow if _followRedirects is true
            bool                                _verifySSL      = true; ///< Whether to verify SSL certificates (set to false only in dev/test)
            long                                _timeoutSeconds = 30;   ///< Timeout for requests in seconds
            long                                _totalRequests  = 0;    ///< Total number of requests made (including retries)





            // -------------------------------------------------------------------------
            // Anti-blocking helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Picks a random User-Agent from the pool.
             * @details Returns the first entry if rotation is disabled or the pool has only one entry.
             * @return A const reference to the selected User-Agent string.
             */
            const std::string& pickUserAgent();


            /**
             * @brief Picks a random proxy from the pool.
             * @details Returns nullptr if rotation is disabled or the pool is empty.
             * @return A pointer to the selected ProxyConfig, or nullptr if none is available.
             */
            const ProxyConfig* pickProxy();


            /**
             * @brief Applies rate limiting by sleeping for a random duration in [minDelay, maxDelay].
             * @details Called automatically before each request.
             */
            void applyRateLimit();


            /**
             * @brief Computes the exponential backoff delay for a given retry attempt.
             * @details Delay = retryBaseDelay × 2^(attempt-1), capped at 30 seconds.
             * @param attempt The current retry attempt number (1-based).
             * @return The computed delay as std::chrono::milliseconds.
             */
            std::chrono::milliseconds computeRetryDelay(int attempt) const;


            /**
             * @brief Determines whether a failed request should be retried.
             * @param statusCode The HTTP status code of the response.
             * @return true if the request should be retried, false otherwise.
             */
            bool shouldRetry(int statusCode) const;





            // -------------------------------------------------------------------------
            // Internal execution
            // -------------------------------------------------------------------------

            /**
             * @brief Executes an HTTP request with retry logic.
             * @details Applies rate limiting before the first attempt, then retries up to
             *          maxRetries times with exponential backoff on retriable status codes.
             * @param url         The target URL.
             * @param method      The HTTP method ("GET" or "POST").
             * @param body        The request body (empty for GET).
             * @param contentType The Content-Type header value (empty for GET).
             * @return The final HttpResponse (from the last attempt).
             */
            HttpResponse executeWithRetry(const std::string& url,
                                          const std::string& method,
                                          const std::string& body,
                                          const std::string& contentType);


            /**
             * @brief Performs a single HTTP request without retry logic.
             * @details Configures the libcurl handle with all current settings (UA, proxy,
             *          headers, SSL, timeouts) and executes the request.
             * @param url         The target URL.
             * @param method      The HTTP method ("GET" or "POST").
             * @param body        The request body (empty for GET).
             * @param contentType The Content-Type header value (empty for GET).
             * @return The HttpResponse for this single attempt.
             */
            HttpResponse executeOnce(const std::string& url,
                                     const std::string& method,
                                     const std::string& body,
                                     const std::string& contentType);





            // -------------------------------------------------------------------------
            // Defaults
            // -------------------------------------------------------------------------

            /**
             * @brief Returns the built-in list of realistic browser User-Agent strings.
             * @details Covers Chrome, Firefox, Safari and Edge across Windows, macOS and Linux.
             * @return A vector of User-Agent strings.
             */
            static std::vector<std::string> defaultUserAgents();





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Constructs a new HttpClient instance.
             * @details Initializes libcurl globally, creates an easy handle, seeds the RNG,
             *          and loads the default User-Agent pool. The Entity prefix is set to "HTTP-CLIENT".
             * @throws std::runtime_error if the libcurl handle cannot be created.
             */
            HttpClient();


            /**
             * @brief Destructs the HttpClient instance.
             * @details Cleans up the libcurl easy handle and releases global libcurl resources.
             */
            ~HttpClient() override;


            /**
             * @brief Copy constructor (deleted — libcurl handles are not copyable).
             */
            HttpClient(const HttpClient&)               = delete;


            /**
             * @brief Copy assignment operator (deleted — libcurl handles are not copyable).
             */
            HttpClient& operator=(const HttpClient&)    = delete;


            /**
             * @brief Move constructor.
             */
            HttpClient(HttpClient&&)                    = default;


            /**
             * @brief Move assignment operator.
             */
            HttpClient& operator=(HttpClient&&)         = default;





            // -------------------------------------------------------------------------
            // Configuration
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the anti-block policy.
             * @param policy The AntiBlockPolicy to apply for subsequent requests.
             */
            void setPolicy(const AntiBlockPolicy& policy)               { _policy           = policy; }


            /**
             * @brief Sets the request timeout.
             * @param seconds Timeout in seconds (0 = no timeout).
             */
            void setTimeout(long seconds)                               { _timeoutSeconds   = seconds; }


            /**
             * @brief Sets whether to verify SSL certificates.
             * @param verify Set to false only in development/testing environments.
             */
            void setVerifySSL(bool verify)                              { _verifySSL        = verify; }


            /**
             * @brief Sets whether to follow HTTP redirects.
             * @param follow       Whether to follow redirects.
             * @param maxRedirects Maximum number of redirects to follow (default: 10).
             */
            void setFollowRedirects(bool follow, int maxRedirects = 10) { _followRedirects  = follow; _maxRedirects = maxRedirects; }


            /**
             * @brief Enables or disables in-memory cookie handling.
             * @param enable Whether to enable cookies (default: true).
             * @param jarPath Path to the cookie jar file for persistence across sessions.
             *                Empty string (default) = in-memory only, cookies lost on destruction
             */
            void enableCookies(bool enable = true, const std::string& jarPath = "");


            /**
             * @brief Clears all stored cookies.
             */
            void clearCookies();





            // -------------------------------------------------------------------------
            // User-Agent management
            // -------------------------------------------------------------------------

            /**
             * @brief Replaces the User-Agent pool with a custom list.
             * @details Has no effect if the provided list is empty.
             * @param agents The new list of User-Agent strings.
             */
            void setUserAgents(const std::vector<std::string>& agents);


            /**
             * @brief Forces a single fixed User-Agent and disables rotation.
             * @param agent The User-Agent string to use for all requests.
             */
            void setUserAgent(const std::string& agent);





            // -------------------------------------------------------------------------
            // Proxy management
            // -------------------------------------------------------------------------

            /**
             * @brief Replaces the proxy pool with the given list.
             * @param proxies The new list of ProxyConfig entries.
             */
            void setProxyPool(const std::vector<ProxyConfig>& proxies)  { _proxyPool = proxies; }


            /**
             * @brief Adds a single proxy to the pool.
             * @param proxy The ProxyConfig to add.
             */
            void addProxy(const ProxyConfig& proxy)                     { _proxyPool.push_back(proxy); }


            /**
             * @brief Clears the proxy pool.
             */
            void clearProxies()                                         { _proxyPool.clear(); }





            // -------------------------------------------------------------------------
            // Custom headers
            // -------------------------------------------------------------------------

            /**
             * @brief Adds or replaces a custom header sent with every request.
             * @param name  The header name (e.g. "Authorization").
             * @param value The header value (e.g. "Bearer token123").
             */
            void setHeader(const std::string& name, const std::string& value) { _customHeaders[name] = value; }


            /**
             * @brief Removes all custom headers.
             */
            void clearHeaders()                                                { _customHeaders.clear(); }



            

            // -------------------------------------------------------------------------
            // Requests
            // -------------------------------------------------------------------------

            /**
             * @brief Sends an HTTP GET request to the given URL.
             * @param url The target URL.
             * @return The HttpResponse containing the status code, body, and headers.
             */
            HttpResponse get(const std::string& url);


            /**
             * @brief Sends an HTTP POST request with a form-encoded body.
             * @param url      The target URL.
             * @param formData Key-value pairs to encode as application/x-www-form-urlencoded.
             * @return The HttpResponse containing the status code, body, and headers.
             */
            HttpResponse post(const std::string& url,
                              const std::map<std::string, std::string>& formData);


            /**
             * @brief Sends an HTTP POST request with a raw JSON body.
             * @param url      The target URL.
             * @param jsonBody The raw JSON string to send as application/json.
             * @return The HttpResponse containing the status code, body, and headers.
             */
            HttpResponse postJson(const std::string& url,
                                  const std::string& jsonBody);





            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /**
             * @brief Returns the total number of HTTP requests made (including retries).
             * @return The request counter.
             */
            long getTotalRequests() const { return _totalRequests; }





            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            /**
             * @brief Percent-encodes a string for safe use in URLs or form bodies.
             * @param value The raw string to encode.
             * @return The URL-encoded string.
             */
            static std::string urlEncode(const std::string& value);

    };

} // namespace CarScraper
