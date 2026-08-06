/**
 * @file baseHtmlSaver.cpp
 *
 * @brief Unit tests for CarScraper::HtmlSaver class.
 *        The [network] tests fetch a real page from example.org and save it
 *        to data/test_html/ so the fixture can be inspected manually.
 *        Run without network tests: ./TestHtmlSaver "[HtmlSaver]" "~[network]"
 *
 * @author Adrien GRAS
 * @date 2026-06-24
 */


// Imports
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "io/htmlHandlers/HtmlSaver.hpp"
#include "io/httpClient/HttpClient.hpp"
#include "core/utils/Constant.hpp"

using namespace CarScraper;
namespace fs = std::filesystem;

static const std::string TEST_HTML_DIR  = "data/test_save_html/";
static const std::string TEST_LINK_FILE = "data/test_save_link.json";
static const std::string TEST_NET_DIR   = "data/test_html/";
static const std::string EXAMPLE_URL    = "https://www.example.org";
static const std::string EXAMPLE_FILE   = TEST_NET_DIR + "example_org.txt";


// =============================================================================
// Helpers
// =============================================================================

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void deleteAllButNot(const std::string& fileToKeep) {
    fs::path fileKept = fs::canonical(fileToKeep);
    fs::path directory = fileKept.parent_path();
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file())
            continue;

        if (fs::canonical(entry.path()) != fileKept) {
            fs::remove(entry.path());
        }
    }
}

void deleteAll(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file())
            continue;
        fs::remove(entry.path());
    }
}

static void prepareDirectories() {
    fs::create_directories(TEST_HTML_DIR);
    std::ofstream file(TEST_LINK_FILE);
    if (file.is_open()) {
        file << R"([
            "https://example.com"
        ])";
        file.close();
    }
}

static void cleanDirectories() {
    fs::remove_all(TEST_HTML_DIR);
    fs::remove(TEST_LINK_FILE);
    deleteAllButNot(TEST_NET_DIR + "htmlparser_test.html");
}


// =============================================================================
// Constructor
// =============================================================================

TEST_CASE("HtmlSaver Constructor", "[HtmlSaver][Constructor]") {

    prepareDirectories();

    SECTION("Default constructor") {
        HtmlSaver saver;

        REQUIRE(saver.getName()         == DEFAULT_STR);
        REQUIRE(saver.getContent()      == DEFAULT_STR);
        REQUIRE(saver.getOutputDir()    == HTML_DIR);
        REQUIRE(saver.getLinkFile()     == LINK_FILE);
    }

    SECTION("Parameterized constructor") {
        HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );

        REQUIRE(saver.getName()         == "page");
        REQUIRE(saver.getContent()      == "<html>Hello</html>");
        REQUIRE(saver.getLink()         == "https://example.com");
        REQUIRE(saver.getOutputDir()    == TEST_HTML_DIR);
        REQUIRE(saver.getLinkFile()     == TEST_LINK_FILE);
    }

    cleanDirectories();
}


// =============================================================================
// Setters
// =============================================================================

TEST_CASE("HtmlSaver Setters", "[HtmlSaver][Setters]") {

    prepareDirectories();

    HtmlSaver saver;

    SECTION("setName") {
        saver.setName("new_name");
        REQUIRE(saver.getName() == "new_name");
    }

    SECTION("setContent") {
        saver.setContent("content");
        REQUIRE(saver.getContent() == "content");
    }

    SECTION("setLink") {
        saver.setLink("https://example.com");
        REQUIRE(saver.getLink() == "https://example.com");
    }

    SECTION("setOutputDir — valid directory") {
        saver.setOutputDir(TEST_HTML_DIR);
        REQUIRE(saver.getOutputDir() == TEST_HTML_DIR);
    }

    SECTION("setOutputDir — invalid directory falls back to HTML_DIR") {
        saver.setOutputDir("nonexistent/path/");
        REQUIRE(saver.getOutputDir() == HTML_DIR);
    }

    SECTION("setLinkFile — valid file") {
        saver.setLinkFile(TEST_LINK_FILE);
        REQUIRE(saver.getLinkFile() == TEST_LINK_FILE);
    }

    SECTION("setLinkFile — invalid file falls back to LINK_FILE") {
        saver.setLinkFile("nonexistent/path/");
        REQUIRE(saver.getLinkFile() == LINK_FILE);
    }

    cleanDirectories();
}


