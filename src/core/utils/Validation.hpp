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
     * Validates a string value against a maximum length.
     *
     * @param strVal The string value to validate.
     * @param className The name of the class being validated.
     * @param attributeName The name of the attribute being validated.
     * @param maxLength The maximum allowed length for the string.
     * @return The validated string value.
     */
    std::string stringValidation(   const std::string&      strVal,
                                    const std::string&      className,
                                    const std::string&      attributeName,
                                    const long unsigned int maxLength = 255);

    
    /**
     * Validates an integer value against minimum and maximum values.
     *
     * @param intVal The integer value to validate.
     * @param className The name of the class being validated.
     * @param attributeName The name of the attribute being validated.
     * @param minValue The minimum allowed value for the integer.
     * @param maxValue The maximum allowed value for the integer.
     * @return The validated integer value.
     */
    int intValidation(  const int           intVal,
                        const std::string&  className,
                        const std::string&  attributeName,
                        const int           minValue = 0,
                        const int           maxValue = 9999);


    /**
     * Validates a double value against minimum and maximum values.
     *
     * @param doubleVal The double value to validate.
     * @param className The name of the class being validated.
     * @param attributeName The name of the attribute being validated.
     * @param minValue The minimum allowed value for the double.
     * @param maxValue The maximum allowed value for the double.
     * @return The validated double value.
     */
    double doubleValidation(const double        doubleVal,
                            const std::string&  className,
                            const std::string&  attributeName,
                            const double        minValue = 0.0,
                            const double        maxValue = 9999.0);





    // =========================================================================
    // Date parsing and formatting
    // =========================================================================

    /**
     * Parses a date string in the format DD/MM/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateDMY(const std::string& strVal);

                                                            
    /**
     * Parses a date string in the format YYYY/MM/DD.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateYMD(const std::string& strVal);


    /**
     * Parses a date string in the format MM/DD/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateMDY(const std::string& strVal);

                                                            
    /**
     * Formats a date into a string in the format DD/MM/YYYY.
     *
     * @param date The date to format.
     * @return The formatted date string.
     */
    std::string formatDate(const std::chrono::year_month_day& date);


} // namespace CarScraper::Validation