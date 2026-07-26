/**
 * @file CaradisiacMapper.hpp
 * 
 * @brief This file declares the CaradisiacMapper class, which is a mapper able to generate a Car Entity from a technical sheet from caradisiac.com.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// For only one include
#pragma once


// Imports
#include "GenericCarMapper.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class CaradisiacMapper
     * 
     * @brief Class representing a Caradisiac mapper
     */
    class CaradisiacMapper : public GenericCarMapper {

        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor.
             */
            CaradisiacMapper();


            


            // -------------------------------------------------------------------------
            // Internal Helpers
            // -------------------------------------------------------------------------

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


            /**
             * @brief Extract the generation, phase and engine from the title
             * @param title The raw title extracted from the HTML
             */
            void titleProcessing(const auto& title);





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Map a car from the Caradisiac website
             * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
             */
            using GenericCarMapper::mapToCar;
            int mapToCar();
    };

} // namespace CarScraper