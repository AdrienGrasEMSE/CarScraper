/**
 * @file baseEnum.cpp
 *
 * @brief Unit tests for CarScraper enums (Co2Class, DataSource, FuelType, GearboxType).
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include <catch2/catch.hpp>
#include "core/enum/Co2Class.hpp"
#include "core/enum/DataSource.hpp"
#include "core/enum/FuelType.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/TextCase.hpp"


/**
 * Using namespace CarScraper for enum values
 */
using namespace CarScraper;


// =============================================================================
// Tests — Co2Class
// =============================================================================

TEST_CASE("co2ClassToString", "[enum][co2class]") {

    SECTION("Converts each known value to its letter") {
        REQUIRE(co2ClassToString(Co2Class::A) == "A");
        REQUIRE(co2ClassToString(Co2Class::B) == "B");
        REQUIRE(co2ClassToString(Co2Class::C) == "C");
        REQUIRE(co2ClassToString(Co2Class::D) == "D");
        REQUIRE(co2ClassToString(Co2Class::E) == "E");
        REQUIRE(co2ClassToString(Co2Class::F) == "F");
        REQUIRE(co2ClassToString(Co2Class::G) == "G");
    }

    SECTION("Returns NA for the unknown value") {
        REQUIRE(co2ClassToString(Co2Class::NA) == "NA");
    }
}


TEST_CASE("co2ClassExplained", "[enum][co2class]") {

    SECTION("Converts each known value to its emission range") {
        REQUIRE(co2ClassExplained(Co2Class::A) == "-100g/km");
        REQUIRE(co2ClassExplained(Co2Class::B) == "101-120g/km");
        REQUIRE(co2ClassExplained(Co2Class::C) == "121-140g/km");
        REQUIRE(co2ClassExplained(Co2Class::D) == "141-160g/km");
        REQUIRE(co2ClassExplained(Co2Class::E) == "161-200g/km");
        REQUIRE(co2ClassExplained(Co2Class::F) == "201-250g/km");
        REQUIRE(co2ClassExplained(Co2Class::G) == "+250g/km");
    }

    SECTION("Returns Unknown for the unknown value") {
        REQUIRE(co2ClassExplained(Co2Class::NA) == "Unknown");
    }
}


TEST_CASE("co2ClassFromString", "[enum][co2class]") {

    SECTION("Parses each known letter") {
        REQUIRE(co2ClassFromString("A") == Co2Class::A);
        REQUIRE(co2ClassFromString("B") == Co2Class::B);
        REQUIRE(co2ClassFromString("C") == Co2Class::C);
        REQUIRE(co2ClassFromString("D") == Co2Class::D);
        REQUIRE(co2ClassFromString("E") == Co2Class::E);
        REQUIRE(co2ClassFromString("F") == Co2Class::F);
        REQUIRE(co2ClassFromString("G") == Co2Class::G);
    }

    SECTION("Is case-insensitive") {
        REQUIRE(co2ClassFromString("a") == Co2Class::A);
        REQUIRE(co2ClassFromString("g") == Co2Class::G);
    }

    SECTION("Returns NA for an unrecognized value") {
        REQUIRE(co2ClassFromString("H") == Co2Class::NA);
        REQUIRE(co2ClassFromString("") == Co2Class::NA);
        REQUIRE(co2ClassFromString("AA") == Co2Class::NA);
    }
}


TEST_CASE("co2ClassIsValid", "[enum][co2class]") {

    SECTION("Returns true for known values") {
        REQUIRE(co2ClassIsValid(Co2Class::A));
        REQUIRE(co2ClassIsValid(Co2Class::G));
    }

    SECTION("Returns false for NA") {
        REQUIRE_FALSE(co2ClassIsValid(Co2Class::NA));
    }
}


// =============================================================================
// Tests — DataSource
// =============================================================================

TEST_CASE("dataSourceToString", "[enum][datasource]") {

    SECTION("Converts each known value to its label") {
        REQUIRE(dataSourceToString(DataSource::ARGUS)      == "Argus");
        REQUIRE(dataSourceToString(DataSource::CARADISIAC) == "Caradisiac");
    }

    SECTION("Returns Unknown for the unknown value") {
        REQUIRE(dataSourceToString(DataSource::NA) == "Unknown");
    }
}


