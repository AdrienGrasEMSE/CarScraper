/**
 * @file baseValidation.cpp
 *
 * @brief Unit tests for CarScraper::Validation functions.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Imports
#include <catch2/catch.hpp>
#include "core/utils/Validation.hpp"
#include "core/utils/Constant.hpp"


/**
 * Using namespace CarScraper for constants values and logging
 */
using namespace CarScraper;
using namespace CarScraper::Validation;


// =============================================================================
// Tests — stringValidation
// =============================================================================

TEST_CASE("stringValidation", "[validation][string]") {

    SECTION("Returns the value when valid") {
        REQUIRE(stringValidation("Toyota", "CarID", "Brand") == "Toyota");
    }

    SECTION("Returns ERROR_STR when empty") {
        REQUIRE(stringValidation("", "CarID", "Brand") == ERROR_STR);
    }

    SECTION("Returns ERROR_STR when exceeds default max length (255)") {
        std::string longStr(256, 'a');
        REQUIRE(stringValidation(longStr, "CarID", "Brand") == ERROR_STR);
    }

    SECTION("Returns the value when exactly at max length") {
        std::string exactStr(255, 'a');
        REQUIRE(stringValidation(exactStr, "CarID", "Brand") == exactStr);
    }

    SECTION("Returns ERROR_STR when exceeds custom max length") {
        REQUIRE(stringValidation("TooLongName", "CarID", "Brand", 5) == ERROR_STR);
    }

    SECTION("Returns the value when within custom max length") {
        REQUIRE(stringValidation("Ford", "CarID", "Brand", 10) == "Ford");
    }

    SECTION("Returns ERROR_STR when exactly one char over custom max length") {
        REQUIRE(stringValidation("abcdef", "CarID", "Brand", 5) == ERROR_STR);
    }
}


// =============================================================================
// Tests — intValidation
// =============================================================================

TEST_CASE("intValidation", "[validation][int]") {

    SECTION("Returns the value when within bounds") {
        REQUIRE(intValidation(100, "CarID", "Power", 0, 9999) == 100);
    }

    SECTION("Returns ERROR_INT when below min") {
        REQUIRE(intValidation(-1, "CarID", "Power", 0, 9999) == ERROR_INT);
    }

    SECTION("Returns ERROR_INT when above max") {
        REQUIRE(intValidation(10000, "CarID", "Power", 0, 9999) == ERROR_INT);
    }

    SECTION("Returns the value when exactly at min") {
        REQUIRE(intValidation(0, "CarID", "Power", 0, 9999) == 0);
    }

    SECTION("Returns the value when exactly at max") {
        REQUIRE(intValidation(9999, "CarID", "Power", 0, 9999) == 9999);
    }

    SECTION("Returns ERROR_INT when min equals max and value differs") {
        REQUIRE(intValidation(5, "CarID", "Doors", 4, 4) == ERROR_INT);
    }

    SECTION("Returns the value when min equals max and value matches") {
        REQUIRE(intValidation(4, "CarID", "Doors", 4, 4) == 4);
    }

    SECTION("Default bounds accept DEFAULT_INT") {
        REQUIRE(intValidation(DEFAULT_INT, "CarID", "Power") == DEFAULT_INT);
    }
}


// =============================================================================
// Tests — doubleValidation
// =============================================================================

TEST_CASE("doubleValidation", "[validation][double]") {

    SECTION("Returns the value when within bounds") {
        REQUIRE(doubleValidation(7.5, "CarID", "Consumption", 0.0, 30.0) == 7.5);
    }

    SECTION("Returns ERROR_DOUBLE when below min") {
        REQUIRE(doubleValidation(-0.1, "CarID", "Consumption", 0.0, 30.0) == ERROR_DOUBLE);
    }

    SECTION("Returns ERROR_DOUBLE when above max") {
        REQUIRE(doubleValidation(30.1, "CarID", "Consumption", 0.0, 30.0) == ERROR_DOUBLE);
    }

    SECTION("Returns the value when exactly at min") {
        REQUIRE(doubleValidation(0.0, "CarID", "Consumption", 0.0, 30.0) == 0.0);
    }

    SECTION("Returns the value when exactly at max") {
        REQUIRE(doubleValidation(30.0, "CarID", "Consumption", 0.0, 30.0) == 30.0);
    }

    SECTION("Default bounds accept DEFAULT_DOUBLE") {
        REQUIRE(doubleValidation(DEFAULT_DOUBLE, "CarID", "Consumption") == DEFAULT_DOUBLE);
    }
}


