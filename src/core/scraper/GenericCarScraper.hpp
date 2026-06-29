/**
 * @file GenericCarScraper.hpp
 * 
 * @brief This file declares a generic CarScraper class, which represents a car oriented scraper.
 *
 * @author Adrien GRAS
 * @date 2026-06-28
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
     * @class GenericCarScraper
     * 
     * @brief Base for website-specific scrapers.
     */
    class GenericCarScraper : public Entity {

        // =========================================================================
        // Protected data and methods
        // =========================================================================
        protected:
    
            // Scraping related data
            std::string                 _outputDirectory;   ///< Path to the ouput directory which will contains all saved files
            std::string                 _carBrand;          ///< The car targeted car brand
            std::string                 _carModel;          ///< The car targeted car model


            // Internal object
            CarScraper::HttpClient      _client;            ///< HTTP Client
            CarScraper::HtmlSaver       _saver;             ///< HTML Saver




            
            // -------------------------------------------------------------------------
            // Constructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor : define prefix as SCRAPER
             */
            GenericCarScraper();


            /**
             * @brief Constructor which define the scraper PREFIX
             * @param prefix The prefix for the full id
             */
            GenericCarScraper(const std::string& prefix);





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
            // Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Destructor.
             */
            ~GenericCarScraper();




            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /** @brief Gets the output directory path
             *  @return The output directory path
             */
            const std::string& getOutputDirectory() const { return _outputDirectory; }


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
             * @brief Sets the output directory path
             * @param outputDirectory The output directory path
             * @note Checks if the directory exists
             */
            void setOutputDirectory(const std::string& outputDirectory);


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
             * @brief Scrap a car model on the targeted website
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note Generic method to implement
             */
            virtual int scrapModel();

    };

}