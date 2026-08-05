/**
 * @file CleanService.cpp
 * 
 * @brief This file implements a cleaning service, which represents a service able to clean directories and save link.
 *
 * @author Adrien GRAS
 * @date 2026-08-04
 */


// Imports
#include "CleanService.hpp"
#include "core/logger/Logger.hpp"
#include "core/utils/Constant.hpp"
#include <filesystem>
#include <fstream>


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
    CleanService::CleanService() : Entity("CLEAN-SERVICE") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Explore a directory and return its content
     * @param directoryPath The path of the directory to explore
     * @return The content of the directory, including files and subdirectories
     */
    DirectoryContent CleanService::exploreDirectory(const std::string& directoryPath) {

        // Namespace alias for filesystem
        namespace fs = std::filesystem;
        DirectoryContent result;


        // Checking if the directory exists
        if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
            return result;
        }
        

        // Explore the directory recursively and collect files and subdirectories
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                result.files.push_back(entry.path().string());
            }
            else if (entry.is_directory()) {
                result.directories.push_back(entry.path().string());
            }
        }

        
        return result;
    }


    /**
     * @brief Remove a list of paths (files or directories)
     * @param paths The list of paths to remove
     * @return SUCCESS_CODE or ERROR_CODE based on the result of the operation
     */
    int CleanService::removePaths(const std::vector<std::string>& paths) {

        // Namespace alias for filesystem
        namespace fs = std::filesystem;
        Logger::debug("[{}].removePaths : start", getFullId());


        // Running through all paths
        int cpt_deleted = 0;
        try {
            for (const auto& path : paths) {

                // Removing the file / directory
                fs::path p(path);
                if (fs::exists(p)) {
                    if (fs::is_directory(p)) {
                        Logger::trace("[{}].removePaths : removing directory \"{}\"", getFullId(), path);
                        fs::remove_all(p);
                        cpt_deleted++;
                    } else {
                        Logger::trace("[{}].removePaths : removing file \"{}\"", getFullId(), path);
                        fs::remove(p);
                        cpt_deleted++;
                    }
                }

            }

        } catch (const std::exception& e) {
            Logger::error("[{}].removePaths : error \"{}\"", getFullId(), e.what());
            return ERROR_CODE;
        }


        // Debug
        Logger::debug("[{}].removePaths : deleted {} files / directories", getFullId(), cpt_deleted);
        return SUCCESS_CODE;

    }


    /**
     * @brief Clean the saved link file
     */
    int CleanService::cleanLink() {

        // Debug
        Logger::debug("[{}].cleanLink : start cleaning \"{}\"", getFullId(), LINK_DIR + "saved_link.json");


        // Reset saved link file
        std::ofstream file(LINK_DIR + "saved_link.json", std::ios::trunc);
        try {
            if (file) {
                file << "[\n]";
                file.close();
            }
        } catch (const std::exception& e) {
            Logger::error("[{}].cleanLink : error \"{}\"", getFullId(), e.what());
            return ERROR_CODE;
        }
        

        // Debug
        Logger::debug("[{}].cleanLink : finnich cleaning \"{}\"", getFullId(), LINK_DIR + "saved_link.json");
        return SUCCESS_CODE;

    }

}