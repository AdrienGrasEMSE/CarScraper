/**
 * @file baseExcelReader.cpp
 *
 * @brief Unit tests for CarScraper::ExcelReader class.
 *        Covers: construction, entity inheritance, excelReadLinkList().
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// Imports
#include <catch2/catch.hpp>
#include "io/ExcelReader/ExcelReader.hpp"
#include "core/utils/Constant.hpp"

using namespace CarScraper;


// =============================================================================
// Helpers
// =============================================================================

static const std::string TEST_EXCEL_PATH = "data/input_excel/ExcelReaderTest.xlsx";


// =============================================================================
// Tests — Entity inheritance
// =============================================================================

TEST_CASE("ExcelReader Entity Inheritance", "[excelreader][entity]") {

    SECTION("UUID is generated and non-empty") {
        ExcelReader reader;
        REQUIRE_FALSE(reader.getUuid().empty());
    }

    SECTION("Prefix is EXCEL_READER") {
        ExcelReader reader;
        REQUIRE(reader.getPrefix() == "EXCEL_READER");
    }

    SECTION("FullId starts with EXCEL_READER-") {
        ExcelReader reader;
        REQUIRE(reader.getFullId().rfind("EXCEL_READER-", 0) == 0);
    }

    SECTION("Two instances have different UUIDs") {
        ExcelReader r1, r2;
        REQUIRE(r1.getUuid() != r2.getUuid());
    }

}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("ExcelReader Construction", "[excelreader][construction]") {

    SECTION("Default constructor sets filePath to DEFAULT_STR") {
        ExcelReader reader;
        REQUIRE(reader.getFilePath() == DEFAULT_STR);
    }

    SECTION("Default constructor produces an empty linkList") {
        ExcelReader reader;
        REQUIRE(reader.getLinkList().empty());
    }

    SECTION("Constructor with path sets filePath correctly") {
        ExcelReader reader(TEST_EXCEL_PATH);
        REQUIRE(reader.getFilePath() == TEST_EXCEL_PATH);
    }

    SECTION("Constructor with path produces an empty linkList before read") {
        ExcelReader reader(TEST_EXCEL_PATH);
        REQUIRE(reader.getLinkList().empty());
    }

}


// =============================================================================
// Tests — Getters / Setters
// =============================================================================

TEST_CASE("ExcelReader Getters and Setters", "[excelreader][getters][setters]") {

    SECTION("setFilePath() updates the file path") {
        ExcelReader reader;
        reader.setFilePath(TEST_EXCEL_PATH);
        REQUIRE(reader.getFilePath() == TEST_EXCEL_PATH);
    }

    SECTION("setFilePath() accepts an empty string") {
        ExcelReader reader;
        reader.setFilePath("");
        REQUIRE(reader.getFilePath().empty());
    }

}


// =============================================================================
// Tests — excelReadLinkList() : valid file
// =============================================================================

TEST_CASE("ExcelReader excelReadLinkList — Valid File", "[excelreader][read]") {

    ExcelReader reader(TEST_EXCEL_PATH);
    reader.excelReadLinkList();

    SECTION("linkList is non-empty after reading") {
        REQUIRE_FALSE(reader.getLinkList().empty());
    }

    SECTION("linkList contains exactly 3 entries (rows 2 to 4)") {
        REQUIRE(reader.getLinkList().size() == 3);
    }

    SECTION("first URL points to example.com") {
        REQUIRE(reader.getLinkList()[0] == "https://example.com/");
    }

    SECTION("second URL points to example.net") {
        REQUIRE(reader.getLinkList()[1] == "https://example.net/");
    }

    SECTION("third URL points to example.org") {
        REQUIRE(reader.getLinkList()[2] == "https://example.org/");
    }

    SECTION("all URLs start with https://") {
        for (const auto& url : reader.getLinkList()) {
            REQUIRE(url.rfind("https://", 0) == 0);
        }
    }

}


// =============================================================================
// Tests — excelReadLinkList() : repeated calls
// =============================================================================

TEST_CASE("ExcelReader excelReadLinkList — Repeated Calls", "[excelreader][read]") {

    SECTION("calling excelReadLinkList() twice produces the same result") {
        ExcelReader reader(TEST_EXCEL_PATH);
        reader.excelReadLinkList();
        const auto first = reader.getLinkList();

        reader.excelReadLinkList();
        const auto second = reader.getLinkList();

        REQUIRE(first == second);
    }

    SECTION("linkList is rebuilt on each call (not accumulated)") {
        ExcelReader reader(TEST_EXCEL_PATH);
        reader.excelReadLinkList();
        reader.excelReadLinkList();
        REQUIRE(reader.getLinkList().size() == 3);
    }

}


// =============================================================================
// Tests — excelReadLinkList() : invalid file
// =============================================================================

TEST_CASE("ExcelReader excelReadLinkList — Invalid File", "[excelreader][read][error]") {

    SECTION("non-existent file produces an empty linkList (no throw)") {
        ExcelReader reader("data/input_excel/__does_not_exist__.xlsx");
        REQUIRE_NOTHROW(reader.excelReadLinkList());
        REQUIRE(reader.getLinkList().empty());
    }

    SECTION("empty filePath produces an empty linkList (no throw)") {
        ExcelReader reader;
        REQUIRE_NOTHROW(reader.excelReadLinkList());
        REQUIRE(reader.getLinkList().empty());
    }

}