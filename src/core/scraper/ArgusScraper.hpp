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
#include "io/excelReader/ExcelReader.hpp"
#include "io/httpClient/HttpClient.hpp"
#include "io/htmlSaver/HtmlSaver.hpp"
#include <string>
#include <vector>


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
    
            // Excel entries related data
            std::string                 _inputFolder;       ///< Path to the input folder which contains all files
            std::string                 _outputFolder;      ///< Path to the ouput folder which will contains all saved files
            std::vector<std::string>    _fileList;          ///< Target file list


            // Scraping related data
            std::string                 _carBrand;          ///< The car targeted car brand
            std::string                 _carModel;          ///< The car targeted car model
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
             * @brief Shuffle a vector
             */
            template<typename T>
            void _shuffle(std::vector<T>& vector);





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


            /**
             * @brief Default Destructor.
             */
            ~ArgusScraper();




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


            /** @brief Gets the car brand
             *  @return The car brand
             */
            const std::string& getCarBrand() const { return _carBrand; }


            /** @brief Gets the car model
             *  @return The car model
             */
            const std::string& getCarModel() const { return _carModel; }


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
             * @note Checks if the directory exists
             */
            void setInputFolder(const std::string& inputFolder);


            /**
             * @brief Sets the output folder path
             * @param outputFolder The output folder path
             * @note Checks if the directory exists
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


            /**
             * @brief Set the car brand
             * @param carBrand The car brand
             */
            void setCarBrand(const std::string& carBrand);


            /**
             * @brief Set the car model
             * @param carModel The car model
             */
            void setCarModel(const std::string& carModel);





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


            /**
             * @brief Scrap a car model on the Argus website
             * @param startDate The start date which the scraping start after
             * @param endDate The end date which the scraping start before
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             */
            int scrapModel(const std::string& startDateStr, const std::string& endDateStr);

    };

}