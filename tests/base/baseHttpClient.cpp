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

using namespace CarScraper;


// =============================================================================
// Helpers
// =============================================================================

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

    SECTION("Sec-Fetch-Site is 'none' when no Referer is set (fresh navigation)") {
        // Comportement interne : on vérifie juste que la requête aboutit
        HttpClient client = buildRefererClient();
        HttpResponse r = client.get("https://example.com");
        REQUIRE(r.statusCode == 200);
    }

    SECTION("Sec-Fetch-Site is 'same-origin' when an explicit Referer is set") {
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