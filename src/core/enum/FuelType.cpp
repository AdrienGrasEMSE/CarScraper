/**
 * @file FuelType.hpp
 * 
 * @brief This file implements the enum for the fuel type, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// Imports
#include "FuelType.hpp"
#include <uni_algo/case.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @brief Converts a FuelType enum value to its string representation.
     *
     * @param value The FuelType value to convert.
     * @return The string representation of the FuelType value.
     */
    std::string fuelTypeToString(FuelType value) {
        switch (value) {
            case FuelType::AC:  return "AC";
            case FuelType::EE:  return "EE";
            case FuelType::EG:  return "EG";
            case FuelType::EH:  return "EH";
            case FuelType::EL:  return "EL";
            case FuelType::EM:  return "EM";
            case FuelType::EN:  return "EN";
            case FuelType::EP:  return "EP";
            case FuelType::EQ:  return "EQ";
            case FuelType::ER:  return "ER";
            case FuelType::ES:  return "ES";
            case FuelType::ET:  return "ET";
            case FuelType::FE:  return "FE";
            case FuelType::FG:  return "FG";
            case FuelType::FL:  return "FL";
            case FuelType::FN:  return "FN";
            case FuelType::GA:  return "GA";
            case FuelType::GE:  return "GE";
            case FuelType::GF:  return "GF";
            case FuelType::GG:  return "GG";
            case FuelType::GH:  return "GH";
            case FuelType::GL:  return "GL";
            case FuelType::GM:  return "GM";
            case FuelType::GN:  return "GN";
            case FuelType::GO:  return "GO";
            case FuelType::GP:  return "GP";
            case FuelType::GQ:  return "GQ";
            case FuelType::GZ:  return "GZ";
            case FuelType::H2:  return "H2";
            case FuelType::NE:  return "NE";
            case FuelType::NH:  return "NH";
            case FuelType::PE:  return "PE";
            case FuelType::PH:  return "PH";
            case FuelType::PL:  return "PL";
            default:            return "NA";
        }
    }


    /**
     * @brief Converts a FuelType enum value to its signification.
     *
     * @param value The FuelType value to convert.
     * @return The string signification of the FuelType value.
     */
    std::string fuelTypeExplained(FuelType value) {
        switch (value) {
            case FuelType::AC:  return "AC";
            case FuelType::EE:  return "Essence électricité (hybride rechargeable)";
            case FuelType::EG:  return "Bicarburation essence-GPL";
            case FuelType::EH:  return "Essence électricité (hybride non rechargeable)";
            case FuelType::EL:  return "Electricité";
            case FuelType::EM:  return "EM";
            case FuelType::EN:  return "EN";
            case FuelType::EP:  return "EP";
            case FuelType::EQ:  return "EQ";
            case FuelType::ER:  return "ER";
            case FuelType::ES:  return "Essence";
            case FuelType::ET:  return "Ethanol";
            case FuelType::FE:  return "Superéthanol";
            case FuelType::FG:  return "FG";
            case FuelType::FL:  return "FL";
            case FuelType::FN:  return "FN";
            case FuelType::GA:  return "GA";
            case FuelType::GE:  return "GE";
            case FuelType::GF:  return "GF";
            case FuelType::GG:  return "GG";
            case FuelType::GH:  return "Gazole-électricité (hybride non rechargeable)";
            case FuelType::GL:  return "Gazole-électricité (hybride rechargeable)";
            case FuelType::GM:  return "GM";
            case FuelType::GN:  return "GN";
            case FuelType::GO:  return "Diesel";
            case FuelType::GP:  return "GPL";
            case FuelType::GQ:  return "GQ";
            case FuelType::GZ:  return "GZ";
            case FuelType::H2:  return "Hydrogène";
            case FuelType::NE:  return "NE";
            case FuelType::NH:  return "NH";
            case FuelType::PE:  return "PE";
            case FuelType::PH:  return "PH";
            case FuelType::PL:  return "PL";
            default:            return "Unknown";
        }
    }


    /**
     * @brief Converts a string to its FuelType enum value.
     *        Returns FuelType::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The FuelType value corresponding to the string.
     */
    FuelType fuelTypeFromString(const std::string& value) {

        // Lowercase noramlisation
        const std::string lower = una::cases::to_lowercase_utf8(value);


        // EE : Essence électricité (hybride rechargeable)
        if (lower == "hybride : essence/electrique") {
            return FuelType::EE;
        }


        // EL : Electricité
        if (lower == "electrique") {
            return FuelType::EL;
        }


        // ES : Essence
        if (lower == "essence") {
            return FuelType::ES;
        }


        // GL : Gazole-électricité (hybride rechargeable)
        if (lower == "diesel/micro-hybride") {
            return FuelType::GL;
        }


        // GO : Diesel
        if (lower == "diesel") {
            return FuelType::GO;
        }


        // GP : GPL
        if (lower == "gpl") {
            return FuelType::GP;
        }


        // H2 : Hydrogène
        if (lower == "hydrogène" ||
            lower == "hydrogene") {
            return FuelType::H2;
        }


        // Unknown
        return FuelType::NA;

    }


    /**
     * @brief Returns whether a FuelType value is valid (i.e. not NA).
     *
     * @param value The FuelType value to check.
     * @return True if the value is known, false otherwise.
     */
    bool fuelTypeIsValid(FuelType value) {
        return value != FuelType::NA;
    }

} // namespace CarScraper