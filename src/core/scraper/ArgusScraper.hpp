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
#include "io/excelReader/ExcelReader.hpp"
#include "io/httpClient/HttpClient.hpp"
#include "io/htmlSaver/HtmlSaver.hpp"
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
            std::vector<std::string>    _fileList;          ///< Target file list
            std::vector<std::string>    _linkList;          ///< Target link list
            std::vector<std::string>    _extractedHtml;     ///< All HTML extracted


            // Internal object
            CarScraper::ExcelReader     _xlsxReader;        ///< Excel Reader
            CarScraper::HttpClient      _client;            ///< HTTP Client
            CarScraper::HtmlSaver       _saver;             ///< HTML Saver





            // -------------------------------------------------------------------------
            // Internal Helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Shuffler the link list to randomize acces
             */
            void _shuffleLinkList();





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


            /** @brief Gets the file list
             *  @return The file list
             */
            const std::vector<std::string>& getFileList() const { return _fileList; }


            /** @brief Gets the link list
             *  @return The link list
             */
            const std::vector<std::string>& getLinkList() const { return _linkList; }


            /** @brief Gets the saved html list
             *  @return The saved html list
             */
            const std::vector<std::string>& getSavedHtml() const { return _extractedHtml; }





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
             * @brief Set the file list
             * @param fileList The file list
             */
            void setFileList(const std::vector<std::string>& fileList);


            /**
             * @brief Set the link list
             * @param linkList The link list
             */
            void setLinkList(const std::vector<std::string>& linkList);





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Extract a file list from the input folder
             */
            void getAllFile();


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
            //void saveAllHtml();

    };

}