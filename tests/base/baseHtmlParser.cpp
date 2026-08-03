/**
 * @file baseHtmlParser.cpp
 *
 * @brief Unit tests for CarScraper::HtmlParser class.
 *        Covers: construction, isLoaded, getText, getAllTexts,
 *                getHeading, getAllHeadings, getAttribute, getAllAttributes,
 *                getTableAsMap, getAllTablesAsMap.
 *
 * @author Adrien GRAS
 * @date 2026-06-24
 */


// Imports
#include <catch2/catch.hpp>
#include <fstream>
#include <sstream>
#include "io/htmlHandlers/HtmlParser.hpp"

using namespace CarScraper;


// =============================================================================
// Helpers
// =============================================================================

static const std::string TEST_HTML_PATH = "data/test_html/htmlparser_test.html";

/**
 * @brief Loads the shared HTML test fixture from disk.
 * All TEST_CASEs that need a real document call this helper.
 */
static std::string loadTestHtml() {
    std::ifstream file(TEST_HTML_PATH);
    REQUIRE(file.is_open());
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


// =============================================================================
// Constructor / isLoaded
// =============================================================================

TEST_CASE("HtmlParser Constructor", "[HtmlParser][Constructor]") {

    SECTION("Valid HTML is loaded successfully") {
        HtmlParser parser(loadTestHtml());
        REQUIRE(parser.isLoaded());
    }

    SECTION("Empty string produces an unloaded parser") {
        HtmlParser parser("");
        REQUIRE_FALSE(parser.isLoaded());
    }

    SECTION("Malformed HTML is recovered and loaded") {
        HtmlParser parser("<html><body><h1>Titre<p>pas fermé");
        REQUIRE(parser.isLoaded());
    }

    SECTION("Move constructor transfers ownership") {
        HtmlParser original(loadTestHtml());
        REQUIRE(original.isLoaded());

        HtmlParser moved(std::move(original));
        REQUIRE(moved.isLoaded());
        REQUIRE_FALSE(original.isLoaded());
    }

    SECTION("Move assignment transfers ownership") {
        HtmlParser a(loadTestHtml());
        HtmlParser b("");

        b = std::move(a);
        REQUIRE(b.isLoaded());
        REQUIRE_FALSE(a.isLoaded());
    }
}


// =============================================================================
// getText
// =============================================================================

TEST_CASE("HtmlParser getText", "[HtmlParser][getText]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns text of first matching node") {
        auto result = parser.getText("//h1");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "Titre principal");
    }

    SECTION("Returns first match when multiple nodes exist") {
        auto result = parser.getText("//h2");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "Section moteur");
    }

    SECTION("Returns text selected by id") {
        auto result = parser.getText("//*[@id='price']");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "15 000 €");
    }

    SECTION("Trims leading and trailing whitespace") {
        auto result = parser.getText("//*[@id='whitespace']");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "valeur avec espaces");
    }

    SECTION("Returns nullopt for unmatched XPath") {
        auto result = parser.getText("//video");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Returns nullopt on unloaded parser") {
        HtmlParser empty("");
        REQUIRE_FALSE(empty.getText("//h1").has_value());
    }
}


// =============================================================================
// getAllTexts
// =============================================================================

TEST_CASE("HtmlParser getAllTexts", "[HtmlParser][getAllTexts]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns all matching node texts") {
        auto results = parser.getAllTexts("//h2");
        REQUIRE(results.size() == 2);
        REQUIRE(results[0] == "Section moteur");
        REQUIRE(results[1] == "Section dimensions");
    }

    SECTION("Returns all <p class='intro'> texts") {
        auto results = parser.getAllTexts("//p[@class='intro']");
        REQUIRE(results.size() == 2);
        REQUIRE(results[0] == "Premier paragraphe d'introduction.");
        REQUIRE(results[1] == "Deuxième paragraphe d'introduction.");
    }

    SECTION("Returns empty vector for unmatched XPath") {
        auto results = parser.getAllTexts("//article");
        REQUIRE(results.empty());
    }

    SECTION("Returns empty vector on unloaded parser") {
        HtmlParser empty("");
        REQUIRE(empty.getAllTexts("//p").empty());
    }
}


// =============================================================================
// getHeading
// =============================================================================

TEST_CASE("HtmlParser getHeading", "[HtmlParser][getHeading]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns first h1") {
        auto result = parser.getHeading(1);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "Titre principal");
    }

    SECTION("Returns first h2") {
        auto result = parser.getHeading(2);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "Section moteur");
    }

    SECTION("Returns first h3") {
        auto result = parser.getHeading(3);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "Sous-section transmission");
    }

    SECTION("Returns nullopt for absent heading level") {
        REQUIRE_FALSE(parser.getHeading(5).has_value());
    }

    SECTION("Returns nullopt for level 0 (out of range)") {
        REQUIRE_FALSE(parser.getHeading(0).has_value());
    }

    SECTION("Returns nullopt for level 7 (out of range)") {
        REQUIRE_FALSE(parser.getHeading(7).has_value());
    }
}


