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
    constexpr       int         SUCCESS_CODE        = 0;        ///< Sentinel return value for success
    constexpr       int         ERROR_CODE          = -1;       ///< Sentinel return value for error
    constexpr       int         IGNORED_ACTION_CODE = 1;        ///< Sentinel return value for ignored action





    // =========================================================================
    // String and numeric validation constants
    // =========================================================================
    constexpr       int         DEFAULT_INT     = 9999;         ///< Sentinel default value for int
    constexpr       int         ERROR_INT       = -1;           ///< Sentinel error value for int

    constexpr       double      DEFAULT_DOUBLE  = 9999.0;       ///< Sentinel default value for double
    constexpr       double      ERROR_DOUBLE    = -1.0;         ///< Sentinel error value for double

    inline const    std::string DEFAULT_STR     = "#";          ///< Sentinel default value for string
    inline const    std::string ERROR_STR       = "error";      ///< Sentinel error value for string
    inline const    std::string NONE_STR        = "empty";      ///< Sentinel none value for string





    // =========================================================================
    // Default directory path
    // =========================================================================

    inline const    std::string HTML_DIR        = "data/save_html/";    ///< Default path for the saved html directory
    inline const    std::string LINK_DIR        = "data/save_link/";    ///< Default path for the saved link directory
    inline const    std::string CAR_SAVE        = "data/save_car/";     ///< Default path for the saved car directory
    inline const    std::string IN_XLSX_DIR     = "data/input_excel/";  ///< Default path for the excel input directory

} // namespace CarScraper