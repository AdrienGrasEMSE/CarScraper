/**
 * @file baseGenericCarMapper.cpp
 *
 * @brief Unit tests for CarScraper::GenericCarMapper class.
 *        Since GenericCarMapper's constructors and internal helpers (cleanNumber,
 *        toInt, toDouble, readFile) are protected, tests instantiate a minimal
 *        concrete subclass (TestMapper) that exposes them publicly for testing.
 *
 * @author Adrien GRAS
 * @date 2026-07-16
 */


// Imports
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include "core/mappers/GenericCarMapper.hpp"
#include "core/utils/Constant.hpp"

using namespace CarScraper;
namespace fs = std::filesystem;


// =============================================================================
// Minimal concrete subclass for testing
// =============================================================================

/**
 * @class TestMapper
 * @brief Minimal concrete subclass of GenericCarMapper used only in unit tests.
 *        Exposes the protected constructors and internal helpers publicly.
 */
class TestMapper : public CarScraper::GenericCarMapper {
public:
    TestMapper()                          : GenericCarMapper("TEST-MAPPER") {}
    TestMapper(const std::string& prefix) : GenericCarMapper(prefix) {}

    // Public wrappers around protected internal helpers
    std::string cleanNumberPublic(std::string value) { return cleanNumber(value); }
    int         toIntPublic(std::optional<std::string>& str)    { return toInt(str); }
    double      toDoublePublic(std::optional<std::string>& str) { return toDouble(str); }
    std::string readFilePublic(const std::string& path) { return readFile(path); }
    int         mapToCar() override {return 0;}
};


// =============================================================================
// Fixture helpers
// =============================================================================

static const std::string TEST_DIR_GENERIC = "data/test_mapper/";

static void prepareDirectory() {
    fs::create_directories(TEST_DIR_GENERIC);
}

static void cleanDirectoryGeneric() {
    fs::remove_all(TEST_DIR_GENERIC);
}

static std::string writeFixtureFileGeneric(const std::string& name, const std::string& content) {
    prepareDirectory();
    const std::string path = TEST_DIR_GENERIC + name;
    std::ofstream file(path);
    file << content;
    file.close();
    return path;
}


// =============================================================================
// Tests — Entity inheritance
// =============================================================================

TEST_CASE("GenericCarMapper Entity Inheritance", "[genericmapper][entity]") {

    SECTION("Custom prefix is applied") {
        TestMapper mapper("ARGUS-MAPPER");
        REQUIRE(mapper.getPrefix() == "ARGUS-MAPPER");
    }

    SECTION("Default TestMapper prefix is TEST-MAPPER") {
        TestMapper mapper;
        REQUIRE(mapper.getPrefix() == "TEST-MAPPER");
    }

    SECTION("Default GenericCarMapper prefix (no arg) is MAPPER") {
        class DefaultMapper : public GenericCarMapper {
            public: DefaultMapper() : GenericCarMapper() {}
            int mapToCar() override {return 0;}
        };
        DefaultMapper mapper;
        REQUIRE(mapper.getPrefix() == "MAPPER");
    }

    SECTION("UUID is generated and non-empty") {
        TestMapper mapper;
        REQUIRE_FALSE(mapper.getUuid().empty());
    }

    SECTION("FullId starts with prefix") {
        TestMapper mapper("ARGUS-MAPPER");
        REQUIRE(mapper.getFullId().rfind("ARGUS-MAPPER-", 0) == 0);
    }

    SECTION("Two mappers have different UUIDs") {
        TestMapper a, b;
        REQUIRE(a.getUuid() != b.getUuid());
    }
}


// =============================================================================
// Tests — Construction
// =============================================================================

TEST_CASE("GenericCarMapper Construction", "[genericmapper][construction]") {

    SECTION("Construction succeeds without throwing") {
        REQUIRE_NOTHROW(TestMapper());
    }

    SECTION("Construction with custom prefix succeeds without throwing") {
        REQUIRE_NOTHROW(TestMapper("CUSTOM-MAPPER"));
    }

    SECTION("inputFile is DEFAULT_STR after construction") {
        TestMapper mapper;
        REQUIRE(mapper.getInputFile() == DEFAULT_STR);
    }

    SECTION("car is default-constructed and not complete") {
        TestMapper mapper;
        REQUIRE_FALSE(mapper.getCar().isComplete());
    }

    SECTION("car is a genuine Car entity with the CAR prefix") {
        TestMapper mapper;
        REQUIRE(mapper.getCar().getPrefix() == "CAR");
    }

    SECTION("car has its own independent UUID from the mapper") {
        TestMapper mapper;
        REQUIRE(mapper.getCar().getUuid() != mapper.getUuid());
    }
}


