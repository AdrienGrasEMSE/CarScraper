/**
 * @file Co2Class.cpp
 * 
 * @brief This file implements the enum for the Co2 class, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// Imports
#include "Co2Class.hpp"
#include <uni_algo/case.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @brief Converts a Co2Class enum value to its string representation.
     *
     * @param value The Co2Class value to convert.
     * @return The string representation of the Co2Class value.
     */
    std::string co2ClassToString(Co2Class value) {
        switch (value) {
            case Co2Class::A:   return "A";
            case Co2Class::B:   return "B";
            case Co2Class::C:   return "C";
            case Co2Class::D:   return "D";
            case Co2Class::E:   return "E";
            case Co2Class::F:   return "F";
            case Co2Class::G:   return "G";
            default:            return "NA";
        }
    }


    /**
     * @brief Converts a Co2Class enum value to its signification.
     *
     * @param value The Co2Class value to convert.
     * @return The string signification of the Co2Class value.
     */
    std::string co2ClassExplained(Co2Class value) {
        switch (value) {
            case Co2Class::A:   return "-100g/km";
            case Co2Class::B:   return "101-120g/km";
            case Co2Class::C:   return "121-140g/km";
            case Co2Class::D:   return "141-160g/km";
            case Co2Class::E:   return "161-200g/km";
            case Co2Class::F:   return "201-250g/km";
            case Co2Class::G:   return "+250g/km";
            default:            return "Unknown";
        }
    }


    /**
     * @brief Converts a string to its Co2Class enum value.
     *        Returns Co2Class::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The Co2Class value corresponding to the string.
     */
    Co2Class co2ClassFromString(const std::string& value) {

        // Lowercase noramlisation
        const std::string lower = una::cases::to_lowercase_utf8(value);


        // A
        if (lower == "a") {
            return Co2Class::A;
        }


        // B
        if (lower == "b") {
            return Co2Class::B;
        }


        // C
        if (lower == "c") {
            return Co2Class::C;
        }


        // D
        if (lower == "d") {
            return Co2Class::D;
        }


        // E
        if (lower == "e") {
            return Co2Class::E;
        }


        // F
        if (lower == "f") {
            return Co2Class::F;
        }


        // G
        if (lower == "g") {
            return Co2Class::G;
        }


        // Unknown
        return Co2Class::NA;

    }


    /**
     * @brief Returns whether a Co2Class value is valid (i.e. not NA).
     *
     * @param value The Co2Class value to check.
     * @return True if the value is known, false otherwise.
     */
    bool co2ClassIsValid(Co2Class value) {
        return value != Co2Class::NA;
    }

} // namespace CarScraper