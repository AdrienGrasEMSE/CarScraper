/**
 * @file baseCaradisiacMapper.cpp
 *
 * @brief Unit tests for CarScraper::CaradisiacMapper class.
 *        Covers the public string helpers (case conversion, prefix/suffix
 *        removal, roman numeral extraction/conversion), titleProcessing(),
 *        and the full mapToCar() pipeline against self-contained
 *        caradisiac.com-shaped HTML fixtures.
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// Imports
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include "core/mapper/CaradisiacMapper.hpp"
#include "core/utils/Constant.hpp"
#include "core/enum/DataSource.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/FuelType.hpp"

using namespace CarScraper;
namespace fs = std::filesystem;


// =============================================================================
// Fixture helpers
// =============================================================================

static const std::string TEST_DIR_CARADISIAC = "data/test_caradisiac_mapper/";

static void cleanDirectoryCaradisiac() {
    fs::remove_all(TEST_DIR_CARADISIAC);
}

static std::string writeFixtureFileCaradisiac(const std::string& name, const std::string& content) {
    fs::create_directories(TEST_DIR_CARADISIAC);
    const std::string path = TEST_DIR_CARADISIAC + name;
    std::ofstream file(path);
    file << content;
    file.close();
    return path;
}

/**
 * @brief Builds a complete, well-formed caradisiac.com technical sheet HTML fixture.
 * @details Mirrors every XPath used by CaradisiacMapper::mapToCar(): breadcrumb,
 *          title (generation/phase/engine embedded), price, trim, and the five
 *          "JScaracCaracN" spec tables.
 * @param stillInSale If true, "Date de fin de commercialisation" is "--".
 */
static std::string buildFullSheetCaradisiac(bool stillInSale) {
    std::string endDateRow = stillInSale
        ? "<tr><td>Date de fin de commercialisation</td><td>--</td></tr>"
        : "<tr><td>Date de fin de commercialisation</td><td>30/06/2019</td></tr>";

    return R"HTML(
<!DOCTYPE html>
<html lang="fr">
<head><meta charset="UTF-8"><title>Fiche technique</title></head>
<body>

    <div class="filAriane line hidden-xs hidden-sm">
        <ol>
            <li><a><span>Accueil</span></a></li>
            <li><a><span>Marques</span></a></li>
            <li><a><span>Renault</span></a></li>
            <li><a><span>Megane</span></a></li>
        </ol>
    </div>

    <h1 class="h1 line margB_L"><span>III (2) 1.5 dCi 110 Business</span></h1>

    <span class="blocPrixVal">21 500 €</span>

    <table>
        <tbody class="JScaracCarac1">
            <tr><td>Finition</td><td>Business</td></tr>
            <tr><td>Date de commercialisation</td><td>01/09/2018</td></tr>
            )HTML" + endDateRow + R"HTML(
        </tbody>
    </table>

    <table>
        <tbody class="JScaracCarac2">
            <tr><td>Hauteur</td><td>1,47</td></tr>
            <tr><td>Largeur sans rétros</td><td>1,80</td></tr>
            <tr><td>Longueur</td><td>4,36</td></tr>
            <tr><td>Poids à vide</td><td>1 350</td></tr>
            <tr><td>Volume de coffre mini/maxi</td><td>434</td></tr>
            <tr><td>Nombre de portes</td><td>5</td></tr>
            <tr><td>Nombre de places assises</td><td>5</td></tr>
        </tbody>
    </table>

    <table>
        <tbody class="JScaracCarac3">
            <tr><td>Boîte de vitesse, Nb de rapports</td><td>Mécanique à 6 rapports</td></tr>
            <tr><td>Motorisation</td><td>Diesel</td></tr>
            <tr><td>Puissance</td><td>110</td></tr>
            <tr><td>Puissance fiscale</td><td>6</td></tr>
        </tbody>
    </table>

    <table>
        <tbody class="JScaracCarac5">
            <tr><td>Volume du réservoir</td><td>47</td></tr>
            <tr><td>Consommation mixte</td><td>3,8</td></tr>
            <tr><td>Emission de CO2 (NEDC)</td><td>99</td></tr>
        </tbody>
    </table>

</body>
</html>
)HTML";
}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("CaradisiacMapper Construction", "[caradisiacmapper][construction]") {

    SECTION("Prefix is CARADISIAC_MAPPER") {
        CaradisiacMapper mapper;
        REQUIRE(mapper.getFullId().rfind("CARADISIAC_MAPPER-", 0) == 0);
    }

    SECTION("Construction does not throw") {
        REQUIRE_NOTHROW(CaradisiacMapper());
    }

    SECTION("Car is not complete before mapping") {
        CaradisiacMapper mapper;
        REQUIRE_FALSE(mapper.getCar().isComplete());
    }
}