// =============================================================================
// Tests — setInputFile
// =============================================================================

TEST_CASE("GenericCarMapper setInputFile", "[genericmapper][setter][inputfile]") {

    SECTION("Valid existing path is accepted and returns true") {
        const std::string path = writeFixtureFileGeneric("sheet.html", "<html></html>");
        TestMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        REQUIRE(mapper.getInputFile() == path);
        cleanDirectoryGeneric();
    }

    SECTION("Non-existent path is rejected and returns false") {
        TestMapper mapper;
        REQUIRE_FALSE(mapper.setInputFile("data/test_mapper/does_not_exist.html"));
        REQUIRE(mapper.getInputFile() == DEFAULT_STR);
    }

    SECTION("Empty string is rejected and returns false") {
        TestMapper mapper;
        REQUIRE_FALSE(mapper.setInputFile(""));
        REQUIRE(mapper.getInputFile() == DEFAULT_STR);
    }

    SECTION("Setting an invalid path after a valid one resets to DEFAULT_STR") {
        const std::string path = writeFixtureFileGeneric("sheet.html", "<html></html>");
        TestMapper mapper;
        REQUIRE(mapper.setInputFile(path));
        REQUIRE(mapper.getInputFile() == path);

        REQUIRE_FALSE(mapper.setInputFile("data/test_mapper/ghost.html"));
        REQUIRE(mapper.getInputFile() == DEFAULT_STR);
        cleanDirectoryGeneric();
    }

    SECTION("setInputFile does not throw") {
        TestMapper mapper;
        REQUIRE_NOTHROW(mapper.setInputFile("data/test_mapper/ghost.html"));
    }
}


// =============================================================================
// Tests — cleanNumber
// =============================================================================

TEST_CASE("GenericCarMapper cleanNumber", "[genericmapper][helper][cleanNumber]") {

    SECTION("Replaces a comma with a dot") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("1234,56") == "1234.56");
    }

    SECTION("Removes spaces (thousands separator)") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("1 234") == "1234");
    }

    SECTION("Combines comma replacement and space removal") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("1 234,56") == "1234.56");
    }

    SECTION("Leaves an already-clean string unchanged") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("1234.56") == "1234.56");
    }

    SECTION("Handles multiple internal spaces") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("1 2 3 4") == "1234");
    }

    SECTION("Empty string stays empty") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("").empty());
    }

    SECTION("Non-numeric characters other than space/comma are left untouched") {
        TestMapper mapper;
        REQUIRE(mapper.cleanNumberPublic("12,5€") == "12.5\u20ac");
    }
}


// =============================================================================
// Tests — toInt
// =============================================================================

TEST_CASE("GenericCarMapper toInt", "[genericmapper][helper][toInt]") {

    SECTION("nullopt returns ERROR_INT") {
        TestMapper mapper;
        std::optional<std::string> str = std::nullopt;
        REQUIRE(mapper.toIntPublic(str) == ERROR_INT);
    }

    SECTION("Simple valid integer string is parsed") {
        TestMapper mapper;
        std::optional<std::string> str = "123";
        REQUIRE(mapper.toIntPublic(str) == 123);
    }

    SECTION("Thousands separator (space) is handled") {
        TestMapper mapper;
        std::optional<std::string> str = "1 234";
        REQUIRE(mapper.toIntPublic(str) == 1234);
    }

    SECTION("Negative numbers are handled") {
        TestMapper mapper;
        std::optional<std::string> str = "-15";
        REQUIRE(mapper.toIntPublic(str) == -15);
    }

    SECTION("Purely non-numeric string returns ERROR_INT") {
        TestMapper mapper;
        std::optional<std::string> str = "abc";
        REQUIRE(mapper.toIntPublic(str) == ERROR_INT);
    }

    SECTION("Empty string returns ERROR_INT") {
        TestMapper mapper;
        std::optional<std::string> str = "";
        REQUIRE(mapper.toIntPublic(str) == ERROR_INT);
    }

    SECTION("Trailing non-numeric characters are silently truncated (std::stoi behavior)") {
        // "45,90" -> cleanNumber -> "45.90" -> std::stoi stops at the first non-digit
        TestMapper mapper;
        std::optional<std::string> str = "45,90";
        REQUIRE(mapper.toIntPublic(str) == 45);
    }

    SECTION("The passed-in optional is mutated in place by cleanNumber") {
        TestMapper mapper;
        std::optional<std::string> str = "1 234";
        mapper.toIntPublic(str);
        REQUIRE(str.value() == "1234");
    }
}


