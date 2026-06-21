/**
 * @file HtmlParser.hpp
 * 
 * @brief This file declares the HtmlParser class, which represents an HTML parser.
 *
 * @author Adrien GRAS
 * @date 2026-06-04
 */


// For only one include
#pragma once


// Imports
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
    * @brief A class for parsing HTML content and extracting structured data.
    */
    class HtmlParser {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            htmlDocPtr  _doc;       ///< Parsed HTML document pointer
            std::string _rawHtml;   ///< Original raw HTML content (for debugging)





            // -------------------------------------------------------------------------
            // Internal helpers
            // -------------------------------------------------------------------------

            /**
            * @brief Evaluates an XPath expression and returns the result.
            * @param xpath The XPath expression to evaluate.
            * @return The result of the XPath evaluation.
            */
            xmlXPathObjectPtr _evalXPath(const std::string& xpath) const;


            /**
            * @brief Extracts the text content of an XML node.
            * @param node The XML node to extract text from.
            * @return The text content of the node.
            */
            std::string _nodeText(xmlNodePtr node) const;


            /**
            * @brief Trims whitespace from a string.
            * @param stringValue The string to trim.
            * @return The trimmed string.
            */
            std::string _trimWhitespace(const std::string& stringValue) const;


            /**
            * @brief Converts a table node into a vector of maps representing its rows.
            * @param tableNode The XML node representing the table.
            * @return Vector of maps containing the table data.
            */
            std::vector<std::map<std::string, std::string>> _tableNodeToMap(xmlNodePtr tableNode) const;





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
            * @brief Constructs an HtmlParser instance with the given HTML content.
            * @param html The raw HTML content to parse.
            */
            explicit HtmlParser(const std::string& html);


            /**
            * @brief Destructor for the HtmlParser instance.
            */
            ~HtmlParser();


            /**
            * @brief Deleted copy constructor to prevent copying of HtmlParser instances.
            * @param other The HtmlParser instance to copy from (not used).
            */
            HtmlParser(const HtmlParser&) = delete;


            /**
            * @brief Deleted copy assignment operator to prevent copying of HtmlParser instances.
            * @param other The HtmlParser instance to copy from (not used).
            * @return Reference to the current instance.
            */
            HtmlParser& operator=(const HtmlParser&) = delete;


            /**
            * @brief Moves the resources from another HtmlParser instance.
            * @param other The HtmlParser instance to move from.
            * @return Reference to the current instance.
            */
            HtmlParser(HtmlParser&& other) noexcept;


            /**
            * @brief Moves the resources from another HtmlParser instance.
            * @param other The HtmlParser instance to move from.
            * @return Reference to the current instance.
            */
            HtmlParser& operator=(HtmlParser&& other) noexcept;





            // -------------------------------------------------------------------------
            // State
            // -------------------------------------------------------------------------

            /**
            * @brief Checks if the HTML content was successfully parsed.
            * @return True if the HTML is loaded, false otherwise.
            */
            bool isLoaded() const { return _doc != nullptr; }





            // -------------------------------------------------------------------------
            // Text extraction by tag or XPath
            // -------------------------------------------------------------------------

            /**
            * @brief Returns the inner text of the first node matching the given XPath expression.
            * @param xpath XPath expression to select the node.
            * @return Optional string containing the inner text, or std::nullopt if not found.
            */
            std::optional<std::string> getText(const std::string& xpath) const;


            /**
            * @brief Returns the inner texts of all nodes matching the given XPath expression.
            * @param xpath XPath expression to select nodes.
            * @return Vector of strings containing the inner texts.
            */
            std::vector<std::string> getAllTexts(const std::string& xpath) const;


            /**
            * @brief Returns the inner text of the first <hN> tag (level 1–6).
            * @param level Heading level (1–6).
            * @return Optional string containing the heading text, or std::nullopt if not found.
            */
            std::optional<std::string> getHeading(int level) const;


            /**
            * @brief Returns the inner texts of all <hN> tags at the given level.
            * @param level Heading level (1–6).
            * @return Vector of strings containing the heading texts.
            */
            std::vector<std::string> getAllHeadings(int level) const;





            // -------------------------------------------------------------------------
            // Attribute extraction
            // -------------------------------------------------------------------------

            /**
            * @brief Returns the value of `attribute` on the first node matching `xpath`.
            * @param xpath XPath expression to select the node.
            * @param attribute Name of the attribute to retrieve.
            * @return Optional string containing the attribute value, or std::nullopt if not found.
            */
            std::optional<std::string> getAttribute(const std::string& xpath,
                                                    const std::string& attribute) const;





            // -------------------------------------------------------------------------
            // Table extraction
            // -------------------------------------------------------------------------

            /**
            * @brief Returns the first <table> matching `xpath` as a list of row-maps.
            * @param xpath XPath expression to select the table (default: "//table")
            * @return Vector of row-maps representing the table rows.
            * @note Each row-map is { column_header -> cell_value }.
            */
            std::vector<std::map<std::string, std::string>> getTableAsMap(const std::string& xpath = "//table") const;


            /**
            * @brief Returns ALL <table> nodes matching `xpath`, each converted to a row-map list.
            * @param xpath XPath expression to select tables (default: "//table")
            * @return Vector of tables, where each table is a vector of row-maps.
            * @note Each row-map is { column_header -> cell_value }.
            */
            std::vector<std::vector<std::map<std::string, std::string>>> getAllTablesAsMap(const std::string& xpath = "//table") const;

    };

} // namespace CarScraper
