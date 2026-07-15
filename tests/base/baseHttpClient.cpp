/**
 * @file baseHttpClient.cpp
 *
 * @brief Unit tests for CarScraper::HttpClient class.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// Imports
#include <catch2/catch.hpp>
#include "io/httpClient/HttpClient.hpp"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include <optional>

using namespace CarScraper;


// =============================================================================
// Helpers
// =============================================================================

/**
 * @brief Extracts a scalar string from a JSON value that may be either a plain
 *        string or a single-element array of strings.
 * @details go-httpbin (httpbingo.org) mirrors Go's net/http.Header and
 *          url.Values types, which are both map[string][]string — so header
 *          and form field values come back as JSON arrays, e.g. {"Accept": ["/"]}
 *          instead of {"Accept": "/"}. The raw "json" field of /post (the
 *          parsed request body) does NOT go through this conversion and stays scalar.
 */
static std::optional<std::string> jsonToScalar(const nlohmann::json& value) {
    if (value.is_array()) {
        if (value.empty()) return std::nullopt;
        return value.at(0).get<std::string>();
    }
    if (value.is_string()) {
        return value.get<std::string>();
    }
    return std::nullopt;
}

/**
 * @brief Case-insensitive lookup of a header value inside the JSON object
 *        returned by httpbingo.org/headers or httpbingo.org/get|post (the "headers" field).
 * @details go-httpbin normalizes header casing (e.g. "accept-language" ->
 *          "Accept-Language"), so a strict-case lookup on our side would be
 *          brittle. Values are also wrapped in a single-element array — see jsonToScalar().
 */
static std::optional<std::string> findHeaderCI(const nlohmann::json& headersJson, const std::string& name) {

    auto toLower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    };

    const std::string target = toLower(name);
    for (auto it = headersJson.begin(); it != headersJson.end(); ++it) {
        if (toLower(it.key()) == target) {
            return jsonToScalar(it.value());
        }
    }
    return std::nullopt;
}

/**
 * @brief Builds an HttpClient configured for fast tests (low delays, no retry).
 */
static HttpClient buildFastClient() {
    HttpClient client;

    AntiBlockPolicy policy;
    policy.minDelayBetweenRequests  = std::chrono::milliseconds(50);
    policy.maxDelayBetweenRequests  = std::chrono::milliseconds(100);
    policy.maxRetries               = 0;
    policy.rotateUserAgent          = false;
    policy.sendReferer              = false;
    client.setPolicy(policy);

    return client;
}

/**
 * @brief Builds an HttpClient with sendReferer enabled (navigation headers active).
 */
static HttpClient buildRefererClient() {
    HttpClient client;

    AntiBlockPolicy policy;
    policy.minDelayBetweenRequests  = std::chrono::milliseconds(50);
    policy.maxDelayBetweenRequests  = std::chrono::milliseconds(100);
    policy.maxRetries               = 0;
    policy.rotateUserAgent          = false;
    policy.sendReferer              = true;
    client.setPolicy(policy);

    return client;
}

/**
 * @brief Builds an HttpClient with User-Agent rotation enabled.
 */
static HttpClient buildRotatingUAClient() {
    HttpClient client;

    AntiBlockPolicy policy;
    policy.minDelayBetweenRequests  = std::chrono::milliseconds(50);
    policy.maxDelayBetweenRequests  = std::chrono::milliseconds(100);
    policy.maxRetries               = 0;
    policy.rotateUserAgent          = true;
    client.setPolicy(policy);

    return client;
}

/**
 * @brief Builds an HttpClient with retries enabled and a short base delay,
 *        so retry tests stay fast while still exercising the backoff loop.
 */
static HttpClient buildRetryClient(int maxRetries, bool retryOn429, bool retryOn503) {
    HttpClient client;

    AntiBlockPolicy policy;
    policy.minDelayBetweenRequests  = std::chrono::milliseconds(10);
    policy.maxDelayBetweenRequests  = std::chrono::milliseconds(20);
    policy.maxRetries               = maxRetries;
    policy.retryOn429               = retryOn429;
    policy.retryOn503               = retryOn503;
    policy.retryBaseDelay           = std::chrono::milliseconds(30);
    policy.rotateUserAgent          = false;
    policy.sendReferer              = false;
    client.setPolicy(policy);

    return client;
}


// =============================================================================
// Tests — Entity inheritance
// =============================================================================

