/**
 * @file ArgusScraper.hpp
 * 
 * @brief This file declares the ArgusScraper class, which represents a scraper for the Argus website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// For only one include
#pragma once


// Imports
#include "core/scraper/GenericCarScraper.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class ArgusScraper
     * 
     * @brief Scrap the Argus website.
     */
    class ArgusScraper : public GenericCarScraper {

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
            ArgusScraper();




            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Scrap a car model on the Argus website
             * @param startDate The start date which the scraping start after
             * @param endDate The end date which the scraping start before
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note The accepted date format is "AAAA"
             */
            int scrapModel(const std::string& startDateStr, const std::string& endDateStr);

    };

}