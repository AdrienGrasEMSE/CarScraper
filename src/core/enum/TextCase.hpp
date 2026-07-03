/**
 * @file TextCase.hpp
 * 
 * @brief This file defines the enum for the text case, with various utilities.
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
     * @brief Text case, for string formatting
     */
    enum class TextCase {
        LOWER,      ///< Lowercase
        UPPER,      ///< Uppercase
        TITLE,      ///< Title case
        NA          ///< Unknown
    };


    // =========================================================================
    // Conversion functions
    // =========================================================================

    /**
     * @brief Converts a TextCase enum value to its string representation.
     * @param value The TextCase value to convert.
     * @return The string representation of the TextCase value.
     */
    std::string textCaseToString(TextCase value);


    /**
     * @brief Converts a string to its TextCase enum value.
     *        Returns TextCase::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The TextCase value corresponding to the string.
     */
    TextCase textCaseFromString(const std::string& value);


    /**
     * @brief Returns whether a TextCase value is valid (i.e. not NA).
     * @param value The TextCase value to check.
     * @return True if the value is known, false otherwise.
     */
    bool textCaseIsValid(TextCase value);

} // namespace CarScraper