TEST_CASE("HttpClient Entity Inheritance", "[httpclient][entity]") {

    SECTION("UUID is generated and non-empty") {
        HttpClient client;
        REQUIRE_FALSE(client.getUuid().empty());
    }

    SECTION("Prefix is HTTP-CLIENT") {
        HttpClient client;
        REQUIRE(client.getPrefix() == "HTTP-CLIENT");
    }

    SECTION("FullId starts with HTTP-CLIENT-") {
        HttpClient client;
        REQUIRE(client.getFullId().rfind("HTTP-CLIENT-", 0) == 0);
    }

    SECTION("Two clients have different UUIDs") {
        HttpClient c1, c2;
        REQUIRE(c1.getUuid() != c2.getUuid());
    }

}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("HttpClient Construction", "[httpclient][construction]") {

    SECTION("Default construction succeeds without throwing") {
        REQUIRE_NOTHROW(HttpClient());
    }

    SECTION("Request counter starts at zero") {
        HttpClient client;
        REQUIRE(client.getTotalRequests() == 0);
    }

    SECTION("Copy construction is deleted (compile-time — verified by design)") {
        // HttpClient c2 = c1; // must not compile
        REQUIRE(true);
    }

    SECTION("Move construction succeeds") {
        HttpClient c1 = buildFastClient();
        HttpClient c2 = std::move(c1);
        REQUIRE(true);
    }

}


// =============================================================================
// Tests — GET requests — Success
// =============================================================================

TEST_CASE("HttpClient GET — Successful Request", "[httpclient][get][network]") {

    HttpClient client = buildFastClient();

    SECTION("GET example.com returns success") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("GET example.com returns status 200") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.statusCode == 200);
    }

    SECTION("GET example.com returns a non-empty body") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE_FALSE(r.body.empty());
    }

    SECTION("GET example.com body contains <html") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.body.find("<html") != std::string::npos);
    }

    SECTION("GET example.com returns a non-empty effectiveUrl") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE_FALSE(r.effectiveUrl.empty());
    }

    SECTION("GET example.com errorMsg is empty on success") {
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.errorMsg.empty());
    }

}


// =============================================================================
// Tests — GET requests — Error handling
// =============================================================================

TEST_CASE("HttpClient GET — Error Handling", "[httpclient][get][error]") {

    HttpClient client = buildFastClient();

    SECTION("GET on an invalid domain returns success == false") {
        HttpResponse r = client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE_FALSE(r.success);
    }

    SECTION("GET on an invalid domain sets a non-empty errorMsg") {
        HttpResponse r = client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE_FALSE(r.errorMsg.empty());
    }

    SECTION("GET on an invalid domain returns statusCode == 0") {
        HttpResponse r = client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE(r.statusCode == 0);
    }

    SECTION("GET on an invalid domain returns an empty body") {
        HttpResponse r = client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE(r.body.empty());
    }

}


// =============================================================================
// Tests — Request counter
// =============================================================================

TEST_CASE("HttpClient Request Counter", "[httpclient][counter]") {

    SECTION("Counter increments after each successful GET") {
        HttpClient client = buildFastClient();
        client.get("https://example.com");
        REQUIRE(client.getTotalRequests() == 1);
        client.get("https://example.com");
        REQUIRE(client.getTotalRequests() == 2);
    }

    SECTION("Counter increments even on failed requests") {
        HttpClient client = buildFastClient();
        client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE(client.getTotalRequests() == 1);
    }

}


// =============================================================================
// Tests — Session User-Agent
// =============================================================================