// =============================================================================
// getFilePath
// =============================================================================

TEST_CASE("HtmlSaver getFilePath", "[HtmlSaver][FilePath]") {

    prepareDirectories();

    HtmlSaver saver(
        "test_file",
        "<html/>",
        "https://example.com",
        TEST_HTML_DIR
    );

    REQUIRE(saver.getFilePath() == TEST_HTML_DIR + "test_file.txt");

    cleanDirectories();
}


// =============================================================================
// save()
// =============================================================================

TEST_CASE("HtmlSaver save()", "[HtmlSaver][Save]") {

    prepareDirectories();

    SECTION("Successful save") {
        HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );

        REQUIRE(saver.save() == SUCCESS_CODE);
        REQUIRE(fs::exists(TEST_HTML_DIR + "page.txt"));
        REQUIRE(readFile(TEST_HTML_DIR + "page.txt") == "<html>Hello</html>");
    }

    SECTION("Save fails when link is not set") {
        HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            DEFAULT_STR,
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );

        REQUIRE(saver.save() == ERROR_CODE);
    }

    SECTION("Saving same link twice returns ignored code") {
        HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );

        REQUIRE(saver.save() == SUCCESS_CODE);
        REQUIRE(saver.save() == IGNORED_ACTION_CODE);
    }

    SECTION("Filename collision — second file saved under dp_ name") {

        deleteAll(TEST_HTML_DIR);

        // First save : crée page.txt
        HtmlSaver saver1(
            "page",
            "<html>First</html>",
            "https://example.com/first",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );
        REQUIRE(saver1.save() == SUCCESS_CODE);
        REQUIRE(fs::exists(TEST_HTML_DIR + "page.txt"));

        // Second save : même _name "page", URL différente → collision
        HtmlSaver saver2(
            "page",
            "<html>Second</html>",
            "https://example.com/second",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );
        REQUIRE(saver2.save() == SUCCESS_CODE);

        // Le fichier original doit pas avoir été renommé
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_0.txt") == "<html>First</html>");

        // Le fichier de repli doit exister et contenir le bon contenu
        REQUIRE(fs::exists(TEST_HTML_DIR + "dp_page_1.txt"));
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_1.txt") == "<html>Second</html>");

        // _name de saver2 ne doit pas avoir été muté
        REQUIRE(saver2.getName() == "page");
    }

    SECTION("Filename collision — plusieurs collisions successives") {

        deleteAll(TEST_HTML_DIR);

        // Crée page.txt manuellement
        { std::ofstream f(TEST_HTML_DIR + "page.txt");     f << "dp0"; }

        // Sauvegarde 3 fois
        HtmlSaver saver(
            "page",
            "dp1",
            "https://example.com/1",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );
        REQUIRE(saver.save() == SUCCESS_CODE);

        saver.setContent("dp2");
        saver.setLink("https://example.com/2");
        REQUIRE(saver.save() == SUCCESS_CODE);

        saver.setContent("dp3");
        saver.setLink("https://example.com/3");
        REQUIRE(saver.save() == SUCCESS_CODE);

        // Le fichier existant est renommé mais son contenu intact
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_0.txt") == "dp0");

        // Le 1er fichier atterrit en dp_page_1
        REQUIRE(fs::exists(TEST_HTML_DIR + "dp_page_1.txt"));
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_1.txt") == "dp1");

        // Le 1er fichier atterrit en dp_page_2
        REQUIRE(fs::exists(TEST_HTML_DIR + "dp_page_2.txt"));
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_2.txt") == "dp2");

        // Le 1er fichier atterrit en dp_page_3
        REQUIRE(fs::exists(TEST_HTML_DIR + "dp_page_3.txt"));
        REQUIRE(readFile(TEST_HTML_DIR + "dp_page_3.txt") == "dp3");

        // _name toujours intact
        REQUIRE(saver.getName() == "page");
    }

    // cleanDirectories();
}