// =============================================================================
// Tests — string helpers
// =============================================================================

TEST_CASE("CaradisiacMapper toUpperCase / toLowerCase", "[caradisiacmapper][helper][case]") {
    CaradisiacMapper mapper;

    SECTION("toUpperCase converts ASCII letters") {
        REQUIRE(mapper.toUpperCase("megane iii") == "MEGANE III");
    }

    SECTION("toLowerCase converts ASCII letters") {
        REQUIRE(mapper.toLowerCase("MEGANE III") == "megane iii");
    }

    SECTION("toUpperCase on an already-uppercase string is a no-op") {
        REQUIRE(mapper.toUpperCase("ABC") == "ABC");
    }

    SECTION("toLowerCase on an empty string returns an empty string") {
        REQUIRE(mapper.toLowerCase("").empty());
    }
}


TEST_CASE("CaradisiacMapper removePrefix", "[caradisiacmapper][helper][removePrefix]") {
    CaradisiacMapper mapper;

    SECTION("Removes a matching prefix, case-insensitively") {
        REQUIRE(mapper.removePrefix("MEGANE III Business", "megane") == "III Business");
    }

    SECTION("Trims whitespace after removing the prefix") {
        REQUIRE(mapper.removePrefix("Clio    V", "Clio") == "V");
    }

    SECTION("Returns ERROR_STR when the prefix does not match") {
        REQUIRE(mapper.removePrefix("Megane III", "Clio") == ERROR_STR);
    }

    SECTION("Returns ERROR_STR when prefix is longer than the string") {
        REQUIRE(mapper.removePrefix("Cl", "Clio") == ERROR_STR);
    }
}


TEST_CASE("CaradisiacMapper removeSuffix", "[caradisiacmapper][helper][removeSuffix]") {
    CaradisiacMapper mapper;

    SECTION("Removes a matching suffix, case-insensitively") {
        REQUIRE(mapper.removeSuffix("iii (2) 1.5 dci 110 business", "Business") == "iii (2) 1.5 dci 110");
    }

    SECTION("Trims whitespace before the removed suffix") {
        REQUIRE(mapper.removeSuffix("Clio   V", "V") == "Clio");
    }

    SECTION("Returns ERROR_STR when the suffix does not match") {
        REQUIRE(mapper.removeSuffix("Megane III", "Clio") == ERROR_STR);
    }

    SECTION("Returns ERROR_STR when suffix is longer than the string") {
        REQUIRE(mapper.removeSuffix("V", "Clio V") == ERROR_STR);
    }
}


TEST_CASE("CaradisiacMapper extractRomanNumbers / romanToInt", "[caradisiacmapper][helper][roman]") {
    CaradisiacMapper mapper;

    SECTION("Extracts a simple roman numeral") {
        REQUIRE(mapper.extractRomanNumbers("megane iii business") == "III");
    }

    SECTION("Returns ERROR_STR when no roman numeral is present") {
        REQUIRE(mapper.extractRomanNumbers("megane business") == ERROR_STR);
    }

    SECTION("romanToInt converts additive numerals") {
        REQUIRE(mapper.romanToInt("III") == 3);
        REQUIRE(mapper.romanToInt("VI")  == 6);
    }

    SECTION("romanToInt converts subtractive numerals") {
        REQUIRE(mapper.romanToInt("IV") == 4);
        REQUIRE(mapper.romanToInt("IX") == 9);
    }

    SECTION("romanToInt converts a mixed numeral") {
        REQUIRE(mapper.romanToInt("XIV") == 14);
    }
}


