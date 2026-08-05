/**
 * @file baseExcelReader.cpp
 *
 * @brief Test cases for the ExcelReader class.
 *
 * @details
 * Covers :
 *   - Default construction
 *   - Construction with path
 *   - setFilePath (valid / invalid)
 *   - _colLetterToIndex  (static helper, public)
 *   - _splitCellRef      (static helper, public)
 *   - excelReadLinkList  (requires a real .xlsx fixture)   [network]
 *   - cleanLinkList      (covered via excelReadLinkList)   [network]
 *
 * @author Adrien GRAS
 * @date 2026-06-14
 */


// Imports
#include <catch2/catch.hpp>
#include "io/excelReader/ExcelReader.hpp"
#include "core/utils/Constant.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Construction
    // =========================================================================

    TEST_CASE("ExcelReader — Default constructor", "[ExcelReader]")
    {
        ExcelReader reader;

        SECTION("filePath is DEFAULT_STR")
        {
            REQUIRE(reader.getFilePath() == DEFAULT_STR);
        }

        SECTION("linkList is empty")
        {
            REQUIRE(reader.getLinkList().empty());
        }
    }


    TEST_CASE("ExcelReader — Constructor with path", "[ExcelReader]")
    {
        SECTION("Valid path sets filePath")
        {
            ExcelReader reader(XLSX_DIR + "ExcelReaderTest.xlsx");
            REQUIRE(reader.getFilePath() == XLSX_DIR + "ExcelReaderTest.xlsx");
        }

        SECTION("Invalid path keeps DEFAULT_STR")
        {
            ExcelReader reader("data/input_excel/does_not_exist.xlsx");
            REQUIRE(reader.getFilePath() == DEFAULT_STR);
        }
    }


    // =========================================================================
    // setFilePath
    // =========================================================================

    TEST_CASE("ExcelReader — setFilePath", "[ExcelReader]")
    {
        ExcelReader reader;

        SECTION("Valid path is accepted")
        {
            reader.setFilePath(XLSX_DIR + "ExcelReaderTest.xlsx");
            REQUIRE(reader.getFilePath() == XLSX_DIR + "ExcelReaderTest.xlsx");
        }

        SECTION("Non-existent path resets to DEFAULT_STR")
        {
            reader.setFilePath("data/input_excel/ghost.xlsx");
            REQUIRE(reader.getFilePath() == DEFAULT_STR);
        }

        SECTION("Empty string resets to DEFAULT_STR")
        {
            reader.setFilePath("");
            REQUIRE(reader.getFilePath() == DEFAULT_STR);
        }
    }


    // =========================================================================
    // Static helpers
    // =========================================================================

    TEST_CASE("ExcelReader — _colLetterToIndex", "[ExcelReader]")
    {
        SECTION("Single letters")
        {
            REQUIRE(ExcelReader::_colLetterToIndex("A") == 0);
            REQUIRE(ExcelReader::_colLetterToIndex("B") == 1);
            REQUIRE(ExcelReader::_colLetterToIndex("Z") == 25);
        }

        SECTION("Double letters")
        {
            REQUIRE(ExcelReader::_colLetterToIndex("AA") == 26);
            REQUIRE(ExcelReader::_colLetterToIndex("AB") == 27);
            REQUIRE(ExcelReader::_colLetterToIndex("AZ") == 51);
            REQUIRE(ExcelReader::_colLetterToIndex("BA") == 52);
        }

        SECTION("Lowercase is handled")
        {
            REQUIRE(ExcelReader::_colLetterToIndex("a") == 0);
            REQUIRE(ExcelReader::_colLetterToIndex("z") == 25);
        }
    }


    TEST_CASE("ExcelReader — _splitCellRef", "[ExcelReader]")
    {
        SECTION("Single-letter column")
        {
            auto [col, row] = ExcelReader::_splitCellRef("A1");
            REQUIRE(col == "A");
            REQUIRE(row == "1");
        }

        SECTION("Multi-digit row")
        {
            auto [col, row] = ExcelReader::_splitCellRef("B42");
            REQUIRE(col == "B");
            REQUIRE(row == "42");
        }

        SECTION("Double-letter column")
        {
            auto [col, row] = ExcelReader::_splitCellRef("AA10");
            REQUIRE(col == "AA");
            REQUIRE(row == "10");
        }
    }


    // =========================================================================
    // excelReadLinkList + cleanLinkList  (fixture .xlsx required)
    // =========================================================================

    TEST_CASE("ExcelReader — excelReadLinkList", "[ExcelReader][network]")
    {
        SECTION("No file set — linkList stays empty")
        {
            ExcelReader reader;
            reader.excelReadLinkList();
            REQUIRE(reader.getLinkList().empty());
        }

        SECTION("linkList is not empty after reading") 
        {
            ExcelReader reader(XLSX_DIR + "ExcelReaderTest.xlsx");
            reader.excelReadLinkList();
            REQUIRE_FALSE(reader.getLinkList().empty());
        }

        SECTION("linkList resets on each call")
        {
            ExcelReader reader(XLSX_DIR + "ExcelReaderTest.xlsx");
            reader.excelReadLinkList();
            const std::size_t firstCount = reader.getLinkList().size();
            reader.excelReadLinkList();
            REQUIRE(reader.getLinkList().size() == firstCount);
        }
    }


    TEST_CASE("ExcelReader — cleanLinkList", "[ExcelReader][network]")
    {
        SECTION("Empty list stays empty")
        {
            ExcelReader reader;
            reader.cleanLinkList();
            REQUIRE(reader.getLinkList().empty());
        }

        SECTION("All links start with https:// after read + clean")
        {
            ExcelReader reader(XLSX_DIR + "ExcelReaderTest.xlsx");
            reader.excelReadLinkList();
            reader.cleanLinkList();
            for (const std::string& link : reader.getLinkList())
            {
                REQUIRE(link.substr(0, 8) == "https://");
            }
        }

        SECTION("clean reduces or keeps list size")
        {
            ExcelReader reader(XLSX_DIR + "ExcelReaderTest.xlsx");
            reader.excelReadLinkList();
            const std::size_t beforeClean = reader.getLinkList().size();
            reader.cleanLinkList();
            REQUIRE(reader.getLinkList().size() <= beforeClean);
        }
    }

} // namespace CarScraper