/**
 * @file ScrapService.cpp
 * 
 * @brief This file implements a scraping service, which represents a service able to scrap car technical sheets from
 * largus.fr and caradisiac.com.
 *
 * @author Adrien GRAS
 * @date 2026-08-03
 */


// Imports
#include "ScrapService.hpp"
#include "core/logger/Logger.hpp"
#include "core/scrapers/ArgusScraper.hpp"
#include "core/scrapers/CaradisiacScraper.hpp"
#include "core/utils/Constant.hpp"
#include <future>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default Constructor
     */
    ScrapService::ScrapService() : Entity("SCRAP-SERVICE") {}




    
    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Scrap a car model on the Argus and Cardisiac website, combine tehcnical data and save them
     * @param brand The target car brand
     * @param model The target car model
     * @param startDate The start date which the scraping start after
     * @param endDate The end date which the scraping start before
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note The accepted date format is "AAAA"
     */
    int ScrapService::run(   const std::string& brand,
                                const std::string& model,
                                const std::string& startDateStr,
                                const std::string& endDateStr) {
        // Debug
        Logger::info("[{}] : Starting to scrap \"{}\" \"{}\" bewteen \"{}\" and \"{}\"",
            getFullId(), brand, model, startDateStr, endDateStr);
        

        // Creating scrapers
        ArgusScraper        argusScraper;
        CaradisiacScraper   caradisiacScraper;


        // Scraper set-up
        argusScraper        .setCarBrand(brand);
        caradisiacScraper   .setCarBrand(brand);
        argusScraper        .setCarModel(model);
        caradisiacScraper   .setCarModel(model);


        // Launching scraping simultaneously
        auto futureArgus        = std::async(std::launch::async, [&] {return argusScraper.scrapModel(startDateStr, endDateStr);});
        auto futureCaradisiac   = std::async(std::launch::async, [&] {return caradisiacScraper.scrapModel(startDateStr, endDateStr);});


        // Getting status code
        int statusArgus         = futureArgus.get();
        int statusCaradisiac    = futureCaradisiac.get();// Final status
        if (statusArgus == SUCCESS_CODE && statusCaradisiac == SUCCESS_CODE) {

            // Debug
            Logger::info("[{}] : Scraping of \"{}\" \"{}\" bewteen \"{}\" and \"{}\" succesful",
                getFullId(), brand, model, startDateStr, endDateStr);
            return SUCCESS_CODE;

            
        } else {

            // Debug
            Logger::error("[{}] : Error while scraping of \"{}\" \"{}\" bewteen \"{}\" and \"{}\"",
                getFullId(), brand, model, startDateStr, endDateStr);
            return ERROR_CODE;

        }

    }


}