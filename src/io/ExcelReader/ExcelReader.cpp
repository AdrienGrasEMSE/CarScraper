/**
 * @file ExcelReader.cpp
 *
 * @brief Implements the ExcelReader class.
 *
 * @details
 * A .xlsx file is a ZIP archive. Hyperlinks are NOT stored as cell values —
 * they live in two XML files inside the archive:
 *
 *   xl/worksheets/_rels/sheet1.xml.rels   →  rId  →  target URL
 *   xl/worksheets/sheet1.xml              →  <hyperlinks> section : cellRef → rId
 *
 * OpenXLSX does not expose these, so we extract and parse them directly:
 *   1. Shell out to `unzip -p` to dump each XML entry to stdout.
 *   2. Parse the XML with libxml2 + XPath.
 *   3. Cross-reference to obtain cellRef → URL.
 *   4. Keep only column A (index 0), skip header row 1.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */
 
 
// Imports
#include "ExcelReader.hpp"
#include "core/utils/Constant.hpp"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <zip.h>
#include <cctype>
#include <stdexcept>
#include <regex>
#include <filesystem>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor : file path set to DEFAULT_STR.
     */
    ExcelReader::ExcelReader() : Entity("EXCEL_READER") {
        _filePath   = DEFAULT_STR;
    }


    /**
     * @brief Constructor with path instanciation
     * @param filePath The file path
     */
    ExcelReader::ExcelReader(const std::string& filePath) : ExcelReader() {
        _filePath = filePath;
    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the file path
     * @param filePath The file path
     * @note Check if the file exists
     */
    void ExcelReader::setFilePath(const std::string& filePath) {

        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the file exists
        if (fs::exists(filePath)) {
            _filePath = filePath;
            Logger::trace("[{}].setFilePath : {}", getFullId(), _filePath);
        } else {
            _filePath = DEFAULT_STR;
            Logger::debug("[{}].setFilePath : File {} does not exists", getFullId(), _filePath);
        }

    }





    // =========================================================================
    // Private helpers
    // =========================================================================

    /**
     * @brief Extracts a file entry from the .xlsx ZIP archive using libzip.
     * @param entryPath  Internal ZIP path (e.g. "xl/worksheets/sheet1.xml").
     * @return Raw XML content as a string, empty string on failure.
     */
    std::string ExcelReader::_extractZipEntry(const std::string& entryPath) const {

        // Checking file
        if (_filePath == DEFAULT_STR) {
            Logger::debug("[{}]._extractZipEntry : File not set", getFullId());
        }

    
        // Opening the whole archive
        int zipError = 0;
        zip_t* archive = zip_open(_filePath.c_str(), ZIP_RDONLY, &zipError);
        if (!archive) {

            // Getting the error
            zip_error_t ze;
            zip_error_init_with_code(&ze, zipError);
            Logger::error("[{}]._extractZipEntry : zip_open failed — {}", getFullId(), zip_error_strerror(&ze));


            // Returning empty string
            zip_error_fini(&ze);
            return {};

        }
    

        // Opening the targeted file
        zip_file_t* file = zip_fopen(archive, entryPath.c_str(), 0);
        if (!file) {
            Logger::error("[{}]._extractZipEntry : entry '{}' not found in archive", getFullId(), entryPath);
            zip_close(archive);
            return {};
        }
    

        // Getting the content
        std::string result;
        char buf[4096];
        zip_int64_t bytesRead = 0;
        while ((bytesRead = zip_fread(file, buf, sizeof(buf))) > 0) {
            result.append(buf, static_cast<size_t>(bytesRead));
        }
    

        // Closing everything
        zip_fclose(file);
        zip_close(archive);
        return result;

    }



    /**
     * @brief Parses xl/worksheets/_rels/sheet1.xml.rels.
     * @return Map of relationship ID → target URL (hyperlinks only).
     */
    std::map<std::string, std::string> ExcelReader::_parseRels() const {

        // Mapping
        std::map<std::string, std::string> rIdToUrl;


        // XML extraction from the zip file
        std::string xml = _extractZipEntry("xl/worksheets/_rels/sheet1.xml.rels");
        if (xml.empty()) {
            Logger::warn("[{}]._parseRels : .rels file is empty or missing", getFullId());
            return rIdToUrl;
        }


        // XML Parsing
        xmlDocPtr doc = xmlParseMemory(xml.c_str(), static_cast<int>(xml.size()));
        if (!doc) {
            Logger::error("[{}]._parseRels : failed to parse .rels XML", getFullId());
            return rIdToUrl;
        }


        // Getting xpath schemes
        xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
        xmlXPathRegisterNs(
            ctx,
            BAD_CAST "r",
            BAD_CAST "http://schemas.openxmlformats.org/package/2006/relationships"
        );


        // Getting specifically all the relationship between Type, Id and Target
        xmlXPathObjectPtr obj = xmlXPathEvalExpression(BAD_CAST "//r:Relationship[@Type and @Id and @Target]", ctx);


        // If the relationship list exist
        if (obj && obj->nodesetval) {

            // Running through all
            for (int i = 0; i < obj->nodesetval->nodeNr; ++i) {

                // Current relationship
                xmlNodePtr node = obj->nodesetval->nodeTab[i];


                // Getting elements according to the xpath schemes
                xmlChar* id     = xmlGetProp(node, BAD_CAST "Id");
                xmlChar* target = xmlGetProp(node, BAD_CAST "Target");
                xmlChar* type   = xmlGetProp(node, BAD_CAST "Type");


                // Adding the relationship if existing
                if (id && target && type) {
                    std::string typeStr(reinterpret_cast<char*>(type));
                    if (typeStr.find("hyperlink") != std::string::npos) {
                        rIdToUrl[reinterpret_cast<char*>(id)] = reinterpret_cast<char*>(target);
                    }
                }


                // Free items
                if (id)     xmlFree(id);
                if (target) xmlFree(target);
                if (type)   xmlFree(type);

            }

        }

        if (obj)  xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);

        Logger::debug("[{}]._parseRels : {} hyperlink relationship(s) found", getFullId(), rIdToUrl.size());
        return rIdToUrl;
    }


    
    /**
     * @brief Parses the <hyperlinks> section of xl/worksheets/sheet1.xml.
     * @return Map of cell reference (e.g. "A2") → relationship ID.
     */
    std::map<std::string, std::string> ExcelReader::_parseHyperlinkRefs() const {

        // Cell mapping
        std::map<std::string, std::string> cellToRId;


        // XML extraction from the zip file
        std::string xml = _extractZipEntry("xl/worksheets/sheet1.xml");
        if (xml.empty()) {
            Logger::error("[{}]._parseHyperlinkRefs : sheet1.xml is empty or missing", getFullId());
            return cellToRId;
        }


        // XML Parsing
        xmlDocPtr doc = xmlParseMemory(xml.c_str(), static_cast<int>(xml.size()));
        if (!doc) {
            Logger::error("[{}]._parseHyperlinkRefs : failed to parse sheet1.xml", getFullId());
            return cellToRId;
        }


        // Getting the xpath schemes
        xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
        xmlXPathRegisterNs(
            ctx,
            BAD_CAST "ss",
            BAD_CAST "http://schemas.openxmlformats.org/spreadsheetml/2006/main"
        );


        // The r:id attribute lives in the office relationships namespace
        xmlXPathRegisterNs(
            ctx,
            BAD_CAST "r",
            BAD_CAST "http://schemas.openxmlformats.org/officeDocument/2006/relationships"
        );


        // Getting all hyperlink
        xmlXPathObjectPtr obj = xmlXPathEvalExpression(BAD_CAST "//ss:hyperlink[@ref]", ctx);


        // If the hyperlink list exist
        if (obj && obj->nodesetval) {

            // Running through all
            for (int i = 0; i < obj->nodesetval->nodeNr; ++i) {

                // Current hyperlink
                xmlNodePtr node = obj->nodesetval->nodeTab[i];


                // Getting the hyperlink
                xmlChar* ref = xmlGetProp(node, BAD_CAST "ref");


                // The r:id attribute is namespace-qualified; try both ways.
                xmlChar* rid = xmlGetProp(node, BAD_CAST "id");
                if (!rid) {

                    // Fallback: walk attributes and match by local name
                    for (xmlAttrPtr attr = node->properties; attr; attr = attr->next) {
                        if (xmlStrcmp(attr->name, BAD_CAST "id") == 0) {
                            rid = xmlNodeGetContent(attr->children);
                            break;
                        }
                    }

                }


                // Adding the mapping
                if (ref && rid) {
                    cellToRId[reinterpret_cast<char*>(ref)] =
                        reinterpret_cast<char*>(rid);
                }


                // Free items
                if (ref) xmlFree(ref);
                if (rid) xmlFree(rid);

            }

        }


        // Free opbject
        if (obj) {xmlXPathFreeObject(obj);}
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);


        // Returning the mapping
        Logger::debug("[{}]._parseHyperlinkRefs : {} hyperlink ref(s) found in sheet", getFullId(), cellToRId.size());
        return cellToRId;

    }





    // =========================================================================
    // Public helpers
    // =========================================================================



    /**
     * @brief Converts an Excel column letter string to a 0-based index.
     *        "A" → 0, "B" → 1, "Z" → 25, "AA" → 26, …
     */
    int ExcelReader::_colLetterToIndex(const std::string& col) {

        // Calculating index
        int result = 0;
        for (char c : col) {
            result = result * 26 + (std::toupper(static_cast<unsigned char>(c)) - 'A' + 1);
        }

        // 0-based
        return result - 1;

    }


    /**
     * @brief Splits a cell reference (e.g. "A3") into its column letters and row digits.
     * @return Pair {colLetters, rowDigits}  →  {"A", "3"}
     */
    std::pair<std::string, std::string> ExcelReader::_splitCellRef(const std::string& cellRef) {

        // Running through the cell ref
        std::string letters, digits;
        for (char c : cellRef) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                letters += c;
            }
            else {
                digits  += c;
            }
        }
        return {letters, digits};

    }





    // =========================================================================
    // Excel Reading Methods
    // =========================================================================

    /**
     * @brief Extract a list of link to technical document
     */
    void ExcelReader::excelReadLinkList() {

        // Debug
        Logger::trace("[{}].excelReadLinkList : Start on \"{}\"", getFullId(), _filePath);


        // Checking file
        if (_filePath == DEFAULT_STR) {
            Logger::debug("[{}]._extractZipEntry : File not set", getFullId());
        }
        

        _linkList.clear();


        // --- Step 1 : rId → URL --------------------------------------------------
        auto rIdToUrl = _parseRels();
        if (rIdToUrl.empty()) {
            Logger::warn("[{}].excelReadLinkList : no hyperlink relationships found — aborting", getFullId());
            return;
        }


        // --- Step 2 : cellRef → rId  (column A only) -----------------------------
        auto cellToRId = _parseHyperlinkRefs();

        // Build  row → URL  for column A (0-based index 0)
        std::map<int, std::string> rowToUrl;
        for (auto& [ref, rid] : cellToRId) {

            // Getting cell infos
            auto [colLetters, rowStr]   = _splitCellRef(ref);
            int colIdx                  = _colLetterToIndex(colLetters);
            int row                     = std::stoi(rowStr);


            // Inspecting only the column A
            if (colIdx == 0) {
                auto it = rIdToUrl.find(rid);
                if (it != rIdToUrl.end()) {
                    rowToUrl[row] = it->second;
                }
            }
        }


        // --- Step 3 : fill _linkList in row order, skip header row 1 -------------
        if (rowToUrl.empty()) {
            Logger::warn("[{}].excelReadLinkList : no hyperlinks found in column A", getFullId());
            return;
        }


        // Filling up the link list
        int maxRow = rowToUrl.rbegin()->first;
        for (int r = 2; r <= maxRow; ++r) {     // row 1 = header → start at 2
            auto it = rowToUrl.find(r);
            _linkList.push_back(it != rowToUrl.end() ? it->second : "");
        }


        // Debug
        Logger::trace("[{}].excelReadLinkList : End on \"{}\"", getFullId(), _filePath);

    }


    /**
     * @brief Get rid of all invalid link in the list
     */
    void ExcelReader::cleanLinkList() {

        // Regex to verify link shape
        const std::regex urlRegex(R"(^https://.+)");


        // Getting the size before erasing invalid link
        const int sizeBefore = static_cast<int>(_linkList.size());


        // Deleting invalid link
        _linkList.erase(
            std::remove_if(_linkList.begin(), _linkList.end(), [&](const std::string& link) {
                return !std::regex_match(link, urlRegex);
            }),
            _linkList.end()
        );


        // Debug
        Logger::trace("[{}].cleanLinkList : {} link erased",
            getFullId(), sizeBefore - static_cast<int>(_linkList.size()));

    }

}