/**
 * @file baseGenericCarScraper.cpp
 *
 * @brief Unit tests for CarScraper::GenericCarScraper class.
 *        Since GenericCarScraper constructors are protected, tests instantiate
 *        a minimal concrete subclass (TestScraper) to exercise the base class logic.
 *
 * @author Adrien GRAS
 * @date 2026-06-29
 */


// Imports
#include <catch2/catch.hpp>
#include "core/scraper/GenericCarScraper.hpp"
#include "core/utils/Constant.hpp"

using namespace CarScraper;


// =============================================================================
// Minimal concrete subclass for testing
// =============================================================================

/**
 * @class TestScraper
 * @brief Minimal concrete subclass of GenericCarScraper used only in unit tests.
 *        Exposes the protected constructors and provides a trivial scrapModel().
 */
class TestScraper : public CarScraper::GenericCarScraper {
public:
    TestScraper()                           : GenericCarScraper("TEST-SCRAPER") {}
    TestScraper(const std::string& prefix)  : GenericCarScraper(prefix) {}
};


// =============================================================================
// Tests — Entity inheritance
// =============================================================================

TEST_CASE("GenericCarScraper Entity Inheritance", "[genericscraper][entity]") {

    SECTION("Custom prefix is applied") {
        TestScraper scraper("ARGUS-SCRAPER");
        REQUIRE(scraper.getPrefix() == "ARGUS-SCRAPER");
    }

    SECTION("Default TestScraper prefix is TEST-SCRAPER") {
        TestScraper scraper;
        REQUIRE(scraper.getPrefix() == "TEST-SCRAPER");
    }

    SECTION("UUID is generated and non-empty") {
        TestScraper scraper;
        REQUIRE_FALSE(scraper.getUuid().empty());
    }

    SECTION("FullId starts with prefix") {
        TestScraper scraper;
        REQUIRE(scraper.getFullId().rfind("TEST-SCRAPER-", 0) == 0);
    }

    SECTION("Two scrapers have different UUIDs") {
        TestScraper s1, s2;
        REQUIRE(s1.getUuid() != s2.getUuid());
    }

}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("GenericCarScraper Construction", "[genericscraper][construction]") {

    SECTION("Construction succeeds without throwing") {
        REQUIRE_NOTHROW(TestScraper());
    }

    SECTION("Construction with custom prefix succeeds without throwing") {
        REQUIRE_NOTHROW(TestScraper("MY-SCRAPER"));
    }

    SECTION("carBrand is DEFAULT_STR after construction") {
        TestScraper scraper;
        REQUIRE(scraper.getCarBrand() == DEFAULT_STR);
    }

    SECTION("carModel is DEFAULT_STR after construction") {
        TestScraper scraper;
        REQUIRE(scraper.getCarModel() == DEFAULT_STR);
    }

    SECTION("outputDirectory is HTML_DIR after construction") {
        TestScraper scraper;
        REQUIRE(scraper.getOutputDirectory() == HTML_DIR);
    }

}


// =============================================================================
// Tests — setCarBrand
// =============================================================================

TEST_CASE("GenericCarScraper setCarBrand", "[genericscraper][setter][brand]") {

    SECTION("Valid brand is stored in default case") {
        TestScraper scraper;
        scraper.setCarBrand("Renault");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

    SECTION("Brand is normalized to default case") {
        TestScraper scraper;
        scraper.setCarBrand("renault");
        REQUIRE(scraper.getCarBrand() == "renault");
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

    SECTION("Brand with accented characters is NFC normalized and non-empty") {
        TestScraper scraper;
        scraper.setCarBrand("citroën");
        REQUIRE_FALSE(scraper.getCarBrand().empty());
        REQUIRE(scraper.getCarBrand() != DEFAULT_STR);
    }

    SECTION("setCarBrand does not throw") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarBrand("Peugeot"));
    }

    SECTION("setCarBrand can be called multiple times") {
        TestScraper scraper;
        scraper.setCarBrand("Renault");
        scraper.setCarBrand("Peugeot");
        REQUIRE(scraper.getCarBrand() == "peugeot");
    }

}


// =============================================================================
// Tests — setCarBrandCase
// =============================================================================

TEST_CASE("GenericCarScraper setCarBrandCase", "[genericscraper][setter][brandCase]") {

    SECTION("Default case is LOWER") {
        TestScraper scraper;
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

    SECTION("Invalid case fallback to LOWER") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::NA);
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

    SECTION("TITLE case is applied") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::TITLE);
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "Renault");
    }

    SECTION("UPPER case is applied") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::UPPER);
        scraper.setCarBrand("renault");
        REQUIRE(scraper.getCarBrand() == "RENAULT");
    }

    SECTION("LOWER case is applied") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::LOWER);
        scraper.setCarBrand("RENAULT");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

    SECTION("setCarBrandCase does not throw") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarBrandCase(CarScraper::TextCase::LOWER));
    }

    SECTION("setCarBrandCase can be called multiple times") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::UPPER);
        scraper.setCarBrandCase(CarScraper::TextCase::LOWER);
        scraper.setCarBrand("Renault");
        REQUIRE(scraper.getCarBrand() == "renault");
    }

}


