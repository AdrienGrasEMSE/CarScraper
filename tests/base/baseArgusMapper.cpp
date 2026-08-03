/**
 * @file baseArgusMapper.cpp
 *
 * @brief Unit tests for CarScraper::ArgusMapper class.
 *        Tests build minimal but representative largus.fr technical-sheet
 *        HTML fixtures on disk and verify that mapToCar() correctly
 *        populates the underlying Car entity via HtmlParser + XPath.
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// Imports
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include "core/mappers/ArgusMapper.hpp"
#include "core/utils/Constant.hpp"
#include "core/enum/DataSource.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/FuelType.hpp"

using namespace CarScraper;
namespace fs = std::filesystem;


// =============================================================================
// Fixture helpers
// =============================================================================

static const std::string TEST_DIR_ARGUS = "data/test_argus_mapper/";

static void cleanDirectoryArgus() {
    fs::remove_all(TEST_DIR_ARGUS);
}

static std::string writeFixtureFileArgus(const std::string& name, const std::string& content) {
    fs::create_directories(TEST_DIR_ARGUS);
    const std::string path = TEST_DIR_ARGUS + name;
    std::ofstream file(path);
    file << content;
    file.close();
    return path;
}

/**
 * @brief Builds a complete, well-formed largus.fr technical sheet HTML fixture.
 * @details Mirrors every XPath used by ArgusMapper::mapToCar(): breadcrumb (brand/model),
 *          versions-table rows, version title/price header and end-of-sale date.
 * @param stillInSale If true, "Date de fin de commercialisation" is "En cours".
 */
static std::string buildFullSheetArgus(bool stillInSale) {
    std::string endDateRow = stillInSale
        ? "<tr><td>Date de fin de commercialisation</td><td>En cours</td></tr>"
        : "<tr><td>Date de fin de commercialisation</td><td>30/06/2019</td></tr>";

    return R"HTML(
<!DOCTYPE html>
<html lang="fr">
<head><meta charset="UTF-8"><title>Fiche technique</title></head>
<body>

    <div class="breadCrumbList">
        <ul>
            <li><a href="/">Accueil</a></li>
            <li><a href="/Toutes-Marques.html">Toutes les marques</a></li>
            <li><a href="/Peugeot.html">Peugeot</a></li>
            <li><a href="/fiche-technique/Peugeot/308.html">308</a></li>
        </ul>
    </div>

    <h1 class="ft-version-title">308 1.5 BlueHDi 130ch Allure Business</h1>

    <div class="ft-version-price">
        01/09/2018
        <strong>25 000 €</strong>
    </div>

    <table class="versions-table">
        <tbody>
            <tr><td>Nom du moteur</td><td>1.5 BlueHDi 130</td></tr>
            <tr><td>Hauteur</td><td>1,47</td></tr>
            <tr><td>Largeur</td><td>1,80</td></tr>
            <tr><td>Longueur</td><td>4,25</td></tr>
            <tr><td>Volume de coffre</td><td>420</td></tr>
            <tr><td>Poids à vide</td><td>1 205</td></tr>
            <tr><td>Nombre de places</td><td>5</td></tr>
            <tr><td>Boîte de vitesses</td><td>Manuelle</td></tr>
            <tr><td>Énergie</td><td>Diesel</td></tr>
            <tr><td>Puissance réelle maxi</td><td>130</td></tr>
            <tr><td>Puissance fiscale</td><td>6</td></tr>
            <tr><td>Réservoir</td><td>53</td></tr>
            <tr><td>Mixte</td><td>3,6</td></tr>
            <tr><td>Émission de CO2</td><td>95</td></tr>
            )HTML" + endDateRow + R"HTML(
        </tbody>
    </table>

</body>
</html>
)HTML";
}


// =============================================================================
// Tests — Entity inheritance / construction
// =============================================================================

TEST_CASE("ArgusMapper Construction", "[argusmapper][construction]") {

    SECTION("Prefix is ARGUS_MAPPER") {
        ArgusMapper mapper;
        REQUIRE(mapper.getFullId().rfind("ARGUS_MAPPER-", 0) == 0);
    }

    SECTION("Construction does not throw") {
        REQUIRE_NOTHROW(ArgusMapper());
    }

    SECTION("Car is not complete before mapping") {
        ArgusMapper mapper;
        REQUIRE_FALSE(mapper.getCar().isComplete());
    }
}


// =============================================================================
// Tests — mapToCar : guard clauses
// =============================================================================

TEST_CASE("ArgusMapper mapToCar guard clauses", "[argusmapper][mapToCar][guard]") {

    SECTION("Returns ERROR_CODE when no input file is set") {
        ArgusMapper mapper;
        REQUIRE(mapper.mapToCar() == ERROR_CODE);
    }

    SECTION("Does not throw when no input file is set") {
        ArgusMapper mapper;
        REQUIRE_NOTHROW(mapper.mapToCar());
    }

    SECTION("Car remains at its default (unmapped) state when no input file is set") {
        ArgusMapper mapper;
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getBrand() == DEFAULT_STR);
    }
}


// =============================================================================
// Tests — mapToCar : full nominal sheet
// =============================================================================

