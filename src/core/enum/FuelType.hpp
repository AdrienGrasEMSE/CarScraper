/**
 * @file FuelType.hpp
 * 
 * @brief This file defines the enum for the fuel type, with various utilities.
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

    enum class FuelType {
        AC, // Air comprimé
        EE, // Essence électricité (hybride rechargeable)
        EG, // Bicarburation essence-GPL
        EH, // Essence-électricité (hybride non rechargeable)
        EL, // Electricité
        EM, // Bicarburation essence-gaz naturel et électricité (hybride rechargeable)
        EN, // Bicarburation essence-gaz naturel
        EP, // Bicarburation essence-gaz naturel et électricité (hybride non rechargeable)
        EQ, // Bicarburation essence-GPL et électricité (hybride non rechargeable)
        ER, // Bicarburation essence-GPL et électricité (hybride rechargeable)
        ES, // Essence
        ET, // Ethanol
        FE, // Superéthanol
        FG, // Bicarburation superéthanol-GPL
        FL, // Superéthanol-électricité (hybride rechargeable)
        FN, // Bicarburation superéthanol-gaz naturel
        GA, // Gazogène (*)
        GE, // Mélange gazogène-essence (*)
        GF, // Mélange de gazole et gaz naturel (dual fuel)
        GG, // Mélange gazogène-gazole (*)
        GH, // Gazole-électricité (hybride non rechargeable)
        GL, // Gazole-électricité (hybride rechargeable)
        GM, // Mélange de gazole et gaz naturel (dual fuel) et électricité (hybride rechargeable)
        GN, // Gaz naturel
        GO, // Gazole (ou diesel)
        GP, // Gaz de pétrole liquéfié GPL (mélange spécial de butane et de propane, à l’exception des butane et propane commerciaux) utilisé en tant que carburant exclusif
        GQ, // Mélange de gazole et gaz naturel (dual fuel) et électricité (hybride non rechargeable)
        GZ, // Autres hydrocarbures gazeux comprimés
        H2, // Hydrogène
        NE, // Gaz naturel-électricité (hybride rechargeable)
        NH, // Gaz naturel-électricité (hybride non rechargeable)
        PE, // Monocarburation GPL-électricité (hybride rechargeable)
        PH, // Monocarburation GPL-électricité (hybride non rechargeable)
        PL, // Pétrole lampant
        NA  // Inconnu
    };


    // =========================================================================
    // Conversion functions
    // =========================================================================

    /**
     * @brief Converts a FuelType enum value to its string representation.
     *
     * @param value The FuelType value to convert.
     * @return The string representation of the FuelType value.
     */
    std::string fuelTypeToString(FuelType value);


    /**
     * @brief Converts a FuelType enum value to its signification.
     *
     * @param value The FuelType value to convert.
     * @return The string signification of the FuelType value.
     */
    std::string fuelTypeExplained(FuelType value);


    /**
     * @brief Converts a string to its FuelType enum value.
     *        Returns FuelType::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The FuelType value corresponding to the string.
     */
    FuelType fuelTypeFromString(const std::string& value);


    /**
     * @brief Returns whether a FuelType value is valid (i.e. not NA).
     *
     * @param value The FuelType value to check.
     * @return True if the value is known, false otherwise.
     */
    bool fuelTypeIsValid(FuelType value);

} // namespace CarScraper