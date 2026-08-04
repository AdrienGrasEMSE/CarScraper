/**
 * @file ScrapingService.hpp
 * 
 * @brief This file declares a scraping service, which represents a service able to scrap car technical sheets from
 * largus.fr and caradisiac.com.
 *
 * @author Adrien GRAS
 * @date 2026-08-03
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include <filesystem>
#include <string>
#include <vector>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class ScrapingService
     * 
     * @brief A service able to complete the full scraping job on largus.fr and caradisiac.com. It scraps all
     * targeted technical files, combine them into a completed one before saving.
     */
    class ScrapingService : public Entity {

        // =========================================================================
        // Public data and methods
        // =========================================================================
        private:

            // -------------------------------------------------------------------------
            // Internal helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Filter a list of files by car brand and model
             * @param files The list of files to filter
             * @param brand The target car brand
             * @param model The target car model
             */
            void filterFilesByBrandAndModel(std::vector<std::filesystem::path>& files,
                                            const std::string& brand,
                                            const std::string& model);





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor
             */
            ScrapingService();





            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------
            
            /**
             * @brief Scrap a car model on the Argus and Cardisiac website, combine tehcnical data and save them
             * @param brand The target car brand
             * @param model The target car model
             * @param startDate The start date which the scraping start after
             * @param endDate The end date which the scraping start before
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note The accepted date format is "AAAA"
             */
            int run(const std::string& brand,
                    const std::string& model,
                    const std::string& startDateStr,
                    const std::string& endDateStr);

    };

}