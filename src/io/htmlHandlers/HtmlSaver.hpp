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
#include "core/utils/Constant.hpp"
#include <string>
#include <unordered_set>


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

            std::string                     _name;          ///< File Name                      | ex : "SaveHtml1"
            std::string                     _content;       ///< HTML Content                   | ex : "File which contains a very interesting content..."
            std::string                     _link;          ///< Link to the HTML               | ex : "https://example.com/"
            std::string                     _outputDir;     ///< Output Directory Path          | ex : "/data/html/"
            std::string                     _linkFile;      ///< Link File Path                 | ex : "/data/saved_link.json"
            std::unordered_set<std::string> _savedLink;     ///< All link to already saved HTML | ex : {"https://example.com/", "https://example.net/"}





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor
             * @note Set every attributes to their default value
             */
            HtmlSaver();


            /**
             * @brief Constructor which initialize all attributes
             * @param name The name of the file
             * @param content The content of the HTML
             * @param link The link of the HTML
             * @param outputDir The output directory path
             * @param outputLinkDir The input directory path for the saved link
             * @param outputLinkDir The output directory path for the saved link
             */
            HtmlSaver(  const std::string& name,
                        const std::string& content,
                        const std::string& link,
                        const std::string& outputDir        = HTML_DIR,
                        const std::string& linkFile         = LINK_FILE
                    );





            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /**
             * @brief Gets the name of the file
             * @return The name of the file
             */
            const std::string& getName() const { return _name; }


            /**
             * @brief Gets the content of the HTML
             * @return The content of the HTML
             */
            const std::string& getContent() const { return _content; }


            /**
             * @brief Gets the link of the HTML
             * @return The link of the HTML
             */
            const std::string& getLink() const { return _link; }


            /**
             * @brief Gets the output directory path
             * @return The output directory path
             */
            const std::string& getOutputDir() const { return _outputDir; }


            /**
             * @brief Gets the link file path
             * @return The link file path
             */
            const std::string& getLinkFile() const { return _linkFile; }


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
             * @brief Set the content of the HTML
             * @param content The content of the HTML
             */
            void setContent(const std::string& content) { _content = content; }


            /**
             * @brief Set the link of the HTML
             * @param link The link of the HTML
             */
            void setLink(const std::string& link) { _link = link; }


            /**
             * @brief Set the output directory path
             * @param outputDir The output directory path
             * @note checks if the directory exists
             */
            void setOutputDir(const std::string& outputDir);


            /**
             * @brief Set the link file
             * @param linkFile The link file path
             * @note checks if the file exists
             */
            void setLinkFile(const std::string& linkFile);





            // -------------------------------------------------------------------------
            // Core
            // -------------------------------------------------------------------------

            /**
             * @brief Save the file
             * @return 0 for success, -1 for errors and 1 when the link has already been saved
             */
            int save();


            /**
             * @brief Load the saved link from a JSON file in the _savedLink attributes
             * @return true or false wheter the link has been loaded or not
             */
            int importSavedLink();


            /**
             * @brief Save the _savedLink attributes in a JSON file
             * @return true or false wheter the link has been saved or not
             */
            int exportSavedLink();


            /**
             * @brief Checks if a link has been already saved or not
             * @param link The link
             * @return true or false wheter the link has been saved or not
             */
            bool alreadySaved(const std::string& link) const;

    };

} // namespace CarScraper