/**
 * @file ArgusScraper.hpp
 * 
 * @brief This file implements the ArgusScraper class, which represents a scraper for the Argus website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// Imports
#include "core/models/Entity.hpp"
#include "core/logger/Logger.hpp"
#include <string>
#include <vector>
#include <map>
#include <filesystem>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor
     */
    ArgusScraper::ArgusScraper() : Entity("ARGUS_SCRAPER") {

        // Set the ouput path for the saved Html
        //_saver.setOutputDir("data/save_html/");

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the input folder path
     * @param inputFolder The input folder path
     */
    void ArgusScraper::setInputFolder(const std::string& inputFolder) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the folder exists
        if (fs::exists(inputFolder)) {
            _inputFolder = inputFolder;
            Logger::trace("[{}].setInputFolder : {}", getFullId(), _inputFolder);
        } else {
            _inputFolder = DEFAULT_STR;
            Logger::debug("[{}].setInputFolder : Folder {} does not exists", getFullId(), inputFolder);
        }

    }


    /**
     * @brief Sets the output folder path
     * @param outputFolder The output folder path
     */
    void ArgusScraper::setOutputFolder(const std::string& outputFolder) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the folder exists
        if (fs::exists(outputFolder)) {
            _outputFolder = outputFolder;
            Logger::trace("[{}].setOutputFolder : {}", getFullId(), _outputFolder);
        } else {
            _outputFolder = DEFAULT_STR;
            Logger::debug("[{}].setOutputFolder : Folder {} does not exists", getFullId(), outputFolder);
        }

    }


    /**
     * @brief Set the link list
     * @param linkList The link list
     */
    void ArgusScraper::setLinkList(const std::vector<std::string> linkList) {
        _linkList = linkList;
    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Extract a link list from all Excel files available
     */
    void getAllLink() {

        // Using the namespace for a better lisibility
        namespace fs = std::filesystem;


        // Running through all files
        for (const auto& entry : fs::directory_iterator(dirPath))
        {
            const std::string filename = entry.path().filename().string();

            if (entry.is_regular_file() &&
                filename.size() > 11 &&
                filename.substr(filename.size() - 11) == "_ARGUS.xlsx")
            {
                files.push_back(entry.path().string());
            }
        }

    }


    /**
     * @brief Scarp all Html using the link list
     */
    void ArgusScraper::scrapAll() {

        // Running through all link list
        for (const std::string& link : _linkList) {
            client.get(link);
            _extractedHtml.push_back(client.getBody());
        }
        
    }


}