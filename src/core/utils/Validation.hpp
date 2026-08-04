/**
 * @file Validation.hpp
 * 
 * @brief This file defines the validation functions used in the CarScraper project.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// For only one include
#pragma once


// Imports
#include <string>
#include <chrono>
#include <optional>


/**
 * Namespace CarScraper::Validation
 */
namespace CarScraper::Validation {

    // =========================================================================
    // String and numeric validations
    // =========================================================================

    /**
     * @brief Validates a string value against a maximum length.
     *
     * @param strVal The string value to validate.
     * @param objectID The ID of the object being validated.
     * @param attributeName The name of the attribute being validated.
     * @param maxLength The maximum allowed length for the string.
     * @return The validated string value.
     */
    std::string stringValidation(   const std::string&      strVal,
                                    const std::string&      objectID,
                                    const std::string&      attributeName,
                                    const long unsigned int maxLength = 255);

    
    /**
     * @brief Validates an integer value against minimum and maximum values.
     *
     * @param intVal The integer value to validate.
     * @param objectID The ID of the object being validated.
     * @param attributeName The name of the attribute being validated.
     * @param minValue The minimum allowed value for the integer.
     * @param maxValue The maximum allowed value for the integer.
     * @return The validated integer value.
     */
    int intValidation(  const int           intVal,
                        const std::string&  objectID,
                        const std::string&  attributeName,
                        const int           minValue = 0,
                        const int           maxValue = 9999);


    /**
     * @brief Validates a double value against minimum and maximum values.
     *
     * @param doubleVal The double value to validate.
     * @param objectID The ID of the object being validated.
     * @param attributeName The name of the attribute being validated.
     * @param minValue The minimum allowed value for the double.
     * @param maxValue The maximum allowed value for the double.
     * @return The validated double value.
     */
    double doubleValidation(const double        doubleVal,
                            const std::string&  objectID,
                            const std::string&  attributeName,
                            const double        minValue = 0.0,
                            const double        maxValue = 9999.0);





    // =========================================================================
    // Date parsing and formatting
    // =========================================================================

    /**
     * @brief Parses a date string in the format DD/MM/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateDMY(const std::string& strVal);

                                                            
    /**
     * @brief Parses a date string in the format YYYY/MM/DD.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateYMD(const std::string& strVal);


    /**
     * @brief Parses a date string in the format MM/DD/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateMDY(const std::string& strVal);

                                                            
    /**
     * @brief Formats a date into a string in the format DD/MM/YYYY.
     *
     * @param date The date to format.
     * @return The formatted date string.
     */
    std::string formatDate(const std::chrono::year_month_day& date);





    // =========================================================================
    // String manipulations
    // =========================================================================

    /**
     * @brief Converts a string to uppercase
     * @param str The string to convert
     * @return The uppercase string
     */
    std::string toUpperCase(const std::string& str);


    /**
     * @brief Converts a string to lowercase
     * @param str The string to convert
     * @return The lowercase string
     */
    std::string toLowerCase(const std::string& str);


    /**
     * @brief Remove a prefix from a string
     * @param str The base string
     * @param prefix The prefix to remove
     * @return The string without the prefix or ERROR_STR if the prefix is not found
     * @note Case insensitive, also removes all whitespace at the beginning of the string
     */
    std::string removePrefix(const std::string& str, const std::string& prefix);


    /**
     * @brief Remove a suffix from a string
     * @param str The base string
     * @param suffix The suffix to remove
     * @return The string without the suffix or ERROR_STR if the suffix is not found
     * @note Case insensitive, also removes all whitespace at the end of the string
     */
    std::string removeSuffix(const std::string& str, const std::string& suffix);


    /**
     * @brief Extract a roman number from a string
     * @param str The base string
     * @return The roman number or ERROR_STR if nothing found
     * @note Case insensitive (forced uppercase)
     */
    std::string extractRomanNumbers(const std::string& str);


    /**
     * @brief Convert a roman number into a arabic one
     * @param romanNumber The base string
     * @return The converted arabic number
     */
    int romanToInt(const std::string& romanNumber);


} // namespace CarScraper::Validation