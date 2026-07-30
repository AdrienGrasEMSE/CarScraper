/**
 * @file testCaradisiacMapper.cpp
 *
 * @brief Unit tests for CarScraper::CaradisiacMapper class.
 *
 * @author Adrien GRAS
 * @date 2026-07-30
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include "../base/baseCaradisiacMapper.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    CarScraper::Logger::init("logs/test_caradisiac_mapper.txt");
    return Catch::Session().run(argc, argv);

}
