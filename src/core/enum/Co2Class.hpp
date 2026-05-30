/**
 * @file Co2Class.hpp
 * 
 * @brief This file defines the enum for the Co2 class, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// For only one include
#pragma once


// Imports
#include <string>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Enum
    // =========================================================================

    enum class Co2Class {
        A,  // -100g/km
        B,  // 101-120g/km
        C,  // 121-140g/km
        D,  // 141-160g/km
        E,  // 161-200g/km
        F,  // 201-250g/km
        G,  // +250g/km
        NA  // Unknown
    };


    // =========================================================================
    // Conversion functions
    // =========================================================================

    /**
     * @brief Converts a Co2Class enum value to its string representation.
     *
     * @param value The Co2Class value to convert.
     * @return The string representation of the Co2Class value.
     */
    std::string co2ClassToString(Co2Class value);


    /**
     * @brief Converts a Co2Class enum value to its signification.
     *
     * @param value The Co2Class value to convert.
     * @return The string signification of the Co2Class value.
     */
    std::string co2ClassExplained(Co2Class value);


    /**
     * @brief Converts a string to its Co2Class enum value.
     *        Returns Co2Class::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The Co2Class value corresponding to the string.
     */
    Co2Class co2ClassFromString(const std::string& value);


    /**
     * @brief Returns whether a Co2Class value is valid (i.e. not NA).
     *
     * @param value The Co2Class value to check.
     * @return True if the value is known, false otherwise.
     */
    bool co2ClassIsValid(Co2Class value);

} // namespace CarScraper