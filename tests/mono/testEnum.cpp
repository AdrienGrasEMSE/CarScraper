/**
 * @file testEnum.cpp
 *
 * @brief Unit tests for all enums.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include "../base/baseEnum.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    CarScraper::Logger::init("logs/test_enum.txt");
    return Catch::Session().run(argc, argv);

}