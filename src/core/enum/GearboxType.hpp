/**
 * @file GearboxType.hpp
 * 
 * @brief This file defines the enum for the gear box type, with various utilities.
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

    enum class GearboxType {
        AUTOMATIC,
        MANUAL,
        NA  // Unknown
    };


    // =========================================================================
    // Conversion functions
    // =========================================================================

    /**
     * @brief Converts a GearboxType enum value to its string representation.
     *
     * @param value The GearboxType value to convert.
     * @return The string representation of the GearboxType value.
     */
    std::string gearBoxTypeToString(GearboxType value);


    /**
     * @brief Converts a string to its GearboxType enum value.
     *        Returns GearboxType::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The GearboxType value corresponding to the string.
     */
    GearboxType gearBoxTypeFromString(const std::string& value);


    /**
     * @brief Returns whether a GearboxType value is valid (i.e. not NA).
     *
     * @param value The GearboxType value to check.
     * @return True if the value is known, false otherwise.
     */
    bool gearBoxTypeIsValid(GearboxType value);

} // namespace CarScraper