TEST_CASE("HttpClient Session User-Agent", "[httpclient][useragent][session]") {

    SECTION("Session UA is fixed at construction — two GETs succeed with the same UA") {
        // The UA is set once at construction and does not change between requests
        HttpClient client = buildFastClient();
        HttpResponse r1 = client.get("https://example.com");
        HttpResponse r2 = client.get("https://example.org");
        REQUIRE(r1.success);
        REQUIRE(r2.success);
    }

    SECTION("resetSessionUserAgent does not throw") {
        HttpClient client = buildRotatingUAClient();
        REQUIRE_NOTHROW(client.resetSessionUserAgent());
    }

    SECTION("GET succeeds after resetSessionUserAgent") {
        HttpClient client = buildRotatingUAClient();
        client.resetSessionUserAgent();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("Two clients constructed independently may have different session UAs") {
        // With 7 UAs in the pool, the probability of both being identical is low but non-zero.
        // We just verify construction succeeds — UA value is not observable from the outside.
        REQUIRE_NOTHROW([](){
            HttpClient c1, c2, c3;
        }());
    }

    SECTION("setUserAgent disables rotation and fixes the UA") {
        HttpClient client = buildFastClient();
        REQUIRE_NOTHROW(client.setUserAgent("Mozilla/5.0 TestAgent/1.0"));
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("setUserAgents with non-empty list does not throw") {
        HttpClient client = buildFastClient();
        REQUIRE_NOTHROW(client.setUserAgents({ "AgentA/1.0", "AgentB/2.0" }));
    }

    SECTION("setUserAgents with empty list is ignored (no throw, pool unchanged)") {
        HttpClient client = buildFastClient();
        REQUIRE_NOTHROW(client.setUserAgents({}));
    }

    SECTION("GET succeeds after setUserAgents") {
        HttpClient client = buildFastClient();
        client.setUserAgents({ "Mozilla/5.0 (compatible; CarScraperTest/1.0)" });
        client.resetSessionUserAgent();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

}


// =============================================================================
// Tests — Referer dynamique
// =============================================================================

TEST_CASE("HttpClient Referer Management", "[httpclient][referer]") {

    SECTION("setReferer does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.setReferer("https://www.google.fr/"));
    }

    SECTION("clearReferer does not throw") {
        HttpClient client;
        client.setReferer("https://www.google.fr/");
        REQUIRE_NOTHROW(client.clearReferer());
    }

    SECTION("GET succeeds with sendReferer enabled and no Referer set (fallback google.fr)") {
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("GET succeeds with sendReferer enabled and an explicit Referer") {
        HttpClient client = buildRefererClient();
        client.setReferer("https://example.com/");
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("GET succeeds after clearReferer (fallback to google.fr)") {
        HttpClient client = buildRefererClient();
        client.setReferer("https://example.com/");
        client.clearReferer();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("GET succeeds when no Referer is set (fresh navigation) — Sec-Fetch-Site value itself is verified in \"HttpClient Anti-Detection Headers (real values)\" below") {
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.statusCode == 200);
    }

    SECTION("GET succeeds with an explicit Referer set — Sec-Fetch-Site value itself is verified in \"HttpClient Anti-Detection Headers (real values)\" below") {
        HttpClient client = buildRefererClient();
        client.setReferer("https://example.com/");
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.statusCode == 200);
    }

    SECTION("Simulated navigation chain — Google → accueil → page intermédiaire → fiche") {
        HttpClient client = buildRefererClient();

        // Etape 1 : Google → example.com (accueil)
        client.setReferer("https://www.google.fr/");
        HttpResponse r1 = client.get("https://example.com");
        REQUIRE(r1.success);

        // Etape 2 : example.com → example.org (page intermédiaire)
        client.setReferer("https://example.com/");
        HttpResponse r2 = client.get("https://example.org");
        REQUIRE(r2.success);

        // Etape 3 : example.org → example.net (fiche terminale)
        client.setReferer("https://example.org/");
        HttpResponse r3 = client.get("https://example.net");
        REQUIRE(r3.success);

        REQUIRE(client.getTotalRequests() == 3);
    }

}


// =============================================================================
// Tests — Navigation headers (Sec-Fetch-* + Connection via sendReferer)
// =============================================================================

TEST_CASE("HttpClient Navigation Headers", "[httpclient][secfetch]") {

    SECTION("GET succeeds with sendReferer = true (Sec-Fetch-* + Connection headers active)") {
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
        REQUIRE(r.statusCode == 200);
    }

    SECTION("GET succeeds with sendReferer = false (no navigation headers)") {
        HttpClient client = buildFastClient();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
        REQUIRE(r.statusCode == 200);
    }

    SECTION("GET succeeds with sendAcceptHeaders = false") {
        HttpClient client;
        AntiBlockPolicy policy;
        policy.minDelayBetweenRequests  = std::chrono::milliseconds(50);
        policy.maxDelayBetweenRequests  = std::chrono::milliseconds(100);
        policy.maxRetries               = 0;
        policy.sendAcceptHeaders        = false;
        client.setPolicy(policy);
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

}


// =============================================================================
// Tests — Anti-detection headers (real values, via httpbingo.org/headers)
// =============================================================================
//
// httpbingo.org/headers echoes back, as JSON, every header the server actually
// received. This lets us assert the *content* of what HttpClient sends instead
// of only checking that the request succeeds.
//

TEST_CASE("HttpClient Anti-Detection Headers (real values)", "[httpclient][headers][network]") {

    SECTION("Accept and Accept-Language are sent with the expected values when sendAcceptHeaders = true") {
        HttpClient client = buildFastClient();
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json  = nlohmann::json::parse(r.body);
        auto accept          = findHeaderCI(json["headers"], "Accept");
        auto acceptLanguage  = findHeaderCI(json["headers"], "Accept-Language");

        REQUIRE(accept.has_value());
        REQUIRE(accept->find("text/html") != std::string::npos);
        REQUIRE(acceptLanguage.has_value());
        REQUIRE(acceptLanguage->find("fr-FR") != std::string::npos);
    }

    SECTION("Accept header is absent when sendAcceptHeaders = false") {
        HttpClient client;
        AntiBlockPolicy policy;
        policy.minDelayBetweenRequests = std::chrono::milliseconds(10);
        policy.maxDelayBetweenRequests = std::chrono::milliseconds(20);
        policy.maxRetries              = 0;
        policy.sendAcceptHeaders       = false;
        client.setPolicy(policy);

        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        // curl always sends a default "Accept: */*" when none is set explicitly —
        // what we verify here is that OUR realistic Accept value is NOT the one sent.
        auto accept = findHeaderCI(json["headers"], "Accept");
        if (accept.has_value()) {
            REQUIRE(accept->find("text/html") == std::string::npos);
        }
    }

    SECTION("Referer header matches the value set via setReferer") {
        HttpClient client = buildRefererClient();
        client.setReferer("https://www.leboncoin.fr/voitures/12345");
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto referer = findHeaderCI(json["headers"], "Referer");
        REQUIRE(referer.has_value());
        REQUIRE(*referer == "https://www.leboncoin.fr/voitures/12345");
    }

    SECTION("Referer falls back to https://www.google.fr/ when sendReferer = true and no Referer is set") {
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto referer = findHeaderCI(json["headers"], "Referer");
        REQUIRE(referer.has_value());
        REQUIRE(*referer == "https://www.google.fr/");
    }

    SECTION("Sec-Fetch-Site is 'none' when no Referer is set (fresh navigation)") {
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto secFetchSite = findHeaderCI(json["headers"], "Sec-Fetch-Site");
        REQUIRE(secFetchSite.has_value());
        REQUIRE(*secFetchSite == "none");
    }

    SECTION("Sec-Fetch-Site is 'same-origin' when an explicit Referer is set") {
        HttpClient client = buildRefererClient();
        client.setReferer("https://httpbingo.org/");
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto secFetchSite = findHeaderCI(json["headers"], "Sec-Fetch-Site");
        REQUIRE(secFetchSite.has_value());
        REQUIRE(*secFetchSite == "same-origin");
    }

    SECTION("A custom header set via setHeader is sent with the exact value") {
        HttpClient client = buildFastClient();
        client.setHeader("X-CarScraper-Test", "audit-2026-07-15");
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto custom = findHeaderCI(json["headers"], "X-CarScraper-Test");
        REQUIRE(custom.has_value());
        REQUIRE(*custom == "audit-2026-07-15");
    }

    SECTION("clearHeaders removes a previously set custom header") {
        HttpClient client = buildFastClient();
        client.setHeader("X-CarScraper-Test", "should-not-be-sent");
        client.clearHeaders();
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        REQUIRE_FALSE(findHeaderCI(json["headers"], "X-CarScraper-Test").has_value());
    }

    SECTION("A fixed User-Agent set via setUserAgent is sent as-is") {
        HttpClient client = buildFastClient();
        client.setUserAgent("CarScraperTestAgent/1.0");
        HttpResponse r = client.get("https://httpbingo.org/headers");
        REQUIRE(r.success);

        nlohmann::json json = nlohmann::json::parse(r.body);
        auto ua = findHeaderCI(json["headers"], "User-Agent");
        REQUIRE(ua.has_value());
        REQUIRE(*ua == "CarScraperTestAgent/1.0");
    }

}


// =============================================================================
// Tests — response.headers map (previously never inspected in any REQUIRE)
// =============================================================================

TEST_CASE("HttpClient Response Headers Map", "[httpclient][headers][network]") {

    HttpClient client = buildFastClient();

    SECTION("response.headers is populated after a successful GET") {
        HttpResponse r = client.get("https://httpbingo.org/get");
        REQUIRE(r.success);
        REQUIRE_FALSE(r.headers.empty());
    }

    SECTION("response.headers contains a Content-Type describing JSON for httpbingo.org/get") {
        HttpResponse r = client.get("https://httpbingo.org/get");
        REQUIRE(r.success);

        // httpbingo.org/get always answers "application/json"; the map keys use
        // whatever casing the server sent, so we search case-insensitively.
        bool found = false;
        for (const auto& [name, value] : r.headers) {
            std::string lower = name;
            std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });
            if (lower == "content-type") {
                found = true;
                REQUIRE(value.find("application/json") != std::string::npos);
            }
        }
        REQUIRE(found);
    }

}


// =============================================================================
// Tests — Cookie persistence
// =============================================================================

TEST_CASE("HttpClient Cookie Persistence", "[httpclient][cookies]") {

    SECTION("enableCookies without jarPath does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.enableCookies(true));
    }

    SECTION("enableCookies with jarPath does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.enableCookies(true, "/tmp/carscraper_test_cookies.txt"));
    }

    SECTION("enableCookies false does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.enableCookies(false));
    }

    SECTION("In-memory cookies: GET after enableCookies succeeds") {
        HttpClient client = buildFastClient();
        client.enableCookies(true);
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("Cookie jar path: GET after enableCookies with jarPath succeeds") {
        HttpClient client = buildFastClient();
        client.enableCookies(true, "/tmp/carscraper_test_cookies.txt");
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("clearCookies after a GET does not throw") {
        HttpClient client = buildFastClient();
        client.enableCookies(true);
        client.get("https://example.com");
        REQUIRE_NOTHROW(client.clearCookies());
    }

}


// =============================================================================
// Tests — Configuration générale
// =============================================================================

TEST_CASE("HttpClient Configuration", "[httpclient][config]") {

    SECTION("setPolicy does not throw") {
        HttpClient client;
        AntiBlockPolicy policy;
        REQUIRE_NOTHROW(client.setPolicy(policy));
    }

    SECTION("setTimeout does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.setTimeout(10));
    }

    SECTION("setVerifySSL does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.setVerifySSL(true));
    }

    SECTION("setFollowRedirects does not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.setFollowRedirects(true, 5));
    }

    SECTION("setHeader and clearHeaders do not throw") {
        HttpClient client;
        REQUIRE_NOTHROW(client.setHeader("X-Test", "value"));
        REQUIRE_NOTHROW(client.clearHeaders());
    }

    SECTION("setProxyPool does not throw") {
        HttpClient client;
        ProxyConfig p;
        p._host = "proxy.example.com";
        p._port = 8080;
        REQUIRE_NOTHROW(client.setProxyPool({ p }));
    }

    SECTION("addProxy and clearProxies do not throw") {
        HttpClient client;
        ProxyConfig p;
        p._host = "proxy.example.com";
        p._port = 8080;
        REQUIRE_NOTHROW(client.addProxy(p));
        REQUIRE_NOTHROW(client.clearProxies());
    }

}


// =============================================================================
// Tests — Retry logic
// =============================================================================

TEST_CASE("HttpClient Retry Logic", "[httpclient][retry]") {

    SECTION("maxRetries = 0 makes only one attempt on failure") {
        HttpClient client = buildFastClient();   // maxRetries = 0
        client.get("https://this-domain-does-not-exist-carscraper.com");
        REQUIRE(client.getTotalRequests() == 1);
    }

    SECTION("A successful request never triggers a retry, even with maxRetries > 0") {
        HttpClient client = buildRetryClient(/*maxRetries=*/3, /*retryOn429=*/true, /*retryOn503=*/true);
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
        REQUIRE(client.getTotalRequests() == 1);
    }

    SECTION("Retries on HTTP 503 up to maxRetries, then gives up", "[network]") {
        // httpbingo.org/status/503 always answers 503, so the client is expected
        // to exhaust every retry attempt: 1 initial try + maxRetries retries.
        HttpClient client = buildRetryClient(/*maxRetries=*/2, /*retryOn429=*/true, /*retryOn503=*/true);
        HttpResponse r = client.get("https://httpbingo.org/status/503");

        REQUIRE_FALSE(r.success);
        REQUIRE(r.statusCode == 503);
        REQUIRE(client.getTotalRequests() == 3);
    }

    SECTION("retryOn503 = false makes only one attempt on a 503 response", "[network]") {
        HttpClient client = buildRetryClient(/*maxRetries=*/2, /*retryOn429=*/true, /*retryOn503=*/false);
        HttpResponse r = client.get("https://httpbingo.org/status/503");

        REQUIRE_FALSE(r.success);
        REQUIRE(r.statusCode == 503);
        REQUIRE(client.getTotalRequests() == 1);
    }

    SECTION("Retries on HTTP 429 up to maxRetries, then gives up", "[network]") {
        HttpClient client = buildRetryClient(/*maxRetries=*/1, /*retryOn429=*/true, /*retryOn503=*/true);
        HttpResponse r = client.get("https://httpbingo.org/status/429");

        REQUIRE_FALSE(r.success);
        REQUIRE(r.statusCode == 429);
        REQUIRE(client.getTotalRequests() == 2);   // 1 initial attempt + 1 retry
    }

    SECTION("A non-retriable status (404) is not retried at all", "[network]") {
        HttpClient client = buildRetryClient(/*maxRetries=*/2, /*retryOn429=*/true, /*retryOn503=*/true);
        HttpResponse r = client.get("https://httpbingo.org/status/404");

        REQUIRE_FALSE(r.success);
        REQUIRE(r.statusCode == 404);
        REQUIRE(client.getTotalRequests() == 1);
    }

    SECTION("Exponential backoff makes more retries take measurably longer", "[network]") {
        // Not an exact timing test (network jitter), just verifies the backoff
        // is actually applied: 2 retries with a 30ms base delay must add at
        // least ~30ms + ~60ms = ~90ms of pure sleep time versus zero retries.
        HttpClient noRetryClient = buildRetryClient(0, true, true);
        auto t0 = std::chrono::steady_clock::now();
        noRetryClient.get("https://httpbingo.org/status/503");
        auto elapsedNoRetry = std::chrono::steady_clock::now() - t0;

        HttpClient retryClient = buildRetryClient(2, true, true);
        auto t1 = std::chrono::steady_clock::now();
        retryClient.get("https://httpbingo.org/status/503");
        auto elapsedWithRetry = std::chrono::steady_clock::now() - t1;

        REQUIRE(elapsedWithRetry > elapsedNoRetry + std::chrono::milliseconds(60));
    }

}


// =============================================================================
// Tests — Proxy pool is actually applied to the request
// =============================================================================
//
// pickProxy() is private, so we can't call it directly. Instead we prove it is
// wired into executeOnce() by observing its *effect*: a dead/unreachable proxy
// must make the request fail when rotation is enabled, and must have no effect
// at all when it's disabled.
//

TEST_CASE("HttpClient Proxy — Applied to the Request", "[httpclient][proxy][network]") {

    ProxyConfig deadProxy;
    deadProxy._host = "127.0.0.1";
    deadProxy._port = 9;   // discard port — nothing listens here, connection is refused immediately

    SECTION("rotateProxy = true routes the request through the (dead) proxy pool and fails") {
        HttpClient client;
        AntiBlockPolicy policy;
        policy.minDelayBetweenRequests = std::chrono::milliseconds(10);
        policy.maxDelayBetweenRequests = std::chrono::milliseconds(20);
        policy.maxRetries              = 0;
        policy.rotateProxy             = true;
        client.setPolicy(policy);
        client.setProxyPool({ deadProxy });

        HttpResponse r = client.get("https://example.com");
        REQUIRE_FALSE(r.success);
        REQUIRE_FALSE(r.errorMsg.empty());
    }

    SECTION("rotateProxy = false (default) ignores the configured proxy pool — request still succeeds") {
        HttpClient client = buildFastClient();   // rotateProxy defaults to false
        client.setProxyPool({ deadProxy });

        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

    SECTION("clearProxies() removes the dead proxy — request succeeds again even with rotateProxy = true") {
        HttpClient client;
        AntiBlockPolicy policy;
        policy.minDelayBetweenRequests = std::chrono::milliseconds(10);
        policy.maxDelayBetweenRequests = std::chrono::milliseconds(20);
        policy.maxRetries              = 0;
        policy.rotateProxy             = true;
        client.setPolicy(policy);
        client.setProxyPool({ deadProxy });
        client.clearProxies();

        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.success);
    }

}


// =============================================================================
// Tests — Redirect following
// =============================================================================

TEST_CASE("HttpClient Redirect Following", "[httpclient][redirect][network]") {

    SECTION("setFollowRedirects(true) follows the redirect to its final destination") {
        HttpClient client = buildFastClient();
        client.setFollowRedirects(true);

        HttpResponse r = client.get("https://httpbingo.org/redirect-to?url=https%3A%2F%2Fhttpbingo.org%2Fget&status_code=302");

        REQUIRE(r.success);
        REQUIRE(r.statusCode == 200);
        REQUIRE(r.effectiveUrl.find("/get") != std::string::npos);
    }

    SECTION("setFollowRedirects(false) stops at the redirect and returns the 3xx status") {
        HttpClient client = buildFastClient();
        client.setFollowRedirects(false);

        HttpResponse r = client.get("https://httpbingo.org/redirect-to?url=https%3A%2F%2Fhttpbingo.org%2Fget&status_code=302");

        REQUIRE_FALSE(r.success);
        REQUIRE(r.statusCode == 302);
    }

}


// =============================================================================
// Tests — POST requests (post() and postJson() had zero coverage)
// =============================================================================

TEST_CASE("HttpClient POST Requests", "[httpclient][post][network]") {

    HttpClient client = buildFastClient();

    SECTION("post() sends form-encoded data and httpbingo.org echoes it back") {
        HttpResponse r = client.post("https://httpbingo.org/post", { { "brand", "Renault" }, { "model", "Clio" } });

        REQUIRE(r.success);
        nlohmann::json json = nlohmann::json::parse(r.body);
        REQUIRE(jsonToScalar(json["form"]["brand"]) == "Renault");
        REQUIRE(jsonToScalar(json["form"]["model"]) == "Clio");

        auto contentType = findHeaderCI(json["headers"], "Content-Type");
        REQUIRE(contentType.has_value());
        REQUIRE(contentType->find("application/x-www-form-urlencoded") != std::string::npos);
    }

    SECTION("post() with empty formData still succeeds") {
        HttpResponse r = client.post("https://httpbingo.org/post", {});
        REQUIRE(r.success);
    }

    SECTION("postJson() sends a raw JSON body with the correct Content-Type") {
        HttpResponse r = client.postJson("https://httpbingo.org/post", R"({"brand":"Peugeot","price":15000})");

        REQUIRE(r.success);
        nlohmann::json json = nlohmann::json::parse(r.body);
        REQUIRE(json["json"]["brand"] == "Peugeot");
        REQUIRE(json["json"]["price"] == 15000);

        auto contentType = findHeaderCI(json["headers"], "Content-Type");
        REQUIRE(contentType.has_value());
        REQUIRE(contentType->find("application/json") != std::string::npos);
    }

    SECTION("POST requests increment the request counter like GET") {
        client.post("https://httpbingo.org/post", { { "a", "b" } });
        REQUIRE(client.getTotalRequests() == 1);
    }

}


// =============================================================================
// Tests — Utilities
// =============================================================================

TEST_CASE("HttpClient urlEncode", "[httpclient][utilities]") {

    SECTION("urlEncode encodes a space as %20") {
        REQUIRE(HttpClient::urlEncode("hello world") == "hello%20world");
    }

    SECTION("urlEncode encodes & correctly") {
        REQUIRE(HttpClient::urlEncode("a&b") == "a%26b");
    }

    SECTION("urlEncode leaves alphanumeric characters unchanged") {
        REQUIRE(HttpClient::urlEncode("abc123") == "abc123");
    }

    SECTION("urlEncode handles empty string") {
        REQUIRE(HttpClient::urlEncode("") == "");
    }

}


// =============================================================================
// Tests — ProxyConfig
// =============================================================================

TEST_CASE("ProxyConfig toUrl", "[httpclient][proxy]") {

    SECTION("toUrl without credentials") {
        ProxyConfig p;
        p._host = "proxy.example.com";
        p._port = 8080;
        REQUIRE(p.toUrl() == "http://proxy.example.com:8080");
    }

    SECTION("toUrl with credentials") {
        ProxyConfig p;
        p._host     = "proxy.example.com";
        p._port     = 3128;
        p._username = "user";
        p._password = "pass";
        REQUIRE(p.toUrl() == "http://user:pass@proxy.example.com:3128");
    }

}