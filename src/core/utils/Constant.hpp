/**
 * @file Constant.hpp
 * 
 * @brief This file defines the constant values used in the CarScraper project.
 *        These constants include default values for car attributes and error sentinel values.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
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
    // String and numeric validation constants
    // =========================================================================
    constexpr       int         DEFAULT_INT    = 9999;
    constexpr       int         ERROR_INT      = -1;

    constexpr       double      DEFAULT_DOUBLE = 9999.0;
    constexpr       double      ERROR_DOUBLE   = -1.0;

    inline const     std::string DEFAULT_STR    = "#";
    inline const     std::string ERROR_STR      = "error";

} // namespace CarScraper