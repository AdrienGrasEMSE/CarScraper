/**
 * @file testExcelReader.cpp
 *
 * @brief Unit tests for CarScraper::ExcelReader class.
 *        Covers: construction, entity inheritance, excelReadLinkList().
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    CarScraper::Logger::init("logs/test_excelReader.txt");
    return Catch::Session().run(argc, argv);

}