// =============================================================================
// Tests — parseDateDMY  (DD/MM/YYYY)
// =============================================================================

TEST_CASE("parseDateDMY", "[validation][date][DMY]") {

    SECTION("Parses a valid date") {
        auto result = parseDateDMY("15/03/2012");
        REQUIRE(result.has_value());
        REQUIRE(static_cast<int>(result->year())            == 2012);
        REQUIRE(static_cast<unsigned>(result->month())      == 3);
        REQUIRE(static_cast<unsigned>(result->day())        == 15);
    }

    SECTION("Returns nullopt for empty string") {
        REQUIRE_FALSE(parseDateDMY("").has_value());
    }

    SECTION("Returns nullopt for wrong format (YYYY/MM/DD)") {
        REQUIRE_FALSE(parseDateDMY("2012/03/15").has_value());
    }

    SECTION("Returns nullopt for impossible date (31/02/2012)") {
        REQUIRE_FALSE(parseDateDMY("31/02/2012").has_value());
    }

    SECTION("Returns nullopt for month out of range (00)") {
        REQUIRE_FALSE(parseDateDMY("15/00/2012").has_value());
    }

    SECTION("Returns nullopt for month out of range (13)") {
        REQUIRE_FALSE(parseDateDMY("15/13/2012").has_value());
    }

    SECTION("Returns nullopt for non-numeric input") {
        REQUIRE_FALSE(parseDateDMY("ab/cd/efgh").has_value());
    }

    SECTION("Returns nullopt for missing separator") {
        REQUIRE_FALSE(parseDateDMY("15032012").has_value());
    }

    SECTION("Parses end-of-month boundary correctly (30/04/2012)") {
        auto result = parseDateDMY("30/04/2012");
        REQUIRE(result.has_value());
    }

    SECTION("Returns nullopt for (31/04/2012) — April has 30 days") {
        REQUIRE_FALSE(parseDateDMY("31/04/2012").has_value());
    }
}


// =============================================================================
// Tests — parseDateYMD  (YYYY/MM/DD)
// =============================================================================

TEST_CASE("parseDateYMD", "[validation][date][YMD]") {

    SECTION("Parses a valid date") {
        auto result = parseDateYMD("2012/03/15");
        REQUIRE(result.has_value());
        REQUIRE(static_cast<int>(result->year())        == 2012);
        REQUIRE(static_cast<unsigned>(result->month())  == 3);
        REQUIRE(static_cast<unsigned>(result->day())    == 15);
    }

    SECTION("Returns nullopt for wrong format (DD/MM/YYYY)") {
        REQUIRE_FALSE(parseDateYMD("15/03/2012").has_value());
    }

    SECTION("Returns nullopt for impossible date") {
        REQUIRE_FALSE(parseDateYMD("2012/02/31").has_value());
    }

    SECTION("Returns nullopt for non-numeric input") {
        REQUIRE_FALSE(parseDateYMD("YYYY/MM/DD").has_value());
    }
}


// =============================================================================
// Tests — parseDateMDY  (MM/DD/YYYY)
// =============================================================================

TEST_CASE("parseDateMDY", "[validation][date][MDY]") {

    SECTION("Parses a valid date") {
        auto result = parseDateMDY("03/15/2012");
        REQUIRE(result.has_value());
        REQUIRE(static_cast<int>(result->year())        == 2012);
        REQUIRE(static_cast<unsigned>(result->month())  == 3);
        REQUIRE(static_cast<unsigned>(result->day())    == 15);
    }

    SECTION("Returns nullopt for wrong format (DD/MM/YYYY)") {
        REQUIRE_FALSE(parseDateMDY("15/03/2012").has_value());
    }

    SECTION("Returns nullopt for impossible date") {
        REQUIRE_FALSE(parseDateMDY("02/31/2012").has_value());
    }
}


// =============================================================================
// Tests — formatDate
// =============================================================================