// =============================================================================
// Tests — toDouble
// =============================================================================

TEST_CASE("GenericCarMapper toDouble", "[genericmapper][helper][toDouble]") {

    SECTION("nullopt returns ERROR_DOUBLE") {
        TestMapper mapper;
        std::optional<std::string> str = std::nullopt;
        REQUIRE(mapper.toDoublePublic(str) == ERROR_DOUBLE);
    }

    SECTION("Simple valid double string is parsed") {
        TestMapper mapper;
        std::optional<std::string> str = "12.5";
        REQUIRE(mapper.toDoublePublic(str) == Approx(12.5));
    }

    SECTION("French comma decimal separator is converted") {
        TestMapper mapper;
        std::optional<std::string> str = "12,5";
        REQUIRE(mapper.toDoublePublic(str) == Approx(12.5));
    }

    SECTION("Thousands separator and comma decimal are both handled") {
        TestMapper mapper;
        std::optional<std::string> str = "1 234,56";
        REQUIRE(mapper.toDoublePublic(str) == Approx(1234.56));
    }

    SECTION("Integer-only string is parsed as a whole double") {
        TestMapper mapper;
        std::optional<std::string> str = "100";
        REQUIRE(mapper.toDoublePublic(str) == Approx(100.0));
    }

    SECTION("Purely non-numeric string returns ERROR_DOUBLE") {
        TestMapper mapper;
        std::optional<std::string> str = "abc";
        REQUIRE(mapper.toDoublePublic(str) == ERROR_DOUBLE);
    }

    SECTION("Empty string returns ERROR_DOUBLE") {
        TestMapper mapper;
        std::optional<std::string> str = "";
        REQUIRE(mapper.toDoublePublic(str) == ERROR_DOUBLE);
    }
}


// =============================================================================
// Tests — readFile
// =============================================================================

TEST_CASE("GenericCarMapper readFile", "[genericmapper][helper][readFile]") {

    SECTION("Returns the exact content of an existing file") {
        const std::string path = writeFixtureFileGeneric("content.txt", "Hello, CarScraper!");
        TestMapper mapper;
        REQUIRE(mapper.readFilePublic(path) == "Hello, CarScraper!");
        cleanDirectoryGeneric();
    }

    SECTION("Preserves multi-line content") {
        const std::string path = writeFixtureFileGeneric("multiline.txt", "line1\nline2\nline3");
        TestMapper mapper;
        REQUIRE(mapper.readFilePublic(path) == "line1\nline2\nline3");
        cleanDirectoryGeneric();
    }

    SECTION("Returns an empty string for an empty file") {
        const std::string path = writeFixtureFileGeneric("empty.txt", "");
        TestMapper mapper;
        REQUIRE(mapper.readFilePublic(path).empty());
        cleanDirectoryGeneric();
    }

    SECTION("Returns ERROR_STR for a non-existent file") {
        TestMapper mapper;
        REQUIRE(mapper.readFilePublic("data/test_mapper/does_not_exist.txt") == ERROR_STR);
    }
}


// =============================================================================
// Tests — mapToCar (base implementation)
// =============================================================================

TEST_CASE("GenericCarMapper mapToCar", "[genericmapper][mapToCar]") {

    SECTION("Base mapToCar does not throw") {
        TestMapper mapper;
        REQUIRE_NOTHROW(mapper.mapToCar());
    }

    SECTION("Base mapToCar does not populate the car") {
        TestMapper mapper;
        mapper.mapToCar();
        REQUIRE_FALSE(mapper.getCar().isComplete());
    }
    
}
