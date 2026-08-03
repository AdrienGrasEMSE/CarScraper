/**
 * @file HtmlParser.cpp
 *
 * @brief This file implements the HtmlParser class, which parses raw HTML content
 *        and exposes structured extraction utilities (text, headings, attributes, tables).
 *
 * @author Adrien GRAS
 * @date 2026-06-04
 */


// Imports
#include "HtmlParser.hpp"
#include <algorithm>
#include <cctype>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Constructs an HtmlParser instance with the given HTML content.
     *
     * @details Parses the raw HTML string using libxml2's htmlReadMemory().
     * The flags HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR | HTML_PARSE_RECOVER
     * suppress error output and allow recovery from malformed HTML (e.g. L'Argus pages).
     * If the string is empty, _doc remains nullptr and isLoaded() returns false.
     *
     * @param html The raw HTML content to parse.
     */
    HtmlParser::HtmlParser(const std::string& html) {

        // First init
        _doc        = nullptr;
        _rawHtml    = html;


        // Stop if the HTML is empty
        if (html.empty()) return;


        // Parse the HTML content into a libxml2 document
        _doc = htmlReadMemory(
            html.c_str(),
            static_cast<int>(html.size()),
            nullptr,    // URL (optional, can be nullptr for memory parsing)
            "UTF-8",    // Encoding
            HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR | HTML_PARSE_RECOVER
        );

    }


    /**
     * @brief Destructor for the HtmlParser instance.
     * @details Releases the libxml2 document held by _doc.
     */
    HtmlParser::~HtmlParser() {

        // Free the libxml2 document
        if (_doc) {
            xmlFreeDoc(_doc);
            _doc = nullptr;     // Safety measure against double-free
        }

    }


    /**
     * @brief Move constructor — transfers ownership of the libxml2 document.
     *
     * @details The source instance is left with _doc = nullptr so its destructor
     * becomes a no-op. _rawHtml is moved to avoid an unnecessary copy.
     *
     * @param other The HtmlParser instance to move from.
     */
    HtmlParser::HtmlParser(HtmlParser&& other) noexcept {
        _doc        = other._doc;
        _rawHtml    = std::move(other._rawHtml);
        other._doc  = nullptr;
    }


    /**
     * @brief Move assignment operator — transfers ownership of the libxml2 document.
     *
     * Frees the current _doc before taking ownership of other's.
     * Self-assignment is guarded with a pointer comparison.
     *
     * @param other The HtmlParser instance to move from.
     * @return Reference to the current instance.
     */
    HtmlParser& HtmlParser::operator=(HtmlParser&& other) noexcept {

        // Guard against self-assignment
        if (this != &other) {

            // Free current document if it exists
            if (_doc) {
                xmlFreeDoc(_doc);
            }
            _doc       = other._doc;
            _rawHtml   = std::move(other._rawHtml);
            other._doc = nullptr;

        }


        // Return pointer
        return *this;

    }





    // =========================================================================
    // Private helpers
    // =========================================================================

    /**
     * @brief Evaluates an XPath expression and returns the result.
     *
     * @details Creates a temporary XPath context, evaluates the expression, then
     * frees the context immediately (the caller owns the returned object).
     * Returns nullptr if _doc is null, if context creation fails, if evaluation
     * fails, or if the resulting node set is empty.
     *
     * @param xpath The XPath expression to evaluate.
     * @return Allocated xmlXPathObjectPtr on success, nullptr otherwise.
     *         The caller is responsible for calling xmlXPathFreeObject().
     */
    xmlXPathObjectPtr HtmlParser::_evalXPath(const std::string& xpath) const  {
        if (!_doc) return nullptr;

        xmlXPathContextPtr ctx = xmlXPathNewContext(_doc);
        if (!ctx) return nullptr;

        xmlXPathObjectPtr result = xmlXPathEvalExpression(
            reinterpret_cast<const xmlChar*>(xpath.c_str()), ctx
        );
        xmlXPathFreeContext(ctx);

        if (!result) return nullptr;
        if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
            xmlXPathFreeObject(result);
            return nullptr;
        }
        return result;
    }


    /**
     * @brief Extracts the text content of an XML node.
     *
     * @details Calls xmlNodeGetContent() which recursively concatenates text from
     * all child nodes. The raw xmlChar* buffer is freed after conversion
     * to std::string. The result is trimmed of leading/trailing whitespace.
     *
     * @param node The XML node to extract text from.
     * @return Trimmed text content, or an empty string if node is null or has no content.
     */
    std::string HtmlParser::_nodeText(xmlNodePtr node) const {
        if (!node) return "";

        xmlChar* raw = xmlNodeGetContent(node);
        if (!raw) return "";

        std::string text(reinterpret_cast<const char*>(raw));
        xmlFree(raw);
        return _trimWhitespace(text);
    }


    /**
     * @brief Trims whitespace from both ends of a string.
     *
     * Uses std::find_if_not with std::isspace on unsigned char to avoid
     * undefined behaviour on signed char values > 127.
     *
     * @param stringValue The string to trim.
     * @return A new string with leading and trailing whitespace removed,
     *         or an empty string if stringValue contains only whitespace.
     */
    std::string HtmlParser::_trimWhitespace(const std::string& stringValue) const {
        auto start = std::find_if_not(stringValue.begin(), stringValue.end(),
            [](unsigned char c){ return std::isspace(c); });
        auto end = std::find_if_not(stringValue.rbegin(), stringValue.rend(),
            [](unsigned char c){ return std::isspace(c); }).base();
        return (start < end) ? std::string(start, end) : "";
    }


    // =========================================================================
    // Text extraction by tag or XPath
    // =========================================================================

    /**
     * @brief Returns the inner text of the first node matching the given XPath expression.
     *
     * Delegates to _evalXPath(), then extracts text from nodeTab[0].
     * Returns std::nullopt if no node is found or if the text content is empty.
     *
     * @param xpath XPath expression to select the node.
     * @return Optional string containing the inner text, or std::nullopt if not found.
     */
    std::optional<std::string> HtmlParser::getText(const std::string& xpath) const {
        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return std::nullopt;

        std::string text = _nodeText(result->nodesetval->nodeTab[0]);
        xmlXPathFreeObject(result);

        if (text.empty()) return std::nullopt;
        return text;
    }


    /**
     * @brief Returns the inner texts of all nodes matching the given XPath expression.
     *
     * Iterates over all nodes in the result set, extracting text from each.
     * Empty strings (whitespace-only nodes) are silently skipped.
     *
     * @param xpath XPath expression to select nodes.
     * @return Vector of non-empty trimmed strings, one per matching node.
     *         Returns an empty vector if no nodes match.
     */
    std::vector<std::string> HtmlParser::getAllTexts(const std::string& xpath) const {
        std::vector<std::string> texts;

        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return texts;

        xmlNodeSetPtr nodes = result->nodesetval;
        for (int i = 0; i < nodes->nodeNr; ++i) {
            std::string text = _nodeText(nodes->nodeTab[i]);
            if (!text.empty())
                texts.push_back(std::move(text));
        }

        xmlXPathFreeObject(result);
        return texts;
    }


    /**
     * @brief Returns the inner text of the first <hN> tag (level 1–6).
     *
     * Builds the XPath expression "//hN" from the requested level and
     * delegates to getText(). Returns std::nullopt if level is out of range.
     *
     * @param level Heading level (1–6).
     * @return Optional string containing the heading text, or std::nullopt if not found.
     */
    std::optional<std::string> HtmlParser::getHeading(int level) const {
        if (level < 1 || level > 6) return std::nullopt;
        return getText("//h" + std::to_string(level));
    }


    /**
     * @brief Returns the inner texts of all <hN> tags at the given level.
     *
     * Builds the XPath expression "//hN" from the requested level and
     * delegates to getAllTexts(). Returns an empty vector if level is out of range.
     *
     * @param level Heading level (1–6).
     * @return Vector of strings containing the heading texts.
     */
    std::vector<std::string> HtmlParser::getAllHeadings(int level) const {
        if (level < 1 || level > 6) return {};
        return getAllTexts("//h" + std::to_string(level));
    }





    // =========================================================================
    // Attribute extraction
    // =========================================================================

    /**
     * @brief Returns the value of `attribute` on the first node matching `xpath`.
     *
     * Uses xmlGetProp() to retrieve the attribute value from the matched node.
     * The raw xmlChar* buffer is freed after conversion to std::string.
     * Returns std::nullopt if no node matches, or if the attribute is absent.
     *
     * @param xpath XPath expression to select the node.
     * @param attribute Name of the HTML attribute to retrieve (e.g. "href", "class").
     * @return Optional string containing the trimmed attribute value, or std::nullopt.
     */
    std::optional<std::string> HtmlParser::getAttribute(const std::string& xpath,
                                                        const std::string& attribute) const {
        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return std::nullopt;

        xmlNodePtr node = result->nodesetval->nodeTab[0];
        xmlXPathFreeObject(result);

        xmlChar* raw = xmlGetProp(node,
            reinterpret_cast<const xmlChar*>(attribute.c_str()));
        if (!raw) return std::nullopt;

        std::string value(reinterpret_cast<const char*>(raw));
        xmlFree(raw);
        return _trimWhitespace(value);
    }


    /**
     * @brief Returns the value of `attribute` for every node matching `xpath`.
     *
     * Iterates over all nodes in the XPath result set and calls xmlGetProp() on
     * each one. Nodes that do not carry the requested attribute are silently
     * skipped, so the output vector may be smaller than the node set.
     *
     * Typical use-cases:
     *   - Extract all href from a <ul> of year links (with or without XPath filter).
     *   - Extract all href from a table's <td><a> cells (version listing).
     *
     * @param xpath      XPath expression to select the nodes.
     * @param attribute  Name of the HTML attribute to retrieve (e.g. "href", "src").
     * @return Vector of trimmed attribute values. Returns an empty vector if no
     *         nodes match or if none of the matched nodes carry the attribute.
     */
    std::vector<std::string> HtmlParser::getAllAttributes(const std::string& xpath,
                                                        const std::string& attribute) const
    {
        std::vector<std::string> values;

        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return values;

        xmlNodeSetPtr nodes = result->nodesetval;
        for (int i = 0; i < nodes->nodeNr; ++i) {

            xmlChar* raw = xmlGetProp(nodes->nodeTab[i],
                reinterpret_cast<const xmlChar*>(attribute.c_str()));

            if (!raw) continue; // attribute absent on this node — skip

            std::string value(reinterpret_cast<const char*>(raw));
            xmlFree(raw);

            std::string trimmed = _trimWhitespace(value);
            if (!trimmed.empty())
                values.push_back(std::move(trimmed));
        }

        xmlXPathFreeObject(result);
        return values;
    }





    // =========================================================================
    // Table extraction
    // =========================================================================

    /**
     * @brief Converts a table node into a vector of maps representing its rows.
     *
     * Strategy:
     *   1. Try to extract column headers from <th> cells of the first <tr>.
     *   2. If no <th> exists, fall back to using the first row's <td> cells as headers.
     *   3. Iterate over all subsequent <tr> rows, mapping each <td> to its header.
     *
     * A dedicated XPath context rooted at tableNode is used so that all XPath
     * expressions are evaluated relative to the table, preventing cross-table
     * collisions when multiple tables are present in the document.
     *
     * Rows with no <td> cells (e.g. spacer rows) are silently skipped.
     * Extra <td> cells beyond the header count are also silently ignored.
     *
     * @param tableNode The XML node representing the <table> element.
     * @return Vector of maps containing the table data, where each map is
     *         { column_header -> cell_value }. Returns an empty vector if
     *         the table has no usable headers or no data rows.
     */
    std::vector<std::map<std::string, std::string>>
    HtmlParser::_tableNodeToMap(xmlNodePtr tableNode) const {
        std::vector<std::map<std::string, std::string>> rows;
        std::vector<std::string> headers;

        // XPath context rooted at the table node for relative evaluations
        xmlXPathContextPtr ctx = xmlXPathNewContext(_doc);
        if (!ctx) return rows;
        ctx->node = tableNode;

        // Lambda for concise relative XPath evaluation within this table
        auto fetch = [&](const std::string& xp) -> xmlXPathObjectPtr {
            return xmlXPathEvalExpression(
                reinterpret_cast<const xmlChar*>(xp.c_str()), ctx);
        };


        // =====================================================================
        // Step 1 — extract column headers
        // =====================================================================
        //
        // Strategy (in order of preference) :
        //   A) <thead> exists → read <th> from its <tr>
        //   B) No <thead>     → read <th> from the first <tr> anywhere
        //   C) No <th> at all → read <td> from the first <tr> anywhere
        //                       (first row is treated as implicit header)
        //
        // We avoid .//tr[1] and .//tr[position()>1] because position() is
        // evaluated per-parent-group by libxml2 : in a thead/tbody table,
        // position() resets to 1 inside tbody, so tbody/tr[1] (the first data
        // row) would be incorrectly skipped.

        // A) <thead> present — most reliable source of headers
        xmlXPathObjectPtr theadTh = fetch(".//thead/tr/th");
        if (theadTh && !xmlXPathNodeSetIsEmpty(theadTh->nodesetval)) {
            xmlNodeSetPtr nodes = theadTh->nodesetval;
            for (int i = 0; i < nodes->nodeNr; ++i)
                headers.push_back(_nodeText(nodes->nodeTab[i]));
            xmlXPathFreeObject(theadTh);
        } else {
            if (theadTh) xmlXPathFreeObject(theadTh);

            // B) No <thead>, but there are <th> somewhere in the first <tr>
            xmlXPathObjectPtr firstTh = fetch(".//tr/th");
            if (firstTh && !xmlXPathNodeSetIsEmpty(firstTh->nodesetval)) {
                // Only take the <th> siblings of the very first <tr> that has one
                xmlNodePtr firstTr = firstTh->nodesetval->nodeTab[0]->parent;
                xmlXPathFreeObject(firstTh);

                xmlXPathContextPtr trCtx = xmlXPathNewContext(_doc);
                trCtx->node = firstTr;
                xmlXPathObjectPtr thInTr = xmlXPathEvalExpression(
                    reinterpret_cast<const xmlChar*>("th"), trCtx);
                xmlXPathFreeContext(trCtx);

                if (thInTr && !xmlXPathNodeSetIsEmpty(thInTr->nodesetval)) {
                    for (int i = 0; i < thInTr->nodesetval->nodeNr; ++i)
                        headers.push_back(_nodeText(thInTr->nodesetval->nodeTab[i]));
                }
                if (thInTr) xmlXPathFreeObject(thInTr);
            } else {
                if (firstTh) xmlXPathFreeObject(firstTh);

                // C) No <th> at all — first <tr> cells are the implicit headers
                xmlXPathObjectPtr firstTds = fetch(".//tr/td");
                if (!firstTds || xmlXPathNodeSetIsEmpty(firstTds->nodesetval)) {
                    if (firstTds) xmlXPathFreeObject(firstTds);
                    xmlXPathFreeContext(ctx);
                    return rows;  // empty table, nothing to extract
                }

                // Collect only the <td> children of the very first <tr>
                xmlNodePtr firstTr = firstTds->nodesetval->nodeTab[0]->parent;
                xmlXPathFreeObject(firstTds);

                xmlXPathContextPtr trCtx = xmlXPathNewContext(_doc);
                trCtx->node = firstTr;
                xmlXPathObjectPtr tdInTr = xmlXPathEvalExpression(
                    reinterpret_cast<const xmlChar*>("td"), trCtx);
                xmlXPathFreeContext(trCtx);

                if (!tdInTr || xmlXPathNodeSetIsEmpty(tdInTr->nodesetval)) {
                    if (tdInTr) xmlXPathFreeObject(tdInTr);
                    xmlXPathFreeContext(ctx);
                    return rows;
                }
                for (int i = 0; i < tdInTr->nodesetval->nodeNr; ++i)
                    headers.push_back(_nodeText(tdInTr->nodesetval->nodeTab[i]));
                xmlXPathFreeObject(tdInTr);

                // The first <tr> was consumed as headers — remember it to skip it
                // when collecting data rows in Step 2.
                // We mark it by storing its pointer; Step 2 checks against it.
                xmlXPathContextPtr dataCtx = xmlXPathNewContext(_doc);
                dataCtx->node = tableNode;
                xmlXPathObjectPtr allTrs = xmlXPathEvalExpression(
                    reinterpret_cast<const xmlChar*>(".//tr"), dataCtx);
                xmlXPathFreeContext(dataCtx);

                if (!allTrs || xmlXPathNodeSetIsEmpty(allTrs->nodesetval)) {
                    if (allTrs) xmlXPathFreeObject(allTrs);
                    xmlXPathFreeContext(ctx);
                    return rows;
                }

                // Collect data rows — skip the first <tr> (used as header)
                xmlNodeSetPtr trNodes = allTrs->nodesetval;
                for (int i = 1; i < trNodes->nodeNr; ++i) {
                    xmlXPathContextPtr rowCtx = xmlXPathNewContext(_doc);
                    rowCtx->node = trNodes->nodeTab[i];
                    xmlXPathObjectPtr tds = xmlXPathEvalExpression(
                        reinterpret_cast<const xmlChar*>("td"), rowCtx);
                    xmlXPathFreeContext(rowCtx);

                    if (!tds || xmlXPathNodeSetIsEmpty(tds->nodesetval)) {
                        if (tds) xmlXPathFreeObject(tds);
                        continue;
                    }

                    std::map<std::string, std::string> rowMap;
                    for (int j = 0; j < tds->nodesetval->nodeNr; ++j) {
                        if (j < static_cast<int>(headers.size()))
                            rowMap[headers[j]] = _nodeText(tds->nodesetval->nodeTab[j]);
                    }
                    xmlXPathFreeObject(tds);

                    if (!rowMap.empty())
                        rows.push_back(std::move(rowMap));
                }

                xmlXPathFreeObject(allTrs);
                xmlXPathFreeContext(ctx);
                return rows;  // early return — data rows already collected above
            }
        }


        // =====================================================================
        // Step 2 — extract data rows
        // =====================================================================
        //
        // If we reach here, headers came from <th> cells (cases A or B).
        // Data rows are all <tr> that contain <td> — i.e. rows in <tbody>,
        // or any <tr> without <th> in a flat table.
        // We explicitly target <tbody> first; fall back to all <tr> with <td>.

        xmlXPathObjectPtr dataTrs = fetch(".//tbody/tr");
        if (!dataTrs || xmlXPathNodeSetIsEmpty(dataTrs->nodesetval)) {
            // No <tbody> — flat table: collect every <tr> that has <td> children
            if (dataTrs) xmlXPathFreeObject(dataTrs);
            dataTrs = fetch(".//tr[td]");
        }

        if (!dataTrs || xmlXPathNodeSetIsEmpty(dataTrs->nodesetval)) {
            if (dataTrs) xmlXPathFreeObject(dataTrs);
            xmlXPathFreeContext(ctx);
            return rows;
        }

        xmlNodeSetPtr trNodes = dataTrs->nodesetval;
        for (int i = 0; i < trNodes->nodeNr; ++i) {
            xmlXPathContextPtr rowCtx = xmlXPathNewContext(_doc);
            rowCtx->node = trNodes->nodeTab[i];
            xmlXPathObjectPtr tds = xmlXPathEvalExpression(
                reinterpret_cast<const xmlChar*>("td"), rowCtx);
            xmlXPathFreeContext(rowCtx);

            if (!tds || xmlXPathNodeSetIsEmpty(tds->nodesetval)) {
                if (tds) xmlXPathFreeObject(tds);
                continue;
            }

            std::map<std::string, std::string> rowMap;
            for (int j = 0; j < tds->nodesetval->nodeNr; ++j) {
                if (j < static_cast<int>(headers.size()))
                    rowMap[headers[j]] = _nodeText(tds->nodesetval->nodeTab[j]);
            }
            xmlXPathFreeObject(tds);

            if (!rowMap.empty())
                rows.push_back(std::move(rowMap));
        }

        xmlXPathFreeObject(dataTrs);
        xmlXPathFreeContext(ctx);
        return rows;
    }


    /**
     * @brief Returns the first <table> matching `xpath` as a list of row-maps.
     *
     * Evaluates the XPath, takes the first node in the result set, and
     * delegates conversion to _tableNodeToMap().
     *
     * @param xpath XPath expression to select the table (default: "//table").
     * @return Vector of row-maps representing the table rows.
     *         Each row-map is { column_header -> cell_value }.
     *         Returns an empty vector if no table matches or the table is empty.
     */
    std::vector<std::map<std::string, std::string>>
    HtmlParser::getTableAsMap(const std::string& xpath) const {
        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return {};

        xmlNodePtr tableNode = result->nodesetval->nodeTab[0];
        auto rows = _tableNodeToMap(tableNode);
        xmlXPathFreeObject(result);
        return rows;
    }


    /**
     * @brief Returns ALL <table> nodes matching `xpath`, each converted to a row-map list.
     *
     * Iterates over every node in the XPath result set and calls _tableNodeToMap()
     * on each. Tables that produce an empty row set (no data) are silently skipped.
     *
     * @param xpath XPath expression to select tables (default: "//table").
     * @return Vector of tables, where each table is a vector of row-maps.
     *         Each row-map is { column_header -> cell_value }.
     *         Returns an empty vector if no tables match.
     */
    std::vector<std::vector<std::map<std::string, std::string>>>
    HtmlParser::getAllTablesAsMap(const std::string& xpath) const {
        std::vector<std::vector<std::map<std::string, std::string>>> tables;

        xmlXPathObjectPtr result = _evalXPath(xpath);
        if (!result) return tables;

        xmlNodeSetPtr nodes = result->nodesetval;
        for (int i = 0; i < nodes->nodeNr; ++i) {
            auto rows = _tableNodeToMap(nodes->nodeTab[i]);
            if (!rows.empty())
                tables.push_back(std::move(rows));
        }

        xmlXPathFreeObject(result);
        return tables;
    }

} // namespace CarScraper