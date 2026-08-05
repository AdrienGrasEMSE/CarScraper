/**
 * @file MapService.cpp
 * 
 * @brief This file implements a mapping service, which represents a service able to map cars technical sheets.
 *
 * @author Adrien GRAS
 * @date 2026-08-04
 */


// Imports
#include "MapService.hpp"
#include "core/logger/Logger.hpp"
#include "core/utils/Constant.hpp"
#include <filesystem>


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
    MapService::MapService() : Entity("MAPPING-SERVICE") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Run the mapping process for a specific car model
     * @param brand The target car brand
     * @param model The target car model
     * @return SUCCESS_CODE or ERROR_CODE based on the result of the operation
     */
    int MapService::run(const std::string& brand, const std::string& model) {

        // Namespace alias for filesystem
        namespace fs = std::filesystem;
        Logger::info("[{}] : starting to map \"{}\" \"{}\" technical data", getFullId(), brand, model);


        // Getting all technical data from the html directory
        std::vector<std::string> argusFiles;
        std::vector<std::string> caradisiacFiles;
        for (const auto& entry : fs::directory_iterator(HTML_DIR)) {

            // Getting the file name
            std::string fileName = entry.path().filename().string();


            // Filtering
            if (fileName.starts_with("ARGUS_")) {
                Logger::trace("[{}] : got \"{}\"", getFullId(), fileName);
                argusFiles.push_back(fileName);
            } else if (fileName.starts_with("CARADISIAC_")) {
                Logger::trace("[{}] : got \"{}\"", getFullId(), fileName);
                caradisiacFiles.push_back(fileName);
            }

        }



        // Debug
        Logger::info("[{}] : created {} car objects for \"{}\" \"{}\"", getFullId(), 3, brand, model);
        return SUCCESS_CODE;

    }

}