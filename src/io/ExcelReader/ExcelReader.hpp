/**
 * @file ExcelReader.hpp
 *
 * @brief Declares the ExcelReader class, which reads an Excel (.xlsx) file
 *        and extracts hyperlink URLs from a given column.
 *
 * @details
 * OpenXLSX does not expose hyperlinks stored inside the .xlsx ZIP archive.
 * This class bypasses that limitation by shelling out to `unzip -p` and
 * parsing the raw XML with libxml2.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include "core/logger/Logger.hpp"
#include <string>
#include <vector>
#include <map>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class ExcelReader
     * 
     * @brief Reads an Excel (.xlsx) file and exposes hyperlink URLs as a vector of strings.
     */
    class ExcelReader : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:
    
            // Data
            std::string                 _filePath;  ///< File Path
            std::vector<std::string>    _linkList;  ///< Extracted hyperlink URLs (populated by excelReadLinkList())





            // -------------------------------------------------------------------------
            // Internal helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Extracts a file entry from the .xlsx ZIP archive using `unzip -p`.
             * @param entryPath  Internal ZIP path (e.g. "xl/worksheets/sheet1.xml").
             * @return Raw XML content as a string, empty on failure.
             */
            std::string _extractZipEntry(const std::string& entryPath) const;


            /**
             * @brief Parses xl/worksheets/_rels/sheet1.xml.rels.
             * @return Map of relationship ID → target URL (hyperlinks only).
             */
            std::map<std::string, std::string> _parseRels() const;


            /**
             * @brief Parses the <hyperlinks> section of xl/worksheets/sheet1.xml.
             * @return Map of cell reference (e.g. "A2") → relationship ID.
             */
            std::map<std::string, std::string> _parseHyperlinkRefs() const;





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor : file path set to DEFAULT_STR.
             */
            ExcelReader();


            /**
             * @brief Constructor with path instanciation
             * @param filePath The file path
             */
            ExcelReader(const std::string& filePath);




            
            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /** @brief Gets the file path
             *  @return The file path
             */
            const std::string& getFilePath() const { return _filePath; }


            /** @brief Gets the link list extracted from the Excel file
             *  @return The link list
             */
            const std::vector<std::string>& getLinkList() const { return _linkList; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the file path
             * @param filePath The file path
             */
            void setFilePath(const std::string& filePath);


            


            // -------------------------------------------------------------------------
            // Public helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Converts an Excel column letter string to a 0-based index.
             * @param col  Column letters (e.g. "A", "B", "AA").
             * @return 0-based column index.
             */
            static int _colLetterToIndex(const std::string& col);


            /**
             * @brief Splits a cell reference such as "A3" into {"A", "3"}.
             * @param cellRef  Full cell reference string.
             * @return Pair {colLetters, rowDigits}.
             */
            static std::pair<std::string, std::string> _splitCellRef(const std::string& cellRef);





            // -------------------------------------------------------------------------
            // Excel Reading Methods
            // -------------------------------------------------------------------------

            /**
             * @brief Extracts all hyperlink URLs from column 1 (column A) of the first sheet.
             *
             * @details
             * Populates _linkList with one entry per data row (header row skipped).
             * Cells with no hyperlink produce an empty string in the list.
             * The list is cleared and rebuilt on every call.
             */
            void excelReadLinkList();


            /**
             * @brief Get rid of all invalid link in the list
             */
            void cleanLinkList();
            
    };

}