TEST_CASE("dataSourceFromString", "[enum][datasource]") {

    SECTION("Parses each known label") {
        REQUIRE(dataSourceFromString("Argus")      == DataSource::ARGUS);
        REQUIRE(dataSourceFromString("Caradisiac") == DataSource::CARADISIAC);
    }

    SECTION("Is case-insensitive") {
        REQUIRE(dataSourceFromString("argus")      == DataSource::ARGUS);
        REQUIRE(dataSourceFromString("CARADISIAC") == DataSource::CARADISIAC);
    }

    SECTION("Returns NA for an unrecognized value") {
        REQUIRE(dataSourceFromString("LaCentrale") == DataSource::NA);
        REQUIRE(dataSourceFromString("") == DataSource::NA);
    }
}


TEST_CASE("dataSourceIsValid", "[enum][datasource]") {

    SECTION("Returns true for known values") {
        REQUIRE(dataSourceIsValid(DataSource::ARGUS));
        REQUIRE(dataSourceIsValid(DataSource::CARADISIAC));
    }

    SECTION("Returns false for NA") {
        REQUIRE_FALSE(dataSourceIsValid(DataSource::NA));
    }
}


// =============================================================================
// Tests — FuelType
// =============================================================================

TEST_CASE("fuelTypeToString", "[enum][fueltype]") {

    SECTION("Converts known SIV codes to themselves") {
        REQUIRE(fuelTypeToString(FuelType::ES) == "ES");
        REQUIRE(fuelTypeToString(FuelType::GO) == "GO");
        REQUIRE(fuelTypeToString(FuelType::EL) == "EL");
        REQUIRE(fuelTypeToString(FuelType::GP) == "GP");
        REQUIRE(fuelTypeToString(FuelType::H2) == "H2");
        REQUIRE(fuelTypeToString(FuelType::GL) == "GL");
        REQUIRE(fuelTypeToString(FuelType::EE) == "EE");
    }

    SECTION("Returns NA for the unknown value") {
        REQUIRE(fuelTypeToString(FuelType::NA) == "NA");
    }
}


TEST_CASE("fuelTypeExplained", "[enum][fueltype]") {

    SECTION("Converts values used on L'Argus to their signification") {
        REQUIRE(fuelTypeExplained(FuelType::ES) == "Essence");
        REQUIRE(fuelTypeExplained(FuelType::GO) == "Diesel");
        REQUIRE(fuelTypeExplained(FuelType::EL) == "Electricité");
        REQUIRE(fuelTypeExplained(FuelType::GP) == "GPL");
        REQUIRE(fuelTypeExplained(FuelType::H2) == "Hydrogène");
        REQUIRE(fuelTypeExplained(FuelType::GL) == "Gazole-électricité (hybride rechargeable)");
        REQUIRE(fuelTypeExplained(FuelType::EE) == "Essence électricité (hybride rechargeable)");
    }

    SECTION("Returns an 'Unused' placeholder for codes not seen on L'Argus") {
        REQUIRE(fuelTypeExplained(FuelType::AC) == "Unused - AC");
        REQUIRE(fuelTypeExplained(FuelType::GA) == "Unused - GA");
    }

    SECTION("Returns Unknown for the unknown value") {
        REQUIRE(fuelTypeExplained(FuelType::NA) == "Unknown");
    }
}


TEST_CASE("fuelTypeFromString", "[enum][fueltype]") {

    SECTION("Parses each L'Argus label to its SIV code") {
        REQUIRE(fuelTypeFromString("Essence")                  == FuelType::ES);
        REQUIRE(fuelTypeFromString("Diesel")                   == FuelType::GO);
        REQUIRE(fuelTypeFromString("Electrique")                == FuelType::EL);
        REQUIRE(fuelTypeFromString("GPL")                       == FuelType::GP);
        REQUIRE(fuelTypeFromString("Diesel/Micro-Hybride")      == FuelType::GL);
        REQUIRE(fuelTypeFromString("Hybride : Essence/Electrique") == FuelType::EE);
    }

    SECTION("Accepts both accented and non-accented spelling for Hydrogène") {
        REQUIRE(fuelTypeFromString("Hydrogène") == FuelType::H2);
        REQUIRE(fuelTypeFromString("Hydrogene") == FuelType::H2);
    }

    SECTION("Is case-insensitive") {
        REQUIRE(fuelTypeFromString("essence") == FuelType::ES);
        REQUIRE(fuelTypeFromString("DIESEL")  == FuelType::GO);
    }

    SECTION("Returns NA for an unrecognized value") {
        REQUIRE(fuelTypeFromString("Ethanol") == FuelType::NA);
        REQUIRE(fuelTypeFromString("") == FuelType::NA);
    }
}


