/**
 * @file HtmlSaver.cpp
 * 
 * @brief This file implements the HtmlSaver class, which represent an entity able to save raw html.
 *
 * @author Adrien GRAS
 * @date 2026-06-03
 */


// Imports
#include "HtmlSaver.hpp"
#include "core/logger/Logger.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <string>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default Constructor
     * @note Set every attributes to their default value
     */
    HtmlSaver::HtmlSaver() : Entity("HTML-SAVER") {
        _name           = DEFAULT_STR;
        _content        = DEFAULT_STR;
        _outputDir      = HTML_DIR;
        _linkFile       = LINK_FILE;
    }


    /**
     * @brief Constructor which initialize all attributes
     */
    HtmlSaver::HtmlSaver(   const std::string& name,
                            const std::string& content,
                            const std::string& link,
                            const std::string& outputDir,
                            const std::string& linkFile
                        ) : HtmlSaver() {

        // File details
        _name           = name;
        _content        = content;
        _link           = link;
        this->setOutputDir(outputDir);
        this->setLinkFile(linkFile);

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Set the output directory path
     * @param outputDir The output directory path
     * @note checks if the directory exists
     */
    void HtmlSaver::setOutputDir(const std::string& outputDir) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the directory exists
        if (fs::exists(outputDir)) {
            _outputDir = outputDir;
        } else {
            _outputDir = HTML_DIR;
            Logger::debug("[{}].setOutputDir : Directory {} does not exists", getFullId(), outputDir);
        }
        Logger::trace("[{}].setOutputDir : {}", getFullId(), _outputDir);


        // If needed, adding the final /
        if (_outputDir.back() != '/') {
            _outputDir.push_back('/');
        }
        
    }
    
    
    /**
     * @brief Set the link file
     * @param linkFile The link file path
     * @note checks if the file exists
    */
    void HtmlSaver::setLinkFile(const std::string& linkFile) {

        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the file exists
        if (fs::exists(linkFile)) {
            _linkFile = linkFile;
        } else {
            _linkFile = LINK_FILE;
            Logger::debug("[{}].setLinkFile : File {} does not exist", getFullId(), linkFile);
        }
        Logger::trace("[{}].setLinkFile : {}", getFullId(), _linkFile);
        
    }





    // =========================================================================
    // Core
    // =========================================================================

    /**
     * @brief Save the file
     * @return 0 for success, -1 for errors and 1 when the link has already been saved
     */
    int HtmlSaver::save() {

        // Security
        if (_link == DEFAULT_STR) {
            Logger::error("[{}].save() link not set", this->getFullId());
            return ERROR_CODE;
        }
        if (_savedLink.contains(_link)) {
            Logger::warn("[{}].save() HTML from {} already saved, action ignored", this->getFullId(), _link);
            return IGNORED_ACTION_CODE;
        }


        // Gettign and checking file path
        std::string effectiveName = _name;
        std::string path          = _outputDir + effectiveName + ".txt";
        int         nb_dp         = 1;
        while (std::filesystem::exists(path)) {
            effectiveName = "dp_" + _name + "_" + std::to_string(nb_dp);
            path          = _outputDir + effectiveName + ".txt";
            nb_dp++;
        }
        if (path != this->getFilePath()) {
            Logger::warn("[{}].save() filename collision on {}, saving as {}",
                this->getFullId(), this->getFilePath(), path);
        }


        // File opening and error handling
        std::ofstream file(path);
        if (!file.is_open()) {
            Logger::error("[{}].save() unable to open file", this->getFullId());
            return ERROR_CODE;
        }


        // File writting and close
        file << _content;
        Logger::trace("[{}].save() the file has been written and saved ({})", this->getFullId(), path);


        // Saving the link
        _savedLink.insert(_link);
        return SUCCESS_CODE;

    }


    /**
     * @brief Load the saved link from a JSON file in the _savedLink attributes
     * @return true or false wheter the link has been loaded or not
     */
    int HtmlSaver::importSavedLink() {
        
        // File opening and error handling
        std::ifstream file(_linkFile);
        if (!file.is_open()) {
            Logger::warn("[{}].importSavedLink() unable to open file {}", this->getFullId(), _linkFile);
            return ERROR_CODE;
        }


        // Getting file lines
        nlohmann::json jsonFile;
        file >> jsonFile;


        // Checking JSON format
        if (!jsonFile.is_array()) {
            Logger::error("[{}].importSavedLink() : File {} is not a JSON array", this->getFullId(), _linkFile);
            return ERROR_CODE;
        }


        // Getting all the saved link
        for (const auto& item : jsonFile) {

            // Cheking if the entry is a string
            if (!item.is_string()) {
                Logger::warn("[{}].importSavedLink() : non string entry ignored", this->getFullId(), _linkFile);
            } else {
                _savedLink.insert(item.get<std::string>());
            }

        }


        // Debug
        Logger::trace("[{}].importSavedLink() : import success", this->getFullId(), _linkFile);
        return SUCCESS_CODE;

    }


    /**
     * @brief Save the _savedLink attributes in a JSON file
     * @return true or false wheter the link has been saved or not
     */
    int HtmlSaver::exportSavedLink() {

        // First getting the lastest version of the saved link file
        this->importSavedLink();


        // Converting the saved link list into a JSON
        nlohmann::json savedLinkJSON =  nlohmann::json::array();
        for (const auto& link : _savedLink) {
            savedLinkJSON.push_back(link);
        }


        // File opening and error handling
        std::ofstream file(_linkFile);
        if (!file.is_open()) {
            Logger::error("[{}].exportSavedLink() unable to open file {}", this->getFullId(), _linkFile);
            return ERROR_CODE;
        }


        // File writting and close
        file << savedLinkJSON.dump(4);
        if (!file.good()) {
            Logger::error("[{}].exportSavedLink() error during writting", this->getFullId());
            return ERROR_CODE;
        }
        Logger::trace("[{}].exportSavedLink() the file has been written and saved ({})",
            this->getFullId(), _linkFile);
        return SUCCESS_CODE;

    }


    /**
     * @brief Checks if a link has been already saved or not
     * @param link The link
     * @return true or false wheter the link has been saved or not
     */
    bool HtmlSaver::alreadySaved(const std::string& link) const {
        return _savedLink.contains(link);
    }

} // namespace CarScraper