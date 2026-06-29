/**
 * @file baseGenericCarScraper.cpp
 *
 * @brief Unit tests for GenericCarScraper class.
 *
 * @author Adrien GRAS
 * @date 2026-06-28
 */


// Imports
#include <catch2/catch.hpp>
#include "core/scraper/GenericCarScraper.hpp"
#include "core/utils/Constant.hpp"

using namespace CarScraper;


// =============================================================================
// Tests — Entity inheritance
// =============================================================================

TEST_CASE("GenericCarScraper Entity Inheritance", "[genericscraper][entity]") {

    SECTION("Default prefix is SCRAPER") {
        GenericCarScraper scraper;
        REQUIRE(scraper.getPrefix() == "SCRAPER");
    }

    SECTION("Custom prefix is applied") {
        GenericCarScraper scraper("ARGUS-SCRAPER");
        REQUIRE(scraper.getPrefix() == "ARGUS-SCRAPER");
    }

    SECTION("UUID is generated and non-empty") {
        GenericCarScraper scraper;
        REQUIRE_FALSE(scraper.getUuid().empty());
    }

    SECTION("FullId starts with prefix") {
        GenericCarScraper scraper;
        REQUIRE(scraper.getFullId().rfind("SCRAPER-", 0) == 0);
    }

    SECTION("Two scrapers have different UUIDs") {
        GenericCarScraper s1, s2;
        REQUIRE(s1.getUuid() != s2.getUuid());
    }

}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("GenericCarScraper Construction", "[genericscraper][construction]") {

    SECTION("Default construction succeeds without throwing") {
        REQUIRE_NOTHROW(GenericCarScraper());
    }

    SECTION("Custom prefix construction succeeds without throwing") {
        REQUIRE_NOTHROW(GenericCarScraper("ARGUS-SCRAPER"));
    }

    SECTION("carBrand is DEFAULT_STR after default construction") {
        GenericCarScraper scraper;
        REQUIRE(scraper.getCarBrand() == DEFAULT_STR);
    }

    SECTION("carModel is DEFAULT_STR after default construction") {
        GenericCarScraper scraper;
        REQUIRE(scraper.getCarModel() == DEFAULT_STR);
    }

    SECTION("outputDirectory is HTML_DIR after default construction") {
        GenericCarScraper scraper;
        REQUIRE(scraper.getOutputDirectory() == HTML_DIR);
    }

}


// =============================================================================
// Tests — setCarBrand
// =============================================================================

TEST_CASE("GenericCarScraper setCarBrand", "[genericscraper][setter][brand]") {

    SECTION("Valid brand is stored") {
        GenericCarScraper scraper;
        scraper.setCarBrand("Renault");
        REQUIRE(scraper.getCarBrand() == "Renault");
    }

    SECTION("Brand is normalized to titlecase") {
        GenericCarScraper scraper;
        scraper.setCarBrand("renault");
        REQUIRE(scraper.getCarBrand() == "Renault");
    }

    SECTION("Brand is normalized to titlecase from uppercase") {
        GenericCarScraper scraper;
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "Renault");
    }

    SECTION("Brand with accented characters is NFC normalized") {
        GenericCarScraper scraper;
        scraper.setCarBrand("citroën");
        REQUIRE_FALSE(scraper.getCarBrand().empty());
        REQUIRE(scraper.getCarBrand() != DEFAULT_STR);
    }

    SECTION("setCarBrand does not throw") {
        GenericCarScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarBrand("Peugeot"));
    }

}


// =============================================================================
// Tests — setCarModel
// =============================================================================

TEST_CASE("GenericCarScraper setCarModel", "[genericscraper][setter][model]") {

    SECTION("Valid model is stored") {
        GenericCarScraper scraper;
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

    SECTION("Model is normalized to titlecase") {
        GenericCarScraper scraper;
        scraper.setCarModel("clio");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

    SECTION("Model is normalized to titlecase from uppercase") {
        GenericCarScraper scraper;
        scraper.setCarModel("CLIO");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

    SECTION("setCarModel does not throw") {
        GenericCarScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarModel("308"));
    }

    SECTION("setCarBrand and setCarModel are independent") {
        GenericCarScraper scraper;
        scraper.setCarBrand("Renault");
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarBrand() == "Renault");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

}


// =============================================================================
// Tests — setOutputDirectory
// =============================================================================

TEST_CASE("GenericCarScraper setOutputDirectory", "[genericscraper][setter][folder]") {

    SECTION("Valid existing folder is stored") {
        GenericCarScraper scraper;
        scraper.setOutputDirectory("/tmp");
        REQUIRE(scraper.getOutputDirectory() == "/tmp");
    }

    SECTION("Non-existing folder falls back to HTML_DIR") {
        GenericCarScraper scraper;
        scraper.setOutputDirectory("/this/path/does/not/exist/carscraper");
        REQUIRE(scraper.getOutputDirectory() == HTML_DIR);
    }

    SECTION("setOutputDirectory does not throw on invalid path") {
        GenericCarScraper scraper;
        REQUIRE_NOTHROW(scraper.setOutputDirectory("/invalid/path"));
    }

    SECTION("setOutputDirectory does not throw on valid path") {
        GenericCarScraper scraper;
        REQUIRE_NOTHROW(scraper.setOutputDirectory("/tmp"));
    }

}


// =============================================================================
// Tests — scrapModel (base implementation)
// =============================================================================

TEST_CASE("GenericCarScraper scrapModel", "[genericscraper][scrapmodel]") {

    SECTION("Base scrapModel returns IGNORED_ACTION_CODE") {
        GenericCarScraper scraper;
        scraper.setCarBrand("Renault");
        scraper.setCarModel("Clio");
        REQUIRE(scraper.scrapModel() == IGNORED_ACTION_CODE);
    }

    SECTION("Base scrapModel does not throw") {
        GenericCarScraper scraper;
        REQUIRE_NOTHROW(scraper.scrapModel());
    }

    SECTION("Base scrapModel without brand/model still returns IGNORED_ACTION_CODE") {
        GenericCarScraper scraper;
        REQUIRE(scraper.scrapModel() == IGNORED_ACTION_CODE);
    }

}