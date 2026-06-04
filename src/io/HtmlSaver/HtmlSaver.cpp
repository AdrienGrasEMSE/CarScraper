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
#include <fstream>
#include <filesystem>


/**
 * Namespace CarScraper
 */
namespace CarScraper {


    /**
     * Define fs as the namespace std::filesystem
     */
    namespace fs = std::filesystem;

    // =========================================================================
    // Private data and methods
    // =========================================================================
    HtmlSaver::HtmlSaver(   const std::string& name,
                            const std::string& content,
                            const std::string& outputDir) : Entity("HTML-SAVER") {

            // File details
            _name       = name;
            _content    = content;
            _outputDir  = outputDir;

    }





    // =========================================================================
    // Core
    // =========================================================================

    


    /**
     * @brief Save the file
     * @return The full path written
     */
    std::string HtmlSaver::save() const {
        
        // Ensure the output directory exists
        fs::create_directories(_outputDir);
        const std::string path = getFilePath();


        // File opening and error handling
        std::ofstream file(path);
        if (!file.is_open()) {
            Logger::error("[{}].save() unable to open file", this->getFullId());
            throw std::runtime_error("HtmlSaver: cannot open file for writing: " + path);
        }


        // File writting and close
        file << _content;
        file.close();
        Logger::trace("[{}].save() the file has been written and saved ({})", this->getFullId(), path);


        // Return the complete file path
        return path;

    }


    /**
     * @return The path
     */
    std::string HtmlSaver::getFilePath() const {
        return _outputDir + _name + ".txt";
    }

} // namespace CarScraper