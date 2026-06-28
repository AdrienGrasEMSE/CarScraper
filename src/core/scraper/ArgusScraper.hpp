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
    
            // Scraping related data
            std::string                 _outputFolder;  ///< Path to the ouput folder which will contains all saved files
            std::string                 _carBrand;      ///< The car targeted car brand
            std::string                 _carModel;      ///< The car targeted car model


            // Internal object
            CarScraper::HttpClient      _client;        ///< HTTP Client
            CarScraper::HtmlSaver       _saver;         ///< HTML Saver





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

            /** @brief Gets the output folder path
             *  @return The output folder path
             */
            const std::string& getOutputFolder() const { return _outputFolder; }


            /** @brief Gets the car brand
             *  @return The car brand
             */
            const std::string& getCarBrand() const { return _carBrand; }


            /** @brief Gets the car model
             *  @return The car model
             */
            const std::string& getCarModel() const { return _carModel; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------


            /**
             * @brief Sets the output folder path
             * @param outputFolder The output folder path
             * @note Checks if the directory exists
             */
            void setOutputFolder(const std::string& outputFolder);


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