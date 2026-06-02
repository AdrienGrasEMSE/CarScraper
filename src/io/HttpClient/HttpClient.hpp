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
        std::string _host;          ///< Proxy host (IP or domain)       | ex: "proxy.example.com"
        int         _port       = 0;///< Proxy port                      | ex: 8080
        std::string _username;      ///< Proxy username (optional)       | ex: "user"
        std::string _password;      ///< Proxy password (optional)       | ex: "pass"

        
        /**
         * @brief Converts the proxy configuration to a URL string.
         * @details If username and password are provided, the URL will include them in the format
         * "http://username:password@host:port".
         * @return The proxy URL
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
        int                         maxRetries              = 3;        ///< Maximum number of retry attempts on failure
        bool                        retryOn429              = true;     ///< Whether to retry on HTTP 429 Too Many Requests
        bool                        retryOn503              = true;     ///< Whether to retry on HTTP 503 Service Unavailable
        std::chrono::milliseconds   retryBaseDelay          { 2000 };   ///< Base delay for retries (in milliseconds, will be doubled on each attempt for exponential backoff)

        // Rotation
        bool                        rotateUserAgent         = true;     ///< Whether to rotate User-Agent header by picking a random one from the pool
        bool                        rotateProxy             = false;    ///< Whether to rotate proxies by picking a random one from the proxy pool

        // Realistic headers
        bool                        sendAcceptHeaders       = true;     ///< Whether to send realistic Accept and Accept-Language headers
        bool                        sendReferer             = false;    ///< Whether to send a spoofed Referer header (e.g. "https://www.google.com/")

    };


    


    // =========================================================================
    // Class HttpClient
    // =========================================================================

    /**
     * @class HttpClient
     * @brief A simple HTTP client that wraps around libcurl to perform HTTP requests with built-in anti-blocking
     * techniques.
     * 
     * @details HttpClient provides methods to perform GET and POST requests while automatically handling:
     * - Random delays between requests to mimic human browsing behavior (rate limiting with jitter)
     * - Automatic retries with exponential backoff on HTTP 429 and 503 errors
     * - Rotation of User-Agent headers from a predefined pool of realistic browser User-Agents
     * - Rotation of proxies from a provided proxy pool
     * - Sending realistic browser headers (Accept, Accept-Language, Referer)
     *
     * Depends on: libcurl
     * Compile with: -lcurl
     */
    class HttpClient {


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
            bool                                _cookiesEnabled = false;///< Whether to enable cookie handling (in-memory, no file storage)
            bool                                _followRedirects = true;///< Whether to follow HTTP redirects (3xx) automatically
            int                                 _maxRedirects   = 10;   ///< Maximum number of redirects to follow if _followRedirects is true
            bool                                _verifySSL      = true; ///< Whether to verify SSL certificates (set to false to ignore SSL errors, not recommended)
            long                                _timeoutSeconds = 30;   ///< Timeout for requests in seconds
            long                                _totalRequests  = 0;    ///< Total number of requests made (including retries)





            // -------------------------------------------------------------------------
            // Anti-blocking helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Picks a random User-Agent from the pool
             * @return A reference to the selected User-Agent string
             */
            const std::string& pickUserAgent();


            /**
             * @brief Picks a random proxy from the pool
             * @return A pointer to the selected proxy configuration, or nullptr if the pool is empty
             */
            const ProxyConfig* pickProxy();


            /**
             * @brief Applies rate limiting with jitter
             */
            void applyRateLimit();


            /**
             * @brief Computes the delay for retry attempts
             * @param attempt The current retry attempt number
             * @return The delay as a std::chrono::milliseconds object
             */
            std::chrono::milliseconds computeRetryDelay(int attempt) const;


            /**
             * @brief Determines if a request should be retried based on its status code
             * @param statusCode The HTTP status code of the request
             * @return true if the request should be retried, false otherwise
             */
            bool shouldRetry(int statusCode) const;





            // -------------------------------------------------------------------------
            // Internal execution
            // -------------------------------------------------------------------------

            /**
             * @brief Executes the request with retry logic on top
             * @param url The URL to request
             * @param method The HTTP method to use
             * @param body The request body
             * @param contentType The content type of the request
             * @return The response from the server
             */
            HttpResponse    executeWithRetry(
                                const std::string& url,
                                const std::string& method,
                                const std::string& body,
                                const std::string& contentType);


            /**
             * @brief Performs a single libcurl request (no retry)
             * @param url The URL to request
             * @param method The HTTP method to use
             * @param body The request body
             * @param contentType The content type of the request
             * @return The response from the server
             */
            HttpResponse    executeOnce(
                                const std::string& url,
                                const std::string& method,
                                const std::string& body,
                                const std::string& contentType);





            // -------------------------------------------------------------------------
            // Defaults
            // -------------------------------------------------------------------------

            /**
             * @brief Returns a curated list of realistic browser User-Agents
             * @return A vector of User-Agent strings
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
             * @brief Constructs a new HttpClient instance
             */
            HttpClient();


            /**
             * @brief Destructs the HttpClient instance
             */
            ~HttpClient();


            /**
             * @brief Copy constructor (deleted)
             */
            HttpClient(const HttpClient&)               = delete;


            /**
             * @brief Assignment operator (deleted)
             */
            HttpClient& operator=(const HttpClient&)    = delete;


            /**
             * @brief Move constructor
             * @param other The other HttpClient instance to move from
             */
            HttpClient(HttpClient&&)                    = default;


            /**
             * @brief Move assignment operator
             * @param other The other HttpClient instance to move from
             * @return A reference to the moved HttpClient instance
             */
            HttpClient& operator=(HttpClient&&)         = default;





            // -------------------------------------------------------------------------
            // Configuration
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the anti-block policy
             * @param policy The anti-block policy to use
             */
            void setPolicy(const AntiBlockPolicy& policy) { _policy = policy; }


            /**
             * @brief Sets the timeout for requests
             * @param seconds The timeout in seconds
             */
            void setTimeout(long seconds) { _timeoutSeconds = seconds; }


            /**
             * @brief Sets whether to verify SSL certificates
             * @param verify Whether to verify SSL certificates (set to false to ignore SSL errors, not recommended)
             */
            void setVerifySSL(bool verify) { _verifySSL = verify; }


            /**
             * @brief Sets whether to follow redirects
             * @param follow Whether to follow redirects
             * @param maxRedirects The maximum number of redirects to follow
             */
            void setFollowRedirects(bool follow, int maxRedirects = 10) { _followRedirects  = follow; _maxRedirects = maxRedirects; }


            /**
             * @brief Enables or disables cookie handling
             * @param enable Whether to enable cookie handling
             */
            void enableCookies(bool enable = true);


            /**
             * @brief Clears all cookies
             */
            void clearCookies();





            // -------------------------------------------------------------------------
            // User-Agent management
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the list of User-Agents to rotate through
             * @param agents The list of User-Agents
             */
            void setUserAgents(const std::vector<std::string>& agents);


            /**
             * @brief Forces a single fixed User-Agent (disables rotation)
             * @param agent The User-Agent to use
             */
            void setUserAgent(const std::string& agent);





            // -------------------------------------------------------------------------
            // Proxy management
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the proxy pool
             * @param proxies The list of proxies
             */
            void setProxyPool(const std::vector<ProxyConfig>& proxies) { _proxyPool = proxies; }


            /**
             * @brief Adds a proxy to the pool
             * @param proxy The proxy to add
             */
            void addProxy(const ProxyConfig& proxy) { _proxyPool.push_back(proxy); }


            /**
             * @brief Clears the proxy pool
             */
            void clearProxies() { _proxyPool.clear(); }





            // -------------------------------------------------------------------------
            // Custom headers
            // -------------------------------------------------------------------------

            /**
             * @brief Sets a custom header to include in every request
             * @param name The name of the header
             * @param value The value of the header
             */
            void setHeader(const std::string& name, const std::string& value) { _customHeaders[name] = value; }


            /**
             * @brief Clears all custom headers
             */
            void clearHeaders() { _customHeaders.clear(); }





            // -------------------------------------------------------------------------
            // Requests
            // -------------------------------------------------------------------------

            /**
             * @brief Sends a GET request to the specified URL
             * @param url The URL to send the request to
             * @return The response from the server
             */
            HttpResponse get(const std::string& url);


            /**
             * @brief Sends a POST request with form-encoded body
             * @param url The URL to send the request to
             * @param formData The form data to include in the request body
             * @return The response from the server
             */
            HttpResponse post(const std::string& url,
                              const std::map<std::string, std::string>& formData);


            /**
             * @brief Sends a POST request with a raw JSON body
             * @param url The URL to send the request to
             * @param jsonBody The JSON body to include in the request
             * @return The response from the server
             */
            HttpResponse postJson(const std::string& url,
                                  const std::string& jsonBody);





            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /**
             * @brief Gets the total number of requests made (including retries)
             * @return The total number of requests
             */
            long getTotalRequests() const { return _totalRequests; }





            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            /**
             * @brief Percent-encodes a string for use in URLs or form bodies
             * @param value The string to encode
             * @return The encoded string
             */
            static std::string urlEncode(const std::string& value);

    };

} // namespace CarScraper
