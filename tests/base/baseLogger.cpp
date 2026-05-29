/**
 * @file baseLogger.cpp
 *
 * @brief Unit tests for CarScraper::Logger class.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"


// =============================================================================
// Tests — Logger
// =============================================================================


TEST_CASE("Logger Default Initialization", "[logger]") {

    CarScraper::Logger::init();
    REQUIRE(CarScraper::Logger::get() != nullptr);

}


TEST_CASE("Logger Initialization with File", "[logger]") {

    CarScraper::Logger::init("logs/test_log.txt");
    REQUIRE(CarScraper::Logger::get() != nullptr);

}


TEST_CASE("Logger Auto-Initialization", "[logger]") {

    CarScraper::Logger::trace("While no init");
    REQUIRE(CarScraper::Logger::get() != nullptr);

}


TEST_CASE("Logger Double Initialization", "[logger]") {

    CarScraper::Logger::init();
    CarScraper::Logger::init();
    REQUIRE(CarScraper::Logger::get() != nullptr);

}