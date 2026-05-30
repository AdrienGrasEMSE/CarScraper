/**
 * @file GearboxType.cpp
 * 
 * @brief This file implements the enum for the gear box type, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// Imports
#include "GearboxType.hpp"
#include <uni_algo/case.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @brief Converts a GearboxType enum value to its string representation.
     *
     * @param value The GearboxType value to convert.
     * @return The string representation of the GearboxType value.
     */
    std::string gearBoxTypeToString(GearboxType value) {
        switch (value) {
            case GearboxType::AUTOMATIC:    return "Automatique";
            case GearboxType::MANUAL:       return "Manuelle";
            default:                        return "Unkown";
        }
    }


    /**
     * @brief Converts a string to its GearboxType enum value.
     *        Returns GearboxType::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The GearboxType value corresponding to the string.
     */
    GearboxType gearBoxTypeFromString(const std::string& value) {

        // Lowercase noramlisation
        const std::string lower = una::cases::to_lowercase_utf8(value);


        // AUTOMATIC : Automatique
        if (lower == "automatique") {
            return GearboxType::AUTOMATIC;
        }


        // MANUAL : Manuelle
        if (lower == "manuelle") {
            return GearboxType::MANUAL;
        }


        // Unknown
        return GearboxType::NA;

    }


    /**
     * @brief Returns whether a GearboxType value is valid (i.e. not NA).
     *
     * @param value The GearboxType value to check.
     * @return True if the value is known, false otherwise.
     */
    bool gearBoxTypeIsValid(GearboxType value) {
        return value != GearboxType::NA;
    }

} // namespace CarScraper