TEST_CASE("formatDate", "[validation][date][format]") {

    SECTION("Formats a date correctly") {
        std::chrono::year_month_day date{
            std::chrono::year{2012},
            std::chrono::month{3},
            std::chrono::day{15}
        };
        REQUIRE(formatDate(date) == "15/03/2012");
    }

    SECTION("Zero-pads day and month correctly") {
        std::chrono::year_month_day date{
            std::chrono::year{2005},
            std::chrono::month{1},
            std::chrono::day{7}
        };
        REQUIRE(formatDate(date) == "07/01/2005");
    }

    SECTION("Round-trip: parse then format returns original string") {
        const std::string original = "15/03/2012";
        auto parsed = parseDateDMY(original);
        REQUIRE(parsed.has_value());
        REQUIRE(formatDate(parsed.value()) == original);
    }

    SECTION("Round-trip: all three parsers produce the same date") {
        auto dmy = parseDateDMY("15/03/2012");
        auto ymd = parseDateYMD("2012/03/15");
        auto mdy = parseDateMDY("03/15/2012");

        REQUIRE(dmy.has_value());
        REQUIRE(ymd.has_value());
        REQUIRE(mdy.has_value());
        REQUIRE(dmy.value() == ymd.value());
        REQUIRE(dmy.value() == mdy.value());
    }

}


// =============================================================================
// Tests — string helpers
// =============================================================================

TEST_CASE("toUpperCase / toLowerCase", "[caradisiacmapper][helper][case]") {

    SECTION("toUpperCase converts ASCII letters") {
        REQUIRE(toUpperCase("megane iii") == "MEGANE III");
    }

    SECTION("toLowerCase converts ASCII letters") {
        REQUIRE(toLowerCase("MEGANE III") == "megane iii");
    }

    SECTION("toUpperCase on an already-uppercase string is a no-op") {
        REQUIRE(toUpperCase("ABC") == "ABC");
    }

    SECTION("toLowerCase on an empty string returns an empty string") {
        REQUIRE(toLowerCase("").empty());
    }
}


TEST_CASE("CaradisiacMapper removePrefix", "[caradisiacmapper][helper][removePrefix]") {

    SECTION("Removes a matching prefix, case-insensitively") {
        REQUIRE(removePrefix("MEGANE III Business", "megane") == "III Business");
    }

    SECTION("Trims whitespace after removing the prefix") {
        REQUIRE(removePrefix("Clio    V", "Clio") == "V");
    }

    SECTION("Returns ERROR_STR when the prefix does not match") {
        REQUIRE(removePrefix("Megane III", "Clio") == ERROR_STR);
    }

    SECTION("Returns ERROR_STR when prefix is longer than the string") {
        REQUIRE(removePrefix("Cl", "Clio") == ERROR_STR);
    }
}


TEST_CASE("CaradisiacMapper removeSuffix", "[caradisiacmapper][helper][removeSuffix]") {

    SECTION("Removes a matching suffix, case-insensitively") {
        REQUIRE(removeSuffix("iii (2) 1.5 dci 110 business", "Business") == "iii (2) 1.5 dci 110");
    }

    SECTION("Trims whitespace before the removed suffix") {
        REQUIRE(removeSuffix("Clio   V", "V") == "Clio");
    }

    SECTION("Returns ERROR_STR when the suffix does not match") {
        REQUIRE(removeSuffix("Megane III", "Clio") == ERROR_STR);
    }

    SECTION("Returns ERROR_STR when suffix is longer than the string") {
        REQUIRE(removeSuffix("V", "Clio V") == ERROR_STR);
    }
}


TEST_CASE("CaradisiacMapper extractRomanNumbers / romanToInt", "[caradisiacmapper][helper][roman]") {

    SECTION("Extracts a simple roman numeral") {
        REQUIRE(extractRomanNumbers("megane iii business") == "III");
    }

    SECTION("Returns ERROR_STR when no roman numeral is present") {
        REQUIRE(extractRomanNumbers("megane business") == ERROR_STR);
    }

    SECTION("romanToInt converts additive numerals") {
        REQUIRE(romanToInt("III") == 3);
        REQUIRE(romanToInt("VI")  == 6);
    }

    SECTION("romanToInt converts subtractive numerals") {
        REQUIRE(romanToInt("IV") == 4);
        REQUIRE(romanToInt("IX") == 9);
    }

    SECTION("romanToInt converts a mixed numeral") {
        REQUIRE(romanToInt("XIV") == 14);
    }
}