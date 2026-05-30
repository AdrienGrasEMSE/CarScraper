/**
 * @file testAll.cpp
 *
 * @brief Run all unit tests for CarScraper project.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include "base/baseLogger.cpp"
#include "base/baseEntity.cpp"
#include "base/baseValidation.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    CarScraper::Logger::init("logs/test_all.txt");
    return Catch::Session().run(argc, argv);

}