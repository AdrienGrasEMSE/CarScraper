/**
 * @file testHtmlParser.cpp
 *
 * @brief Unit tests for CarScraper::HtmlParser class.
 *
 * @author Adrien GRAS
 * @date 2026-06-24
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include "../base/baseHtmlParser.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    CarScraper::Logger::init("logs/test_htmlParser.txt");
    return Catch::Session().run(argc, argv);

}