TEST_CASE("fuelTypeIsValid", "[enum][fueltype]") {

    SECTION("Returns true for known values") {
        REQUIRE(fuelTypeIsValid(FuelType::ES));
        REQUIRE(fuelTypeIsValid(FuelType::GO));
    }

    SECTION("Returns false for NA") {
        REQUIRE_FALSE(fuelTypeIsValid(FuelType::NA));
    }
}


// =============================================================================
// Tests — GearboxType
// =============================================================================

TEST_CASE("gearBoxTypeToString", "[enum][gearboxtype]") {

    SECTION("Converts each known value to its French label") {
        REQUIRE(gearBoxTypeToString(GearboxType::AUTOMATIC) == "Automatique");
        REQUIRE(gearBoxTypeToString(GearboxType::MANUAL)    == "Manuelle");
    }

    SECTION("Returns Unknown for the unknown value") {
        REQUIRE(gearBoxTypeToString(GearboxType::NA) == "Unknown");
    }
}


TEST_CASE("gearBoxTypeFromString", "[enum][gearboxtype]") {

    SECTION("Parses each known label") {
        REQUIRE(gearBoxTypeFromString("Automatique") == GearboxType::AUTOMATIC);
        REQUIRE(gearBoxTypeFromString("Manuelle")    == GearboxType::MANUAL);
    }

    SECTION("Is case-insensitive") {
        REQUIRE(gearBoxTypeFromString("automatique") == GearboxType::AUTOMATIC);
        REQUIRE(gearBoxTypeFromString("MANUELLE")    == GearboxType::MANUAL);
    }

    SECTION("Returns NA for an unrecognized value") {
        REQUIRE(gearBoxTypeFromString("Semi-automatique") == GearboxType::NA);
        REQUIRE(gearBoxTypeFromString("") == GearboxType::NA);
    }
}


TEST_CASE("gearBoxTypeIsValid", "[enum][gearboxtype]") {

    SECTION("Returns true for known values") {
        REQUIRE(gearBoxTypeIsValid(GearboxType::AUTOMATIC));
        REQUIRE(gearBoxTypeIsValid(GearboxType::MANUAL));
    }

    SECTION("Returns false for NA") {
        REQUIRE_FALSE(gearBoxTypeIsValid(GearboxType::NA));
    }
}


// =============================================================================
// Tests — TextCase
// =============================================================================

TEST_CASE("textCaseToString", "[enum][textcase]") {

    SECTION("Converts each known value to its label") {
        REQUIRE(textCaseToString(TextCase::LOWER) == "Lowercase");
        REQUIRE(textCaseToString(TextCase::UPPER) == "Uppercase");
        REQUIRE(textCaseToString(TextCase::TITLE) == "Title case");
    }

    SECTION("Returns Unknown for the unknown value") {
        REQUIRE(textCaseToString(TextCase::NA) == "Unknown");
    }
}


TEST_CASE("textCaseFromString", "[enum][textcase]") {

    SECTION("Parses each known label") {
        REQUIRE(textCaseFromString("Lowercase") == TextCase::LOWER);
        REQUIRE(textCaseFromString("Uppercase") == TextCase::UPPER);
        REQUIRE(textCaseFromString("Title case") == TextCase::TITLE);
    }

    SECTION("Is case-insensitive") {
        REQUIRE(textCaseFromString("lowercase") == TextCase::LOWER);
        REQUIRE(textCaseFromString("UPPERCASE") == TextCase::UPPER);
        REQUIRE(textCaseFromString("title case") == TextCase::TITLE);
    }

    SECTION("Returns NA for an unrecognized value") {
        REQUIRE(textCaseFromString("Invalid") == TextCase::NA);
        REQUIRE(textCaseFromString("") == TextCase::NA);
    }
}


TEST_CASE("textCaseIsValid", "[enum][textcase]") {

    SECTION("Returns true for known values") {
        REQUIRE(textCaseIsValid(TextCase::LOWER));
        REQUIRE(textCaseIsValid(TextCase::UPPER));
        REQUIRE(textCaseIsValid(TextCase::TITLE));
    }

    SECTION("Returns false for NA") {
        REQUIRE_FALSE(textCaseIsValid(TextCase::NA));
    }
}