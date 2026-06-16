/**
 * @file HtmlSaver.hpp
 * 
 * @brief This file declares the HtmlSaver class, which represent an entity able to save raw html.
 *
 * @author Adrien GRAS
 * @date 2026-06-03
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include <string>
#include <stdexcept>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /*
    * @class HtmlSaver
    * 
    * @brief Persists raw HTML content to disk under data/save_html/.
    */
    class HtmlSaver : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            std::string _name;      ///< File Name          | ex : "SaveHtml1"
            std::string _content;   ///< File Content       | ex : "File which contains a very interesting content..."
            std::string _outputDir; ///< Output Folder Path | ex : "/data/save_html/"





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor
             * @note Set every attributes to DEFAULT_STR
             */
            HtmlSaver();


            /**
             * @brief Constructor which initialize all attributes
             */
            HtmlSaver(  const std::string& name,
                        const std::string& content,
                        const std::string& outputDir = "data/save_html/");


            /**
             * @brief Default destructor
             */
            ~HtmlSaver() = default;





            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /**
             * @brief Gets the name of the file
             * @return The name of the file
             */
            const std::string& getName() const { return _name; }


            /**
             * @brief Gets the content of the file
             * @return The content of the file
             */
            const std::string& getContent() const { return _content;   }


            /**
             * @brief Gets the output folder path
             * @return The output folder path
             */
            const std::string& getOutputDir() const { return _outputDir; }


            /**
             * @brief Gets the full file path
             * @return The full path
             */
            const std::string getFilePath() const { return _outputDir + _name + ".txt"; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------

            /**
             * @brief Set the name of the file
             * @param name The name of the file
             */
            void setName(const std::string& name) { _name = name; }


            /**
             * @brief Set the content of the file
             * @param content The content of the file
             */
            void setContent(const std::string& content) { _content = content; }


            /**
             * @brief Set the output folder path
             * @param outputDir The output folder path
             */
            void setOutputDir(const std::string& outputDir) { _outputDir = outputDir; }





            // -------------------------------------------------------------------------
            // Core
            // -------------------------------------------------------------------------

            /**
             * @brief Save the file
             * @return The full path written
             */
            std::string save() const;

    };

} // namespace CarScraper