// =============================================================================
// Tests — mapToCar : guard clauses
// =============================================================================

TEST_CASE("CaradisiacMapper mapToCar guard clauses", "[caradisiacmapper][mapToCar][guard]") {

    SECTION("Returns ERROR_CODE when no input file is set") {
        CaradisiacMapper mapper;
        REQUIRE(mapper.mapToCar() == ERROR_CODE);
    }

    SECTION("Does not throw when no input file is set") {
        CaradisiacMapper mapper;
        REQUIRE_NOTHROW(mapper.mapToCar());
    }

    SECTION("Car stays at its default sentinel state when no input file is set") {
        CaradisiacMapper mapper;
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getBrand() == DEFAULT_STR);
    }
}


// =============================================================================
// Tests — mapToCar : full nominal sheet
// =============================================================================

TEST_CASE("CaradisiacMapper mapToCar on a complete sheet", "[caradisiacmapper][mapToCar][nominal]") {

    const std::string path = writeFixtureFileCaradisiac("full.html", buildFullSheetCaradisiac(false));
    CaradisiacMapper mapper;
    REQUIRE(mapper.setInputFile(path));

    SECTION("Returns SUCCESS_CODE") {
        REQUIRE(mapper.mapToCar() == SUCCESS_CODE);
    }

    SECTION("Brand and model are extracted from the breadcrumb") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getBrand() == "RENAULT");
        REQUIRE(mapper.getCar().getModel() == "Megane");
    }

    SECTION("Price is parsed as an int") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getPrice() == 21500);
    }

    SECTION("Trim is read directly from the Finition row") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getTrim() == "Business");
    }

    SECTION("Title processing extracts engine, phase and generation") {
        // Title: "III (2) 1.5 dCi 110 Business" with trim "Business" stripped first.
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getEngine()     == "1.5 dci 110");
        REQUIRE(car.getPhase()      == "2");
        REQUIRE(car.getGeneration() == "3");
    }

    SECTION("Dimensions are parsed as doubles/ints, comma decimals handled") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getHeight() == Approx(1.47));
        REQUIRE(car.getWidth()  == Approx(1.80));
        REQUIRE(car.getLength() == Approx(4.36));
        REQUIRE(car.getWeight() == 1350);
    }

    SECTION("Liveability fields are parsed") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getTrunkVolume() == 434);
        REQUIRE(car.getDoorCount()   == 5);
        REQUIRE(car.getSeatCount()   == 5);
    }

    SECTION("Gearbox type and gear count are parsed from the French label") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getGearboxType() == CarScraper::GearboxType::MANUAL);
        REQUIRE(car.getGearCount()   == 6);
    }

    SECTION("Power figures are parsed") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getFuelType()      == CarScraper::FuelType::GO);
        REQUIRE(car.getHorsePower()    == 110);
        REQUIRE(car.getTaxHorsePower() == 6);
    }

    SECTION("Consumption figures are parsed, comma decimals handled") {
        mapper.mapToCar();
        const auto& car = mapper.getCar();
        REQUIRE(car.getTankCapacity()    == 47);
        REQUIRE(car.getFuelConsumption() == Approx(3.8));
        REQUIRE(car.getCo2Emissions()    == 99);
    }

    SECTION("Commercialisation start date is extracted") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getCommercialisationStart().has_value());
    }

    SECTION("Ended sheet sets stillInSale to false and records the end date") {
        mapper.mapToCar();
        REQUIRE_FALSE(mapper.getCar().isStillInSale());
        REQUIRE(mapper.getCar().getCommercialisationEnd().has_value());
    }

    SECTION("DataSource is set to CARADISIAC") {
        mapper.mapToCar();
        REQUIRE(mapper.getCar().getDataSource() == CarScraper::DataSource::CARADISIAC);
    }

    cleanDirectoryCaradisiac();
}


// =============================================================================
// Tests — mapToCar : still-in-sale sheet
// =============================================================================

