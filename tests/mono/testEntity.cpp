/**
 * @file testEntity.cpp
 *
 * @brief Unit tests for CarScraper::Entity class.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "../base/baseEntity.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    return Catch::Session().run(argc, argv);

}