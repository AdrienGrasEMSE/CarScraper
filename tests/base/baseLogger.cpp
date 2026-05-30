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

TEST_CASE("Logger default initialization", "[logger]") {

    CarScraper::Logger::reset();                            // default state

    CarScraper::Logger::init();
    REQUIRE(CarScraper::Logger::get() != nullptr);

}


TEST_CASE("Logger initialization with custom file", "[logger]") {

    CarScraper::Logger::reset();

    CarScraper::Logger::init("logs/test_log.txt");
    REQUIRE(CarScraper::Logger::get() != nullptr);

}


TEST_CASE("Logger auto-initialization via _safeGet", "[logger]") {

    CarScraper::Logger::reset();                            // _instance == nullptr

    CarScraper::Logger::trace("auto-init test");            // trigger _safeGet → init()
    REQUIRE(CarScraper::Logger::get() != nullptr);          // must be initialized now

}


TEST_CASE("Logger double initialization is idempotent", "[logger]") {

    CarScraper::Logger::reset();

    CarScraper::Logger::init();
    auto first = CarScraper::Logger::get();

    CarScraper::Logger::init();                             // second init should do nothing
    auto second = CarScraper::Logger::get();

    REQUIRE(first == second);                               // same instance, no crash

}


TEST_CASE("Logger is nullptr before any init", "[logger]") {

    CarScraper::Logger::reset();

    REQUIRE(CarScraper::Logger::get() == nullptr);          // real default state is nullptr

}