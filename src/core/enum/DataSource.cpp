/**
 * @file DataSource.cpp
 * 
 * @brief This file implements the enum for the data source, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include "DataSource.hpp"
#include <uni_algo/case.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @brief Converts a DataSource enum value to its string representation.
     * @param value The DataSource value to convert.
     * @return The string representation of the DataSource value.
     */
    std::string dataSourceToString(DataSource value) {
        switch (value) {
            case DataSource::ARGUS:         return "Argus";
            case DataSource::CARADISIAC:    return "Caradisiac";
            default:                        return "Unknown";
        }
    }


    /**
     * @brief Converts a string to its DataSource enum value.
     *        Returns DataSource::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The DataSource value corresponding to the string.
     */
    DataSource dataSourceFromString(const std::string& value) {

        // Lowercase noramlisation
        const std::string lower = una::cases::to_lowercase_utf8(value);


        // ARGUS : Argus
        if (lower == "argus") {
            return DataSource::ARGUS;
        }

        // CARADISIAC : Caradisiac
        if (lower == "caradisiac") {
            return DataSource::CARADISIAC;
        }

        // Unknown
        return DataSource::NA;

    }


    /**
     * @brief Returns whether a DataSource value is valid (i.e. not NA).
     * @param value The DataSource value to check.
     * @return True if the value is known, false otherwise.
     */
    bool dataSourceIsValid(DataSource value) {
        return value != DataSource::NA;
    }

} // namespace CarScraper