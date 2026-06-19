/**
 * @file baseHtmlSaver.cpp
 *
 * @brief Unit tests for CarScraper::HtmlSaver class.
 *
 * @author Adrien GRAS
 * @date 2026-06-03
 */


// Imports
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "io/htmlSaver/HtmlSaver.hpp"
#include "core/utils/Constant.hpp"

namespace fs = std::filesystem;

static const std::string TEST_HTML_DIR = "data/test_save_html/";
static const std::string TEST_LINK_DIR = "data/test_save_link/";


// ============================================================================
// Helpers
// ============================================================================

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static void prepareDirectories() {
    fs::create_directories(TEST_HTML_DIR);
    fs::create_directories(TEST_LINK_DIR);
}

static void cleanDirectories() {
    fs::remove_all(TEST_HTML_DIR);
    fs::remove_all(TEST_LINK_DIR);
}


// ============================================================================
// Constructor
// ============================================================================

TEST_CASE("HtmlSaver Constructor", "[HtmlSaver][Constructor]") {

    prepareDirectories();

    SECTION("Default constructor") {
        CarScraper::HtmlSaver saver;

        REQUIRE(saver.getName() == CarScraper::DEFAULT_STR);
        REQUIRE(saver.getContent() == CarScraper::DEFAULT_STR);
        REQUIRE(saver.getOutputDir() == CarScraper::HTML_DIR);
        REQUIRE(saver.getInputLinkDir() == CarScraper::LINK_DIR);
        REQUIRE(saver.getOutputLinkDir() == CarScraper::LINK_DIR);
    }

    SECTION("Parameterized constructor") {

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(saver.getName() == "page");
        REQUIRE(saver.getContent() == "<html>Hello</html>");
        REQUIRE(saver.getLink() == "https://example.com");
        REQUIRE(saver.getOutputDir() == TEST_HTML_DIR);
        REQUIRE(saver.getInputLinkDir() == TEST_LINK_DIR);
        REQUIRE(saver.getOutputLinkDir() == TEST_LINK_DIR);
    }

    cleanDirectories();
}


// ============================================================================
// Getters / Setters
// ============================================================================

TEST_CASE("HtmlSaver Setters", "[HtmlSaver][Setters]") {

    prepareDirectories();

    CarScraper::HtmlSaver saver;

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

    SECTION("setOutputDir") {
        saver.setOutputDir(TEST_HTML_DIR);
        REQUIRE(saver.getOutputDir() == TEST_HTML_DIR);
    }

    SECTION("setInputLinkDir") {
        saver.setInputLinkDir(TEST_LINK_DIR);
        REQUIRE(saver.getInputLinkDir() == TEST_LINK_DIR);
    }

    SECTION("setOutputLinkDir") {
        saver.setOutputLinkDir(TEST_LINK_DIR);
        REQUIRE(saver.getOutputLinkDir() == TEST_LINK_DIR);
    }

    cleanDirectories();
}


// ============================================================================
// getFilePath
// ============================================================================

TEST_CASE("HtmlSaver getFilePath", "[HtmlSaver][FilePath]") {

    prepareDirectories();

    CarScraper::HtmlSaver saver(
        "test_file",
        "<html/>",
        "https://example.com",
        TEST_HTML_DIR
    );

    REQUIRE(
        saver.getFilePath() ==
        TEST_HTML_DIR + "test_file.txt"
    );

    cleanDirectories();
}


// ============================================================================
// save()
// ============================================================================

TEST_CASE("HtmlSaver save()", "[HtmlSaver][Save]") {

    prepareDirectories();

    SECTION("Successful save") {

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(saver.save() == CarScraper::SUCCESS_CODE);

        REQUIRE(
            fs::exists(TEST_HTML_DIR + "page.txt")
        );

        REQUIRE(
            readFile(TEST_HTML_DIR + "page.txt")
            == "<html>Hello</html>"
        );
    }

    SECTION("Save fails when link is not set") {

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            CarScraper::DEFAULT_STR,
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(saver.save() == CarScraper::ERROR_CODE);
    }

    SECTION("Saving same link twice returns ignored code") {

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://example.com",
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(saver.save() == CarScraper::SUCCESS_CODE);

        REQUIRE(
            saver.save() == CarScraper::IGNORED_ACTION_CODE
        );
    }

    cleanDirectories();
}


// ============================================================================
// alreadySaved()
// ============================================================================

TEST_CASE("HtmlSaver alreadySaved()", "[HtmlSaver][AlreadySaved]") {

    prepareDirectories();

    CarScraper::HtmlSaver saver(
        "page",
        "<html>Hello</html>",
        "https://example.com",
        TEST_HTML_DIR,
        TEST_LINK_DIR,
        TEST_LINK_DIR
    );

    REQUIRE_FALSE(
        saver.alreadySaved("https://example.com")
    );

    saver.save();

    REQUIRE(
        saver.alreadySaved("https://example.com")
    );

    cleanDirectories();
}


// ============================================================================
// exportSavedLink()
// ============================================================================

TEST_CASE("HtmlSaver exportSavedLink()", "[HtmlSaver][Export]") {

    prepareDirectories();

    CarScraper::HtmlSaver saver(
        "page",
        "<html>Hello</html>",
        "https://example.com",
        TEST_HTML_DIR,
        TEST_LINK_DIR,
        TEST_LINK_DIR
    );

    saver.save();

    REQUIRE(
        saver.exportSavedLink() == CarScraper::SUCCESS_CODE
    );

    REQUIRE(
        fs::exists(TEST_LINK_DIR + "saved_link.json")
    );

    std::ifstream file(
        TEST_LINK_DIR + "saved_link.json"
    );

    nlohmann::json json;
    file >> json;

    REQUIRE(json.is_array());
    REQUIRE(json.size() == 1);
    REQUIRE(json[0] == "https://example.com");

    cleanDirectories();
}


// ============================================================================
// importSavedLink()
// ============================================================================

TEST_CASE("HtmlSaver importSavedLink()", "[HtmlSaver][Import]") {

    prepareDirectories();

    SECTION("Import valid json file") {

        nlohmann::json json =
        {
            "https://example.com",
            "https://google.com"
        };

        std::ofstream file(
            TEST_LINK_DIR + "saved_link.json"
        );

        file << json.dump(4);
        file.close();

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://dummy.com",
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(
            saver.importSavedLink()
            == CarScraper::SUCCESS_CODE
        );

        REQUIRE(
            saver.alreadySaved("https://example.com")
        );

        REQUIRE(
            saver.alreadySaved("https://google.com")
        );
    }

    SECTION("Missing json file") {

        CarScraper::HtmlSaver saver(
            "page",
            "<html>Hello</html>",
            "https://dummy.com",
            TEST_HTML_DIR,
            TEST_LINK_DIR,
            TEST_LINK_DIR
        );

        REQUIRE(
            saver.importSavedLink()
            == CarScraper::ERROR_CODE
        );
    }

    cleanDirectories();
}