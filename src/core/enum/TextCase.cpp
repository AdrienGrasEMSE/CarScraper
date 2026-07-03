/**
 * @file TextCase.cpp
 * 
 * @brief This file implements the enum for the text case, with various utilities.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include "TextCase.hpp"
#include <uni_algo/case.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @brief Converts a TextCase enum value to its string representation.
     * @param value The TextCase value to convert.
     * @return The string representation of the TextCase value.
     */
    std::string textCaseToString(TextCase value) {
        switch (value) {
            case TextCase::LOWER:   return "Lowercase";
            case TextCase::UPPER:   return "Uppercase";
            case TextCase::TITLE:   return "Title case";
            default:                return "Unknown";
        }
    }


    /**
     * @brief Converts a string to its TextCase enum value.
     *        Returns TextCase::NA if the string does not match any known value.
     *
     * @param value The string to convert.
     * @return The TextCase value corresponding to the string.
     */
    TextCase textCaseFromString(const std::string& value) {

        // Lowercase noramlisation
        const std::string lower = una::cases::to_lowercase_utf8(value);


        // LOWER : Lowercase
        if (lower == "lowercase") {
            return TextCase::LOWER;
        }

        // UPPER : Uppercase
        if (lower == "uppercase") {
            return TextCase::UPPER;
        }

        // TITLE : Title case
        if (lower == "title case") {
            return TextCase::TITLE;
        }

        // Unknown
        return TextCase::NA;

    }


    /**
     * @brief Returns whether a TextCase value is valid (i.e. not NA).
     * @param value The TextCase value to check.
     * @return True if the value is known, false otherwise.
     */
    bool textCaseIsValid(TextCase value) {
        return value != TextCase::NA;
    }

} // namespace CarScraper