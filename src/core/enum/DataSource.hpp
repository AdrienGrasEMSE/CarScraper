/**
 * @file DataSource.hpp
 * 
 * @brief This file defines the enum for the data source, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
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

    /**
     * @brief Data source, from largus.fr or caradisiac.com
     */
    enum class DataSource {
        ARGUS,      ///< Data from largus.fr
        CARADISIAC, ///< Data from caradisiac.com
        NA          ///< Unknown
    };


    // =========================================================================
    // Conversion functions
    // =========================================================================

    /**
     * @brief Converts a DataSource enum value to its string representation.
     * @param value The DataSource value to convert.
     * @return The string representation of the DataSource value.
     */
    std::string dataSourceToString(DataSource value);


    /**
     * @brief Converts a string to its DataSource enum value.
     *        Returns DataSource::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The DataSource value corresponding to the string.
     */
    DataSource dataSourceFromString(const std::string& value);


    /**
     * @brief Returns whether a DataSource value is valid (i.e. not NA).
     * @param value The DataSource value to check.
     * @return True if the value is known, false otherwise.
     */
    bool dataSourceIsValid(DataSource value);

} // namespace CarScraper