/**
 * @file ArgusScraper.hpp
 * 
 * @brief This file declares the ArgusScraper class, which represents a scraper for the Argus website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include "core/logger/Logger.hpp"
#include "io/ExcelReader/ExcelReader.hpp"
#include "io/HttpClient/HttpClient.hpp"
#include "io/HtmlSaver/HtmlSaver.hpp"
#include <string>
#include <vector>
#include <map>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class ArgusScraper
     * 
     * @brief Scrap the Argus website.
     */
    class ArgusScraper : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:
    
            // Data
            std::string                 _inputFolder;       ///< Path to the input folder which contains all files
            std::string                 _outputFolder;      ///< Path to the ouput folder which will contains all saved files
            std::vector<std::string>    _linkList;          ///< Target link list
            std::vector<std::string>    _extractedHtml;     ///< All HTML extracted


            // Internal object
            CarScraper::ExcelReader     _xlsxReader;        ///< Excel Reader
            CarScraper::HttpClient      _client;            ///< HTTP Client
            CarScraper::HtmlSaver       _saver;             ///< HTML Saver





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor.
             */
            ArgusScraper();




            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /** @brief Gets the input folder path
             *  @return The input folder path
             */
            const std::string& getInputFolder() const { return _inputFolder; }


            /** @brief Gets the output folder path
             *  @return The output folder path
             */
            const std::string& getOutputFolder() const { return _outputFolder; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------


            /**
             * @brief Sets the input folder path
             * @param inputFolder The input folder path
             */
            void setInputFolder(const std::string& inputFolder);


            /**
             * @brief Sets the output folder path
             * @param outputFolder The output folder path
             */
            void setOutputFolder(const std::string& outputFolder);


            /**
             * @brief Set the link list
             * @param linkList The link list
             */
            void setLinkList(const std::vector<std::string> linkList);





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Extract a link list from all Excel files available
             */
            void getAllLink();


            /**
             * @brief Scrap all HTML using the link list
             */
            void scrapAllLink();


            /**
             * @brief Save all HTML
             */
            void saveAllHtml();

    };

}