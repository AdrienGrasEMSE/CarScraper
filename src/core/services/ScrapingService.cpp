/**
 * @file ScrapingService.cpp
 * 
 * @brief This file implements a scraping service, which represents a service able to scrap car technical sheets from
 * largus.fr and caradisiac.com.
 *
 * @author Adrien GRAS
 * @date 2026-08-03
 */


// Imports
#include "ScrapingService.hpp"
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
    ScrapingService::ScrapingService() : Entity("SCRAP-SERVICE") {}





    // =========================================================================
    // Internal helpers
    // =========================================================================

    /**
     * @brief Filter a list of files by car brand and model
     * @param files The list of files to filter
     * @param brand The target car brand
     * @param model The target car model
     */
    void ScrapingService::filterFilesByBrandAndModel(   std::vector<std::filesystem::path>& files,
                                                        const std::string& brand,
                                                        const std::string& model) {

        // Running through the files
        for (const auto& file : files) {

            // Getting the file path
            std::string filePath = file.string();

            // Filtering files
            if (filePath.find(brand) == std::string::npos || filePath.find(model) == std::string::npos) {
                files.erase(std::remove(files.begin(), files.end(), file), files.end());
                Logger::info("[{}].scrapModel : file removed : {}", getFullId(), file.string());
            }

        }

    }




    
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
    int ScrapingService::run(   const std::string& brand,
                                const std::string& model,
                                const std::string& startDateStr,
                                const std::string& endDateStr) {
        
        // Creating scrapers
        namespace fs = std::filesystem;
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
        int statusCaradisiac    = futureCaradisiac.get();


        // Getting all the files in the save_html directory
        std::vector<fs::path> argusFiles;
        std::vector<fs::path> caradisiacFiles;
        for (const auto& entry : fs::directory_iterator(HTML_DIR)) {

            // Getting the file path
            std::string filePath = entry.path().string();

            // Filtering files by type
            if (entry.is_regular_file()) {
                if (filePath.find("ARGUS") != std::string::npos) {
                    argusFiles.push_back(entry.path());
                } else if (filePath.find("CARADISIAC") != std::string::npos) {
                    caradisiacFiles.push_back(entry.path());
                }
            }

        }


        for (const auto& file : argusFiles) {
            Logger::info("[{}].scrapModel : Argus file found : {}", getFullId(), file.string());
        }
        for (const auto& file : caradisiacFiles) {
            Logger::info("[{}].scrapModel : Caradisiac file found : {}", getFullId(), file.string());
        }


        // Filtering files by brand and model
        filterFilesByBrandAndModel(argusFiles, brand, model);
        filterFilesByBrandAndModel(caradisiacFiles, brand, model);

        
        for (const auto& file : argusFiles) {
            Logger::info("[{}].scrapModel : Argus file found : {}", getFullId(), file.string());
        }
        for (const auto& file : caradisiacFiles) {
            Logger::info("[{}].scrapModel : Caradisiac file found : {}", getFullId(), file.string());
        }



        // Final status
        if (statusArgus == SUCCESS_CODE && statusCaradisiac == SUCCESS_CODE) {
            return SUCCESS_CODE;
        } else {
            return ERROR_CODE;
        }

    }


}