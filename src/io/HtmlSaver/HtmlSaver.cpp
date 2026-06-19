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
        _inputLinkDir   = LINK_DIR;
        _outputLinkDir  = LINK_DIR;
    }


    /**
     * @brief Constructor which initialize all attributes
     */
    HtmlSaver::HtmlSaver(   const std::string& name,
                            const std::string& content,
                            const std::string& link,
                            const std::string& outputDir,
                            const std::string& inputLinkDir,
                            const std::string& outputLinkDir
                        ) : HtmlSaver() {

        // File details
        _name           = name;
        _content        = content;
        _link           = link;
        this->setOutputDir(outputDir);
        this->setInputLinkDir(inputLinkDir);
        this->setOutputLinkDir(outputLinkDir);

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
     * @brief Set the input directory path for the saved link
     * @param inputLinkDir The input directory path for the saved link
     * @note checks if the directory exists
    */
    void HtmlSaver::setInputLinkDir(const std::string& inputLinkDir) {

        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the directory exists
        if (fs::exists(inputLinkDir)) {
            _inputLinkDir = inputLinkDir;
        } else {
            _inputLinkDir = LINK_DIR;
            Logger::debug("[{}].setInputLinkDir : Directory {} does not exists", getFullId(), inputLinkDir);
        }
        Logger::trace("[{}].setInputLinkDir : {}", getFullId(), _inputLinkDir);


        // If needed, adding the final /
        if (_inputLinkDir.back() != '/') {
            _inputLinkDir.push_back('/');
        }
        
    }


    /**
     * @brief Set the output directory path for the saved link
     * @param outputLinkDir The output directory path for the saved link
     * @note checks if the directory exists
    */
    void HtmlSaver::setOutputLinkDir(const std::string& outputLinkDir) {

        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the directory exists
        if (fs::exists(outputLinkDir)) {
            _outputLinkDir = outputLinkDir;
        } else {
            _outputLinkDir = LINK_DIR;
            Logger::debug("[{}].setOutputLinkDir : Directory {} does not exists", getFullId(), outputLinkDir);
        }
        Logger::trace("[{}].setOutputLinkDir : {}", getFullId(), _outputLinkDir);


        // If needed, adding the final /
        if (_outputLinkDir.back() != '/') {
            _outputLinkDir.push_back('/');
        }
        
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


        // Getting namespace
        namespace fs = std::filesystem;
        

        // Getting file path
        const std::string path = this->getFilePath();


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
        
        // Create file path
        const std::string path = _inputLinkDir + "saved_link.json";


        // File opening and error handling
        std::ifstream file(path);
        if (!file.is_open()) {
            Logger::error("[{}].importSavedLink() unable to open file {}", this->getFullId(), path);
            return ERROR_CODE;
        }


        // Getting file lines
        nlohmann::json jsonFile;
        file >> jsonFile;


        // Checking JSON format
        if (!jsonFile.is_array()) {
            Logger::error("[{}].importSavedLink() : File {} is not a JSON array", this->getFullId(), path);
            return ERROR_CODE;
        }


        // Getting all the saved link
        for (const auto& item : jsonFile) {

            // Cheking if the entry is a string
            if (!item.is_string()) {
                Logger::warn("[{}].importSavedLink() : non string entry ignored", this->getFullId(), path);
            } else {
                _savedLink.insert(item.get<std::string>());
            }

        }


        // Debug
        Logger::trace("[{}].importSavedLink() : import success", this->getFullId(), path);
        return SUCCESS_CODE;

    }


    /**
     * @brief Save the _savedLink attributes in a JSON file
     * @return true or false wheter the link has been saved or not
     */
    int HtmlSaver::exportSavedLink() const {

        // Converting the saved link list into a JSON
        nlohmann::json savedLinkJSON =  nlohmann::json::array();
        for (const auto& link : _savedLink) {
            savedLinkJSON.push_back(link);
        }


        // Create file path
        const std::string path = _outputLinkDir + "saved_link.json";


        // File opening and error handling
        std::ofstream file(path);
        if (!file.is_open()) {
            Logger::error("[{}].exportSavedLink() unable to open file {}", this->getFullId(), path);
            return ERROR_CODE;
        }


        // File writting and close
        file << savedLinkJSON.dump(4);
        if (!file.good()) {
            Logger::error("[{}].exportSavedLink() error during writting", this->getFullId());
            return ERROR_CODE;
        }
        Logger::trace("[{}].exportSavedLink() the file has been written and saved ({})", this->getFullId(), path);
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