// =============================================================================
// Tests — setCarModel
// =============================================================================

TEST_CASE("GenericCarScraper setCarModel", "[genericscraper][setter][model]") {

    SECTION("Valid model is stored in default case") {
        TestScraper scraper;
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("Model is normalized to default case") {
        TestScraper scraper;
        scraper.setCarModel("clio");
        REQUIRE(scraper.getCarModel() == "clio");
        scraper.setCarModel("CLIO");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("setCarModel does not throw") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarModel("308"));
    }

    SECTION("setCarBrand and setCarModel are independent") {
        TestScraper scraper;
        scraper.setCarBrand("Renault");
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarBrand() == "renault");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("setCarModel can be called multiple times") {
        TestScraper scraper;
        scraper.setCarModel("Clio");
        scraper.setCarModel("Megane");
        REQUIRE(scraper.getCarModel() == "megane");
    }

}


// =============================================================================
// Tests — setCarModelCase
// =============================================================================

TEST_CASE("GenericCarScraper setCarModelCase", "[genericscraper][setter][modelCase]") {

    SECTION("Default case is LOWER") {
        TestScraper scraper;
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("Invalid case fallback to LOWER") {
        TestScraper scraper;
        scraper.setCarModelCase(CarScraper::TextCase::NA);
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("TITLE case is applied") {
        TestScraper scraper;
        scraper.setCarModelCase(CarScraper::TextCase::TITLE);
        scraper.setCarModel("cliO");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

    SECTION("UPPER case is applied") {
        TestScraper scraper;
        scraper.setCarModelCase(CarScraper::TextCase::UPPER);
        scraper.setCarModel("clio");
        REQUIRE(scraper.getCarModel() == "CLIO");
    }

    SECTION("LOWER case is applied") {
        TestScraper scraper;
        scraper.setCarModelCase(CarScraper::TextCase::LOWER);
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("setCarModelCase does not throw") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setCarModelCase(CarScraper::TextCase::LOWER));
    }

    SECTION("setCarModelCase can be called multiple times") {
        TestScraper scraper;
        scraper.setCarModelCase(CarScraper::TextCase::UPPER);
        scraper.setCarModelCase(CarScraper::TextCase::LOWER);
        scraper.setCarModel("Clio");
        REQUIRE(scraper.getCarModel() == "clio");
    }

    SECTION("setCarModelCase and setCarBrandCase are independent") {
        TestScraper scraper;
        scraper.setCarBrandCase(CarScraper::TextCase::UPPER);
        scraper.setCarModelCase(CarScraper::TextCase::TITLE);
        scraper.setCarBrand("renault");
        scraper.setCarModel("clIO");
        REQUIRE(scraper.getCarBrand() == "RENAULT");
        REQUIRE(scraper.getCarModel() == "Clio");
    }

}


// =============================================================================
// Tests — setOutputDirectory
// =============================================================================

TEST_CASE("GenericCarScraper setOutputDirectory", "[genericscraper][setter][directory]") {

    SECTION("Valid existing directory is stored") {
        TestScraper scraper;
        scraper.setOutputDirectory("/tmp");
        REQUIRE(scraper.getOutputDirectory() == "/tmp");
    }

    SECTION("Non-existing directory falls back to HTML_DIR") {
        TestScraper scraper;
        scraper.setOutputDirectory("/this/path/does/not/exist/carscraper");
        REQUIRE(scraper.getOutputDirectory() == HTML_DIR);
    }

    SECTION("setOutputDirectory does not throw on invalid path") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setOutputDirectory("/invalid/path"));
    }

    SECTION("setOutputDirectory does not throw on valid path") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.setOutputDirectory("/tmp"));
    }

    SECTION("setOutputDirectory can be overridden with a valid path") {
        TestScraper scraper;
        scraper.setOutputDirectory("/invalid/path");
        scraper.setOutputDirectory("/tmp");
        REQUIRE(scraper.getOutputDirectory() == "/tmp");
    }

}


// =============================================================================
// Tests — scrapModel (base implementation)
// =============================================================================

TEST_CASE("GenericCarScraper scrapModel", "[genericscraper][scrapmodel]") {

    SECTION("Base scrapModel returns IGNORED_ACTION_CODE") {
        TestScraper scraper;
        scraper.setCarBrand("Renault");
        scraper.setCarModel("Clio");
        REQUIRE(scraper.scrapModel() == IGNORED_ACTION_CODE);
    }

    SECTION("Base scrapModel does not throw") {
        TestScraper scraper;
        REQUIRE_NOTHROW(scraper.scrapModel());
    }

    SECTION("Base scrapModel without brand/model still returns IGNORED_ACTION_CODE") {
        TestScraper scraper;
        REQUIRE(scraper.scrapModel() == IGNORED_ACTION_CODE);
    }

}