// =============================================================================
// getAllHeadings
// =============================================================================

TEST_CASE("HtmlParser getAllHeadings", "[HtmlParser][getAllHeadings]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns all h2 headings") {
        auto results = parser.getAllHeadings(2);
        REQUIRE(results.size() == 2);
        REQUIRE(results[0] == "Section moteur");
        REQUIRE(results[1] == "Section dimensions");
    }

    SECTION("Returns single h1") {
        auto results = parser.getAllHeadings(1);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == "Titre principal");
    }

    SECTION("Returns empty vector for absent heading level") {
        REQUIRE(parser.getAllHeadings(4).empty());
    }

    SECTION("Returns empty vector for out-of-range level") {
        REQUIRE(parser.getAllHeadings(0).empty());
        REQUIRE(parser.getAllHeadings(7).empty());
    }
}


// =============================================================================
// getAttribute
// =============================================================================

TEST_CASE("HtmlParser getAttribute", "[HtmlParser][getAttribute]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns href of first anchor with class marques-name") {
        auto result = parser.getAttribute(
            "//a[@class='marques-name']", "href");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "/fiche-technique/Renault/Clio/2017.html");
    }

    SECTION("Returns id attribute of a span") {
        auto result = parser.getAttribute("//span[@id='price']", "id");
        REQUIRE(result.has_value());
        REQUIRE(result.value() == "price");
    }

    SECTION("Returns nullopt for absent attribute") {
        REQUIRE_FALSE(parser.getAttribute("//h1", "href").has_value());
    }

    SECTION("Returns nullopt for unmatched XPath") {
        REQUIRE_FALSE(parser.getAttribute("//video", "src").has_value());
    }

    SECTION("Returns nullopt on unloaded parser") {
        HtmlParser empty("");
        REQUIRE_FALSE(empty.getAttribute("//a", "href").has_value());
    }
}


// =============================================================================
// getAllAttributes
// =============================================================================

TEST_CASE("HtmlParser getAllAttributes", "[HtmlParser][getAllAttributes]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns all href from anchors with class marques-name") {
        auto hrefs = parser.getAllAttributes("//a[@class='marques-name']", "href");

        REQUIRE(hrefs.size() == 6);
        REQUIRE(hrefs[0] == "/fiche-technique/Renault/Clio/2017.html");
        REQUIRE(hrefs[1] == "/fiche-technique/Renault/Clio/2016.html");
        REQUIRE(hrefs[2] == "/fiche-technique/Renault/Clio/2015.html");
        REQUIRE(hrefs[3] == "/fiche-technique/Renault/Clio/2014.html");
        REQUIRE(hrefs[4] == "/fiche-technique/Renault/Clio/2013.html");
        REQUIRE(hrefs.back() == "/fiche-technique/Renault/Clio/2012.html");
    }

    SECTION("Filtered XPath returns only matching years (2014-2015)") {
        auto hrefs = parser.getAllAttributes(
            "//a[@class='marques-name']"
            "[contains(@href,'/2014.') or contains(@href,'/2015.')]",
            "href"
        );

        REQUIRE(hrefs.size() == 2);
        REQUIRE(hrefs[0] == "/fiche-technique/Renault/Clio/2015.html");
        REQUIRE(hrefs[1] == "/fiche-technique/Renault/Clio/2014.html");
    }

    SECTION("Returns all href from version table links") {
        auto hrefs = parser.getAllAttributes("//table[@id='versions']//td/a", "href");

        REQUIRE(hrefs.size() == 3);
        REQUIRE(hrefs[0] == "/fiche-technique/Renault/Clio/V/2026/Berline/10-TCe-90ch-Evolution.html");
        REQUIRE(hrefs[1] == "/fiche-technique/Renault/Clio/V/2026/Berline/15-dCi-100ch-Evolution.html");
        REQUIRE(hrefs[2] == "/fiche-technique/Renault/Clio/V/2026/Berline/16-ETech-145ch-Evolution.html");
    }

    SECTION("Nodes without the requested attribute are silently skipped") {
        // <h1> and <h2> have no href — result should be empty
        auto hrefs = parser.getAllAttributes("//h1 | //h2", "href");
        REQUIRE(hrefs.empty());
    }

    SECTION("Returns empty vector for unmatched XPath") {
        auto hrefs = parser.getAllAttributes("//video", "src");
        REQUIRE(hrefs.empty());
    }

    SECTION("Returns empty vector on unloaded parser") {
        HtmlParser empty("");
        REQUIRE(empty.getAllAttributes("//a", "href").empty());
    }
}