// =============================================================================
// alreadySaved()
// =============================================================================

TEST_CASE("HtmlSaver alreadySaved()", "[HtmlSaver][AlreadySaved]") {

    prepareDirectories();

    HtmlSaver saver(
        "page",
        "<html>Hello</html>",
        "https://example.com",
        TEST_HTML_DIR,
        TEST_LINK_FILE
    );

    REQUIRE_FALSE(saver.alreadySaved("https://example.com"));

    saver.save();

    REQUIRE(saver.alreadySaved("https://example.com"));

    cleanDirectories();
}


// =============================================================================
// exportSavedLink()
// =============================================================================

TEST_CASE("HtmlSaver exportSavedLink()", "[HtmlSaver][Export]") {

    prepareDirectories();

    HtmlSaver saver(
        "page",
        "<html>Hello</html>",
        "https://example.com",
        TEST_HTML_DIR,
        TEST_LINK_FILE
    );

    saver.save();

    REQUIRE(saver.exportSavedLink() == SUCCESS_CODE);
    REQUIRE(fs::exists(TEST_LINK_FILE));

    std::ifstream file(TEST_LINK_FILE);
    nlohmann::json json;
    file >> json;

    REQUIRE(json.is_array());
    REQUIRE(json.size() == 1);
    REQUIRE(json[0] == "https://example.com");

    cleanDirectories();
}


// =============================================================================
// importSavedLink()
// =============================================================================

TEST_CASE("HtmlSaver importSavedLink()", "[HtmlSaver][Import]") {

    prepareDirectories();

    SECTION("Import valid json file") {
        nlohmann::json json = {
            "https://example.com",
            "https://google.com"
        };

        std::ofstream file(TEST_LINK_FILE);
        file << json.dump(4);
        file.close();

        HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://dummy.com",
            TEST_HTML_DIR,
            TEST_LINK_FILE
        );

        REQUIRE(saver.importSavedLink() == SUCCESS_CODE);
        REQUIRE(saver.alreadySaved("https://example.com"));
        REQUIRE(saver.alreadySaved("https://google.com"));
    }

    cleanDirectories();
}


// =============================================================================
// Network — fetch example.org and save to data/test_html/
// Tag: [network] — exclude with "~[network]" to run offline
// =============================================================================

TEST_CASE("HtmlSaver save() with real HTTP content from example.org", "[HtmlSaver][network]") {

    fs::create_directories(TEST_NET_DIR);
    fs::create_directories(TEST_LINK_FILE);

    // --- Fetch the page ------------------------------------------------------
    HttpClient client;
    HttpResponse response = client.get(EXAMPLE_URL);

    REQUIRE(response.statusCode == 200);
    REQUIRE_FALSE(response.body.empty());

    // --- Save via HtmlSaver --------------------------------------------------
    HtmlSaver saver(
        "example_org",
        response.body,
        EXAMPLE_URL,
        TEST_NET_DIR,
        TEST_LINK_FILE
    );

    REQUIRE(saver.save() == SUCCESS_CODE);

    // --- Verify the file was written -----------------------------------------
    REQUIRE(fs::exists(EXAMPLE_FILE));

    std::string saved = readFile(EXAMPLE_FILE);
    REQUIRE_FALSE(saved.empty());

    // example.org always contains these strings
    REQUIRE(saved.find("Example Domain") != std::string::npos);
    REQUIRE(saved.find("<html")          != std::string::npos);

    // --- Verify the link was tracked -----------------------------------------
    REQUIRE(saver.alreadySaved(EXAMPLE_URL));

    // --- Verify a second save is ignored (dedup) -----------------------------
    REQUIRE(saver.save() == IGNORED_ACTION_CODE);

    // Note: TEST_NET_DIR is intentionally NOT cleaned — the file persists at
    // data/test_html/example_org.txt for manual inspection and for
    // baseHtmlParser tests that may want real-world HTML.
    fs::remove_all(TEST_LINK_FILE);
}