TEST_CASE("CaradisiacMapper mapToCar with a still-in-sale sheet", "[caradisiacmapper][mapToCar][stillInSale]") {

    const std::string path = writeFixtureFileCaradisiac("still_in_sale.html", buildFullSheetCaradisiac(true));
    CaradisiacMapper mapper;
    REQUIRE(mapper.setInputFile(path));
    mapper.mapToCar();

    SECTION("stillInSale is true when the end date row is \"--\"") {
        REQUIRE(mapper.getCar().isStillInSale());
    }

    SECTION("No commercialisation end date is recorded") {
        REQUIRE_FALSE(mapper.getCar().getCommercialisationEnd().has_value());
    }

    cleanDirectoryCaradisiac();
}


// =============================================================================
// Tests — mapToCar : missing / malformed fields
// =============================================================================

TEST_CASE("CaradisiacMapper mapToCar with missing fields", "[caradisiacmapper][mapToCar][missing]") {

    SECTION("Missing gearbox row leaves gearbox at its NA default and does not throw") {
        const std::string html = R"HTML(
<!DOCTYPE html>
<html><body>
    <div class="filAriane line hidden-xs hidden-sm">
        <ol>
            <li><a><span>-</span></a></li>
            <li><a><span>-</span></a></li>
            <li><a><span>Dacia</span></a></li>
            <li><a><span>Sandero</span></a></li>
        </ol>
    </div>
    <h1 class="h1 line margB_L"><span>Sandero Access</span></h1>
    <span class="blocPrixVal">9 990 €</span>
    <table><tbody class="JScaracCarac1">
        <tr><td>Finition</td><td>Access</td></tr>
    </tbody></table>
    <table><tbody class="JScaracCarac2"></tbody></table>
    <table><tbody class="JScaracCarac3"></tbody></table>
    <table><tbody class="JScaracCarac5"></tbody></table>
</body></html>
)HTML";
        const std::string path = writeFixtureFileCaradisiac("missing_gearbox.html", html);
        CaradisiacMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        REQUIRE(mapper.mapToCar() == SUCCESS_CODE);

        const auto& car = mapper.getCar();
        REQUIRE(car.getGearboxType() == CarScraper::GearboxType::NA);
        // setGearCount() is only reached inside the regex-match branch, so on a
        // missing/unmatched gearbox row it keeps the Car's own default (unset)
        // sentinel rather than the mapper's ERROR_INT.
        REQUIRE(car.getGearCount()   == DEFAULT_INT);
        REQUIRE(car.getHorsePower()  == ERROR_INT);

        cleanDirectoryCaradisiac();
    }

    SECTION("Title without an engine-displacement marker skips phase/generation extraction") {
        const std::string html = R"HTML(
<!DOCTYPE html>
<html><body>
    <div class="filAriane line hidden-xs hidden-sm">
        <ol><li><a><span>-</span></a></li><li><a><span>-</span></a></li>
            <li><a><span>Dacia</span></a></li><li><a><span>Sandero</span></a></li></ol>
    </div>
    <h1 class="h1 line margB_L"><span>Sandero Access</span></h1>
    <span class="blocPrixVal">9 990 €</span>
    <table><tbody class="JScaracCarac1">
        <tr><td>Finition</td><td>Access</td></tr>
    </tbody></table>
    <table><tbody class="JScaracCarac2"></tbody></table>
    <table><tbody class="JScaracCarac3"></tbody></table>
    <table><tbody class="JScaracCarac5"></tbody></table>
</body></html>
)HTML";
        const std::string path = writeFixtureFileCaradisiac("no_engine_marker.html", html);
        CaradisiacMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        mapper.mapToCar();

        const auto& car = mapper.getCar();
        // No "<n>[.,]<n>" pattern in "sandero access" (after stripping the "access"
        // trim suffix) -> the engine regex never matches -> engine/phase/generation
        // are never set and keep the Car's default sentinels.
        REQUIRE(car.getEngine()     == DEFAULT_STR);
        REQUIRE(car.getPhase()      == DEFAULT_STR);
        REQUIRE(car.getGeneration() == DEFAULT_STR);

        cleanDirectoryCaradisiac();
    }
}