// =============================================================================
// getTableAsMap
// =============================================================================

TEST_CASE("HtmlParser getTableAsMap", "[HtmlParser][getTableAsMap]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Extracts table with <th> headers") {
        auto rows = parser.getTableAsMap("//table[@id='specs']");

        REQUIRE(rows.size() == 2);

        // Row 0
        REQUIRE(rows[0].count("Marque"));
        REQUIRE(rows[0].count("Modèle"));
        REQUIRE(rows[0].count("Puissance"));
        REQUIRE(rows[0].count("Énergie"));
        REQUIRE(rows[0].at("Marque")    == "Peugeot");
        REQUIRE(rows[0].at("Modèle")    == "308");
        REQUIRE(rows[0].at("Puissance") == "130 ch");
        REQUIRE(rows[0].at("Énergie")   == "Essence");

        // Row 1
        REQUIRE(rows[1].at("Marque")    == "Renault");
        REQUIRE(rows[1].at("Modèle")    == "Mégane");
        REQUIRE(rows[1].at("Puissance") == "115 ch");
        REQUIRE(rows[1].at("Énergie")   == "Diesel");
    }

    SECTION("Extracts table without <th> using first row as implicit header") {
        auto rows = parser.getTableAsMap("//table[@id='consumption']");

        REQUIRE(rows.size() == 2);
        REQUIRE(rows[0].count("Carburant"));
        REQUIRE(rows[0].count("Boîte"));
        REQUIRE(rows[0].count("Consommation"));
        REQUIRE(rows[0].at("Carburant")    == "Essence");
        REQUIRE(rows[0].at("Boîte")        == "Manuelle");
        REQUIRE(rows[0].at("Consommation") == "6.5 L/100");
        REQUIRE(rows[1].at("Carburant")    == "Diesel");
        REQUIRE(rows[1].at("Boîte")        == "Automatique");
        REQUIRE(rows[1].at("Consommation") == "5.2 L/100");
    }

    SECTION("Default XPath returns first table in document") {
        auto rows = parser.getTableAsMap();
        REQUIRE(rows.size() == 2);
        REQUIRE(rows[0].count("Marque"));
    }

    SECTION("Returns empty vector for unmatched XPath") {
        auto rows = parser.getTableAsMap("//table[@id='nope']");
        REQUIRE(rows.empty());
    }

    SECTION("Returns empty vector on unloaded parser") {
        HtmlParser empty("");
        REQUIRE(empty.getTableAsMap().empty());
    }
}


// =============================================================================
// getAllTablesAsMap
// =============================================================================

TEST_CASE("HtmlParser getAllTablesAsMap", "[HtmlParser][getAllTablesAsMap]") {

    HtmlParser parser(loadTestHtml());

    SECTION("Returns all tables in the document") {
        auto tables = parser.getAllTablesAsMap();
        // Fixture has 3 tables: specs, consumption, versions
        REQUIRE(tables.size() == 3);
    }

    SECTION("First table is specs (2 data rows)") {
        auto tables = parser.getAllTablesAsMap();
        REQUIRE(tables[0].size() == 2);
        REQUIRE(tables[0][0].at("Marque") == "Peugeot");
        REQUIRE(tables[0][1].at("Marque") == "Renault");
    }

    SECTION("Second table is consumption (2 data rows)") {
        auto tables = parser.getAllTablesAsMap();
        REQUIRE(tables[1].size() == 2);
        REQUIRE(tables[1][0].at("Carburant") == "Essence");
        REQUIRE(tables[1][1].at("Carburant") == "Diesel");
    }

    SECTION("Third table is versions (3 data rows)") {
        auto tables = parser.getAllTablesAsMap();
        REQUIRE(tables[2].size() == 3);
        REQUIRE(tables[2][0].count("Version"));
        REQUIRE(tables[2][0].count("Carrosserie"));
        REQUIRE(tables[2][0].count("Énergie"));
    }

    SECTION("XPath filter returns only matching tables") {
        auto tables = parser.getAllTablesAsMap("//table[@id='versions']");
        REQUIRE(tables.size() == 1);
        REQUIRE(tables[0].size() == 3);
    }

    SECTION("Returns empty vector when no tables match") {
        auto tables = parser.getAllTablesAsMap("//table[@id='nope']");
        REQUIRE(tables.empty());
    }

    SECTION("Returns empty vector on unloaded parser") {
        HtmlParser empty("");
        REQUIRE(empty.getAllTablesAsMap().empty());
    }
}