/**
 * @file CaradisiacScraper.hpp
 * 
 * @brief This file declares the CaradisiacScraper class, which represents a scraper for the Caradisiac website.
 *
 * @author Adrien GRAS
 * @date 2026-06-29
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
     * @class CaradisiacScraper
     * 
     * @brief Scrap the Caradisiac website.
     */
    class CaradisiacScraper : public GenericCarScraper {

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
            CaradisiacScraper();




            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Scrap a car model on the Caradisiac website
             * @param startDate The start date which the scraping start after
             * @param endDate The end date which the scraping start before
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note The accepted date format is "AAAA"
             */
            using GenericCarScraper::scrapModel;
            int scrapModel(const std::string& startDateStr, const std::string& endDateStr);


            /**
             * @brief Scrap a car all generations of a model in the specified interval
             * @param modelLink The link of the model to scrap
             * @param response The previous HTTP response containing the model page
             * @param dateInterval The interval of dates to scrap
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note The accepted date format is "AAAA"
             */
            int modelScraper(std::string& modelLink, CarScraper::HttpResponse response, std::vector<int>& dateInterval);


            /**
             * @brief Scrap a car all years of a generation in the specified interval
             * @param genLink The link of the generation to scrap
             * @param response The previous HTTP response containing the generation page
             * @param dateInterval The interval of dates to scrap
             * @note The accepted date format is "AAAA"
             */
            void yearScraper(std::string& genLink, CarScraper::HttpResponse response, std::vector<int>& dateInterval);

    };

}