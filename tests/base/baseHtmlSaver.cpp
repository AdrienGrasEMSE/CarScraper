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
#include "io/HtmlSaver/HtmlSaver.hpp"


// =============================================================================
// Helpers
// =============================================================================


namespace fs = std::filesystem;

// Temporary output directory used across all tests (cleaned up after each section)
static const std::string TEST_OUTPUT_DIR = "data/test_save_html/";

// Helper : reads the content of a file and returns it as a string
static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Helper : removes the test output directory after a test
static void cleanTestDir() {
    fs::remove_all(TEST_OUTPUT_DIR);
}




// =============================================================================
// Tests — Constructor
// =============================================================================

TEST_CASE("HtmlSaver Constructor", "[htmlsaver][constructor]") {

    SECTION("name is correctly set") {
        CarScraper::HtmlSaver saver("test_page", "<html/>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getName() == "test_page");
    }

    SECTION("content is correctly set") {
        CarScraper::HtmlSaver saver("test_page", "<html><body>Hello</body></html>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getContent() == "<html><body>Hello</body></html>");
    }

    SECTION("outputDir is correctly set when provided") {
        CarScraper::HtmlSaver saver("test_page", "<html/>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getOutputDir() == TEST_OUTPUT_DIR);
    }

    SECTION("outputDir uses default value when not provided") {
        CarScraper::HtmlSaver saver("test_page", "<html/>");
        REQUIRE(saver.getOutputDir() == "data/save_html/");
    }

    SECTION("Entity prefix is HTML-SAVER") {
        CarScraper::HtmlSaver saver("test_page", "<html/>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getPrefix() == "HTML-SAVER");
    }

    SECTION("getFullId() starts with HTML-SAVER-") {
        CarScraper::HtmlSaver saver("test_page", "<html/>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getFullId().rfind("HTML-SAVER-", 0) == 0);
    }
}




// =============================================================================
// Tests — Getters / Setters
// =============================================================================

TEST_CASE("HtmlSaver Getters and Setters", "[htmlsaver][getters][setters]") {

    SECTION("setName() updates the name") {
        CarScraper::HtmlSaver saver("old_name", "<html/>", TEST_OUTPUT_DIR);
        saver.setName("new_name");
        REQUIRE(saver.getName() == "new_name");
    }

    SECTION("setContent() updates the content") {
        CarScraper::HtmlSaver saver("page", "<html/>", TEST_OUTPUT_DIR);
        saver.setContent("<html><body>Updated</body></html>");
        REQUIRE(saver.getContent() == "<html><body>Updated</body></html>");
    }

    SECTION("setOutputDir() updates the output directory") {
        CarScraper::HtmlSaver saver("page", "<html/>", TEST_OUTPUT_DIR);
        saver.setOutputDir("data/other_dir/");
        REQUIRE(saver.getOutputDir() == "data/other_dir/");
    }

    SECTION("setContent() accepts empty string") {
        CarScraper::HtmlSaver saver("page", "<html/>", TEST_OUTPUT_DIR);
        saver.setContent("");
        REQUIRE(saver.getContent().empty());
    }
}




// =============================================================================
// Tests — getFilePath()
// =============================================================================

TEST_CASE("HtmlSaver getFilePath", "[htmlsaver][filepath]") {

    SECTION("getFilePath() returns outputDir + name + .txt") {
        CarScraper::HtmlSaver saver("my_page", "<html/>", "data/save_html/");
        REQUIRE(saver.getFilePath() == "data/save_html/my_page.txt");
    }

    SECTION("getFilePath() reflects a name change via setName()") {
        CarScraper::HtmlSaver saver("old", "<html/>", TEST_OUTPUT_DIR);
        saver.setName("new");
        REQUIRE(saver.getFilePath() == TEST_OUTPUT_DIR + "new.txt");
    }

    SECTION("getFilePath() reflects a directory change via setOutputDir()") {
        CarScraper::HtmlSaver saver("page", "<html/>", "data/save_html/");
        saver.setOutputDir("data/other/");
        REQUIRE(saver.getFilePath() == "data/other/page.txt");
    }

    SECTION("getFilePath() is stable across multiple calls") {
        CarScraper::HtmlSaver saver("stable", "<html/>", TEST_OUTPUT_DIR);
        REQUIRE(saver.getFilePath() == saver.getFilePath());
    }
}




// =============================================================================
// Tests — save()
// =============================================================================

TEST_CASE("HtmlSaver save", "[htmlsaver][save]") {

    SECTION("save() returns the correct file path") {
        CarScraper::HtmlSaver saver("save_test", "<html/>", TEST_OUTPUT_DIR);
        const std::string returned = saver.save();
        REQUIRE(returned == TEST_OUTPUT_DIR + "save_test.txt");
        cleanTestDir();
    }

    SECTION("save() creates the file on disk") {
        CarScraper::HtmlSaver saver("existence_test", "<html/>", TEST_OUTPUT_DIR);
        saver.save();
        REQUIRE(fs::exists(TEST_OUTPUT_DIR + "existence_test.txt"));
        cleanTestDir();
    }

    SECTION("save() writes content correctly to the file") {
        const std::string expected = "<html><body><p>Hello World</p></body></html>";
        CarScraper::HtmlSaver saver("content_test", expected, TEST_OUTPUT_DIR);
        saver.save();
        REQUIRE(readFile(TEST_OUTPUT_DIR + "content_test.txt") == expected);
        cleanTestDir();
    }

    SECTION("save() creates the output directory if it does not exist") {
        const std::string newDir = "data/test_new_dir_htmlsaver/";
        fs::remove_all(newDir);
        CarScraper::HtmlSaver saver("dir_test", "<html/>", newDir);
        saver.save();
        REQUIRE(fs::exists(newDir));
        fs::remove_all(newDir);
    }

    SECTION("save() overwrites an existing file with new content") {
        CarScraper::HtmlSaver saver("overwrite_test", "first content", TEST_OUTPUT_DIR);
        saver.save();
        saver.setContent("second content");
        saver.save();
        REQUIRE(readFile(TEST_OUTPUT_DIR + "overwrite_test.txt") == "second content");
        cleanTestDir();
    }

    SECTION("save() handles empty content without throwing") {
        CarScraper::HtmlSaver saver("empty_test", "", TEST_OUTPUT_DIR);
        REQUIRE_NOTHROW(saver.save());
        REQUIRE(readFile(TEST_OUTPUT_DIR + "empty_test.txt").empty());
        cleanTestDir();
    }

    SECTION("save() handles large content correctly") {
        const std::string large(100000, 'x');
        CarScraper::HtmlSaver saver("large_test", large, TEST_OUTPUT_DIR);
        saver.save();
        REQUIRE(readFile(TEST_OUTPUT_DIR + "large_test.txt") == large);
        cleanTestDir();
    }

    SECTION("save() called twice produces two separate files for different names") {
        CarScraper::HtmlSaver s1("file_a", "content A", TEST_OUTPUT_DIR);
        CarScraper::HtmlSaver s2("file_b", "content B", TEST_OUTPUT_DIR);
        s1.save();
        s2.save();
        REQUIRE(readFile(TEST_OUTPUT_DIR + "file_a.txt") == "content A");
        REQUIRE(readFile(TEST_OUTPUT_DIR + "file_b.txt") == "content B");
        cleanTestDir();
    }
}