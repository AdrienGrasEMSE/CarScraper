/**
 * @file CleanService.hpp
 * 
 * @brief This file declares a cleaning service, which represents a service able to clean directories and save link.
 *
 * @author Adrien GRAS
 * @date 2026-08-04
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include <vector>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @struct DirectoryContent
     * @brief Represents the content of a directory, including files and subdirectories.
     */
    struct DirectoryContent {
        std::vector<std::string> files;        ///< List of files in the directory
        std::vector<std::string> directories;  ///< List of subdirectories in the directory
    };





    /**
     * @class CleanService
     * 
     * @brief A service able to clean directories and reset the saved link file
     */
    class CleanService : public Entity {

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
            CleanService();





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Explore a directory and return its content
             * @param directoryPath The path of the directory to explore
             * @return The content of the directory, including files and subdirectories
             */
            DirectoryContent exploreDirectory(const std::string& directoryPath);


            /**
             * @brief Remove a list of paths (files or directories)
             * @param paths The list of paths to remove
             * @return SUCCESS_CODE or ERROR_CODE based on the result of the operation
             */
            int removePaths(const std::vector<std::string>& paths);


            /**
             * @brief Clean the saved link file
             * @return SUCCESS_CODE or ERROR_CODE based on the result of the operation
             */
            int cleanLink();

    };

}