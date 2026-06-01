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
    constexpr       int         DEFAULT_INT    = 9999;          ///< Sentinel default value for int
    constexpr       int         ERROR_INT      = -1;            ///< Sentinel error value for int

    constexpr       double      DEFAULT_DOUBLE = 9999.0;        ///< Sentinel default value for double
    constexpr       double      ERROR_DOUBLE   = -1.0;          ///< Sentinel error value for double

    inline const     std::string DEFAULT_STR    = "#";          ///< Sentinel default value for string
    inline const     std::string ERROR_STR      = "error";      ///< Sentinel error value for string

} // namespace CarScraper