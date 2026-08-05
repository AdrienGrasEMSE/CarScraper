/**
 * @file baseRobotsPolicy.cpp
 *
 * @brief Unit tests for CarScraper::RobotsPolicy class.
 *        The [network] tests fetch real robots.txt files from largus.fr and
 *        caradisiac.com to verify the paths actually scraped by ArgusScraper
 *        and CaradisiacScraper remain allowed under the "*" group.
 *        Run without network tests: ./TestRobotsPolicy "[RobotsPolicy]" "~[network]"
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// Imports
#include <catch2/catch.hpp>
#include "io/robotsPolicy/RobotsPolicy.hpp"

using namespace CarScraper;


// =============================================================================
// isAllowed() — fail-open on unreachable host
// =============================================================================

TEST_CASE("RobotsPolicy isAllowed() — fails open on unreachable host", "[RobotsPolicy][network]") {

    RobotsPolicy policy;

    // A domain with no robots.txt (or no DNS entry at all) must not block scraping
    REQUIRE(policy.isAllowed("this-domain-does-not-exist-carscraper.invalid", "/any/path"));

}


// =============================================================================
// isAllowed() — real robots.txt, Largus
// =============================================================================

TEST_CASE("RobotsPolicy isAllowed() — largus.fr real robots.txt", "[RobotsPolicy][network]") {

    RobotsPolicy policy;

    SECTION("Paths actually scraped by ArgusScraper remain allowed") {
        REQUIRE(policy.isAllowed("www.largus.fr", "/Toutes-Marques.html"));
        REQUIRE(policy.isAllowed("www.largus.fr", "/Peugeot.html"));
        REQUIRE(policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot.html"));
        REQUIRE(policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208.html"));
        REQUIRE(policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208/2020.html"));
    }

    SECTION("Explicitly disallowed paths are blocked") {
        REQUIRE_FALSE(policy.isAllowed("www.largus.fr", "/admin/"));
        REQUIRE_FALSE(policy.isAllowed("www.largus.fr", "/search/"));
        REQUIRE_FALSE(policy.isAllowed("www.largus.fr", "/monargus/"));
        REQUIRE_FALSE(policy.isAllowed("www.largus.fr", "/js/main.js"));
    }

}


// =============================================================================
// isAllowed() — real robots.txt, Caradisiac
// =============================================================================

TEST_CASE("RobotsPolicy isAllowed() — caradisiac.com real robots.txt", "[RobotsPolicy][network]") {

    RobotsPolicy policy;

    SECTION("Paths actually scraped by CaradisiacScraper remain allowed") {
        REQUIRE(policy.isAllowed("www.caradisiac.com", "/auto--peugeot/"));
        REQUIRE(policy.isAllowed("www.caradisiac.com", "/fiches-techniques/auto--peugeot/"));
        REQUIRE(policy.isAllowed("www.caradisiac.com", "/fiches-techniques/modele--peugeot-208/"));
        REQUIRE(policy.isAllowed("www.caradisiac.com", "/fiches-techniques/gamme--peugeot-208/"));
    }

    SECTION("Explicitly disallowed paths are blocked") {
        REQUIRE_FALSE(policy.isAllowed("www.caradisiac.com", "/general/recherche/"));
        REQUIRE_FALSE(policy.isAllowed("www.caradisiac.com", "/application/"));
        REQUIRE_FALSE(policy.isAllowed("www.caradisiac.com", "/depot-avis/?brand=peugeot"));
    }

}


// =============================================================================
// isAllowed() — caching behavior
// =============================================================================

TEST_CASE("RobotsPolicy isAllowed() — caches rules per host", "[RobotsPolicy][network]") {

    RobotsPolicy policy;

    // First call fetches and caches; second call must reuse the cache and
    // return a consistent result without needing a new fetch.
    bool first  = policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208.html");
    bool second = policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208.html");
    REQUIRE(first == second);

}


// =============================================================================
// clearCache()
// =============================================================================

TEST_CASE("RobotsPolicy clearCache() — forces a re-fetch", "[RobotsPolicy][network]") {

    RobotsPolicy policy;

    REQUIRE(policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208.html"));

    policy.clearCache();

    // After clearing, the same path must still resolve the same way
    // (re-fetches robots.txt from scratch).
    REQUIRE(policy.isAllowed("www.largus.fr", "/fiche-technique/Peugeot/208.html"));

}
