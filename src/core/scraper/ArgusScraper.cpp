/**
 * @file ArgusScraper.cpp
 * 
 * @brief This file implements the ArgusScraper class, which represents a scraper for the Argus website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// Imports
#include "ArgusScraper.hpp"
#include "core/utils/Constant.hpp"
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <random>


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

        // Importing the list of already saved link
        _saver.importSavedLink();

    }


    /**
     * @brief Default destructor
     */
    ArgusScraper::~ArgusScraper() {

        // Exporting the list of already saved link
        _saver.exportSavedLink();

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
     * @brief Set the file list
     * @param fileList The file list
     * @todo Check if every file exists
     */
    void ArgusScraper::setFileList(const std::vector<std::string>& fileList) {
        _fileList = fileList;
    }


    /**
     * @brief Set the link list
     * @param linkList The link list
     * @todo Check if every link is valid
     */
    void ArgusScraper::setLinkList(const std::vector<std::string>& linkList) {
        _linkList = linkList;
    }





    // =========================================================================
    // Internal helpers
    // =========================================================================

    /**
     * @brief Shuffler the link list to randomize acces
     */
    void ArgusScraper::_shuffleLinkList() {
        auto rng = std::default_random_engine {};
        std::ranges::shuffle(_linkList, rng);
    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Extract a file list from the input folder
     */
    void ArgusScraper::getAllFile() {

        // Checking folder path
        if (_inputFolder == DEFAULT_STR) {
            Logger::error("[{}].getAllLink : File not set", getFullId());
            return;
        }


        // Getting namespace
        namespace fs = std::filesystem;


        // Running through all files
        for (const auto& entry : fs::directory_iterator(_inputFolder))
        {

            // Getting filename
            const std::string filename = entry.path().filename().string();


            // Checking file type and filename
            if (entry.is_regular_file() &&
                filename.size() > 11 &&
                filename.substr(filename.size() - 11) == "_ARGUS.xlsx")
            {
                _fileList.push_back(entry.path().string());
            }

        }

    }


    /**
     * @brief Extract a link list from the file list
     */
    void ArgusScraper::getAllLink() {

        // Checking the file list
        if (_fileList.size() == 0) {
            Logger::error("[{}].getAllLink : no file in the file list", getFullId());
            return;
        }


        // Debug
        Logger::trace("[{}].getAllLink : Starting", getFullId());


        // Going throug all file
        for (const std::string& file : _fileList) {
            if (_xlsxReader.setFilePath(file) && _xlsxReader.excelReadLinkList()) {
                std::vector<std::string> extractedLinkList = _xlsxReader.getLinkList();
                _linkList.insert(_linkList.end(), extractedLinkList.begin(), extractedLinkList.end());
            }
        }


        // Debug
        Logger::trace("[{}].getAllLink : got {} link", getFullId(), _linkList.size());

    }


    /**
     * @brief Scarp all Html using the link list
     */
    void ArgusScraper::scrapAllLink() {

        // Checking the file list
        if (_linkList.size() == 0) {
            Logger::error("[{}].scrapAllLink : no link in the link list", getFullId());
            return;
        }


        // Debug
        Logger::trace("[{}].scrapAllLink : Starting", getFullId());


        // Random link acces
        this->_shuffleLinkList();


        // Running through all link list
        for (const std::string& link : _linkList) {

            // Only getting unsaved link
            if (_saver.alreadySaved(link)) {
                Logger::trace("[{}].scrapAllLink : ignoring {}", getFullId(), link);
            } else {

                // Getting response and saving HTML if no error
                CarScraper::HttpResponse response = _client.get(link);
                if (response.statusCode == 200) {
                    _extractedHtml.push_back(response.body);
                }

                
                // Setting up the saver
                // _saver.setName(#); TODO : Scrap the name of the car and the date for the file name
                _saver.setLink(link);
                _saver.setContent(response.body);
                _saver.save();

            }

        }


        // Debug
        Logger::trace("[{}].scrapAllLink : got {} raw HTML", getFullId(), _extractedHtml.size());
        
    }


}