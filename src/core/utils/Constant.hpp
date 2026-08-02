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

    /**
     * @brief Constant values used in the CarScraper project
     * @details This namespace contains constant values for default and error sentinel values,
     *          as well as default directory paths for saving data.
     * @note The sentinel values uses prime number to avoid collision with actual calculated values.
     * Here is a list of prime numbers used :
     * 1000003
     * 1705141
     * 2432597
     * 3174553
     * 3928669
     * 4692953
     * 5464033
     * 7028977
     * 8616563
     * 9999991
     */

    // =========================================================================
    // String and numeric validation constants
    // =========================================================================
    constexpr       int         SUCCESS_CODE        = 9999991;          ///< Sentinel return value for success
    constexpr       int         ERROR_CODE          = -9999991;         ///< Sentinel return value for error
    constexpr       int         IGNORED_ACTION_CODE = 1000003;          ///< Sentinel return value for ignored action





    // =========================================================================
    // String and numeric validation constants
    // =========================================================================
    constexpr       int         DEFAULT_INT     = 9999991;              ///< Sentinel default value for int (primanry number for unicity)
    constexpr       int         ERROR_INT       = -9999991;             ///< Sentinel error value for int (negative primanry number for unicity)

    constexpr       double      DEFAULT_DOUBLE  = 0.9999991;            ///< Sentinel default value for double (. primanry number for unicity)
    constexpr       double      ERROR_DOUBLE    = -0.9999991;           ///< Sentinel error value for double (negative . primanry number for unicity)

    inline const    std::string DEFAULT_STR     = "&1705141 - default"; ///< Sentinel default value for string (Symbole &xxx for unicity)
    inline const    std::string ERROR_STR       = "&9999991 - error";   ///< Sentinel error value for string (Symbole &xxx for unicity)
    inline const    std::string NONE_STR        = "&1000003 - none";    ///< Sentinel none value for string (Symbole &xxx for unicity)





    // =========================================================================
    // Default directory path
    // =========================================================================

    inline const    std::string HTML_DIR        = "data/save_html/";    ///< Default path for the saved html directory
    inline const    std::string LINK_DIR        = "data/save_link/";    ///< Default path for the saved link directory
    inline const    std::string CAR_SAVE        = "data/save_car/";     ///< Default path for the saved car directory
    inline const    std::string IN_XLSX_DIR     = "data/input_excel/";  ///< Default path for the excel input directory

} // namespace CarScraper