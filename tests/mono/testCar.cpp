/**
 * @file testCar.cpp
 *
 * @brief Unit tests for CarScraper::Car class.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Define CATCH_CONFIG_RUNNER to provide our own main function
#define CATCH_CONFIG_RUNNER


// Imports
#include <catch2/catch.hpp>
#include "../base/baseCar.cpp"


// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Catch2 runner
    return Catch::Session().run(argc, argv);

}