TEST_CASE("ArgusMapper mapToCar on a complete sheet", "[argusmapper][mapToCar][nominal]") {

    const std::string path = writeFixtureFileArgus("full.html", buildFullSheetArgus(false));
    ArgusMapper mapper;
    REQUIRE(mapper.setInputFile(path));

    SECTION("Returns SUCCESS_CODE") {
        REQUIRE(mapper.mapToCar() == SUCCESS_CODE);
    }

    SECTION("Brand and model are extracted from the breadcrumb") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getBrand() == "PEUGEOT");
        REQUIRE(mapper.getCar().getModel() == "308");
    }

    SECTION("Engine is extracted from the versions table") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getEngine() == "1.5 BlueHDi 130");
    }

    SECTION("Price is parsed as an int from the strong tag") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getPrice() == 25000);
    }

    SECTION("Trim is extracted from the title after the horsepower marker") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getTrim() == "Allure Business");
    }

    SECTION("Dimensions are parsed as doubles/ints") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getHeight()      == Approx(1.47));
        REQUIRE(car.getWidth()       == Approx(1.80));
        REQUIRE(car.getLength()      == Approx(4.25));
        REQUIRE(car.getTrunkVolume() == 420);
        REQUIRE(car.getWeight()      == 1205);
        REQUIRE(car.getSeatCount()   == 5);
    }

    SECTION("Gearbox type is parsed from the French label") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getGearboxType() == CarScraper::GearboxType::MANUAL);
    }

    SECTION("Power figures are parsed") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getFuelType()     == CarScraper::FuelType::GO);
        REQUIRE(car.getHorsePower()   == 130);
        REQUIRE(car.getTaxHorsePower()== 6);
    }

    SECTION("Consumption figures are parsed, comma decimals handled") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getTankCapacity()    == 53);
        REQUIRE(car.getFuelConsumption() == Approx(3.6));
        REQUIRE(car.getCo2Emissions()    == 95);
    }

    SECTION("Commercialisation start date is extracted from the price label") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getCommercialisationStart().has_value());
    }

    SECTION("Ended sheet sets stillInSale to false and records the end date") {
        mapper.mapToCar();
        REQUIRE_FALSE(mapper.getCar().isStillInSale());
        REQUIRE(mapper.getCar().getCommercialisationEnd().has_value());
    }

    SECTION("DataSource is set to ARGUS") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getDataSource() == CarScraper::DataSource::ARGUS);
    }

    cleanDirectoryArgus();
}


// =============================================================================
// Tests — mapToCar : still-in-sale sheet
// =============================================================================

TEST_CASE("ArgusMapper mapToCar with a still-in-sale sheet", "[argusmapper][mapToCar][stillInSale]") {

    const std::string path = writeFixtureFileArgus("still_in_sale.html", buildFullSheetArgus(true));
    ArgusMapper mapper;
    REQUIRE(mapper.setInputFile(path));
    mapper.mapToCar();

    SECTION("stillInSale is true") {
        REQUIRE(mapper.getCar().isStillInSale());
    }

    SECTION("No commercialisation end date is recorded") {
        REQUIRE_FALSE(mapper.getCar().getCommercialisationEnd().has_value());
    }

    cleanDirectoryArgus();
}


// =============================================================================
// Tests — mapToCar : missing / malformed fields
// =============================================================================

TEST_CASE("ArgusMapper mapToCar with missing fields", "[argusmapper][mapToCar][missing]") {

    SECTION("Missing versions-table rows fall back to ERROR sentinels") {
        const std::string html = R"HTML(
<!DOCTYPE html>
<html><body>
    <div class="breadCrumbList">
        <ul>
            <li><a href="/">Accueil</a></li>
            <li><a href="/x.html">Marques</a></li>
            <li><a href="/Renault.html">Renault</a></li>
            <li><a href="/fiche-technique/Renault/Clio.html">Clio</a></li>
        </ul>
    </div>
    <h1 class="ft-version-title">Clio sans puissance connue</h1>
    <div class="ft-version-price"><strong>12 000 €</strong></div>
    <table class="versions-table"><tbody></tbody></table>
</body></html>
)HTML";
        const std::string path = writeFixtureFileArgus("missing.html", html);
        ArgusMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        REQUIRE(mapper.mapToCar() == SUCCESS_CODE);

        const auto& car = mapper.getCar();
        REQUIRE(car.getEngine()      == ERROR_STR);
        REQUIRE(car.getHorsePower()  == ERROR_INT);
        REQUIRE(car.getFuelConsumption() == Approx(ERROR_DOUBLE));

        cleanDirectoryArgus();
    }

    SECTION("Title without a horsepower marker leaves trim unset") {
        const std::string html = R"HTML(
<!DOCTYPE html>
<html><body>
    <div class="breadCrumbList">
        <ul><li><a>-</a></li><li><a>-</a></li><li><a>Renault</a></li><li><a>Clio</a></li></ul>
    </div>
    <h1 class="ft-version-title">Clio sans mention de puissance</h1>
    <div class="ft-version-price"><strong>12 000 €</strong></div>
    <table class="versions-table"><tbody></tbody></table>
</body></html>
)HTML";
        const std::string path = writeFixtureFileArgus("no_trim.html", html);
        ArgusMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        mapper.mapToCar();
        // No "<n>ch"/"<n>cv" marker in the title -> the trim-extraction regex never
        // matches -> setTrim() is never called -> the Car keeps its default sentinel.
        REQUIRE(mapper.getCar().getTrim() == DEFAULT_STR);

        cleanDirectoryArgus();
    }
}
