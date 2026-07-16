/**
 * @file baseLogger.cpp
 *
 * @brief Unit tests for CarScraper::Logger class.
 *        Covers: initialization, idempotency, auto-init, and one dedicated
 *                test per log level (trace/debug/info/warn/error/critical)
 *                verifying the formatted message is actually written to the
 *                log file — not just that the call does not throw.
 *
 * @author Adrien GRAS
 * @date 2026-07-16
 */


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include <fstream>
#include <sstream>


// =============================================================================
// Helpers
// =============================================================================

/**
 * Reads the full content of a file as a string.
 * @details Used to inspect what the file sink actually wrote, since
 *          Logger exposes no in-memory sink or getter for the last message.
 */
static std::string readLogFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


// =============================================================================
// Tests — Initialization
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




// =============================================================================
// Tests — Log levels (formatted message actually written to file)
// =============================================================================

TEST_CASE("Logger log levels write their formatted message to the log file", "[logger][levels]") {

    SECTION("trace level logs a formatted message") {
        const std::string logFile = "logs/test_log_trace.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::trace("trace value = {}", 1);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[trace]") != std::string::npos);
        REQUIRE(content.find("trace value = 1") != std::string::npos);
    }


    SECTION("debug level logs a formatted message") {
        const std::string logFile = "logs/test_log_debug.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::debug("debug value = {}", 2);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[debug]") != std::string::npos);
        REQUIRE(content.find("debug value = 2") != std::string::npos);
    }


    SECTION("info level logs a formatted message") {
        const std::string logFile = "logs/test_log_info.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::info("info value = {}", 3);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[info]") != std::string::npos);
        REQUIRE(content.find("info value = 3") != std::string::npos);
    }


    SECTION("warn level logs a formatted message") {
        const std::string logFile = "logs/test_log_warn.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::warn("warn value = {}", 4);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[warning]") != std::string::npos);   // spdlog label for warn() is "warning"
        REQUIRE(content.find("warn value = 4") != std::string::npos);
    }


    SECTION("error level logs a formatted message") {
        const std::string logFile = "logs/test_log_error.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::error("error value = {}", 5);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[error]") != std::string::npos);
        REQUIRE(content.find("error value = 5") != std::string::npos);
    }


    SECTION("critical level logs a formatted message") {
        const std::string logFile = "logs/test_log_critical.txt";
        CarScraper::Logger::reset();
        CarScraper::Logger::init(logFile);

        CarScraper::Logger::critical("critical value = {}", 6);
        CarScraper::Logger::get()->flush();

        const std::string content = readLogFile(logFile);
        REQUIRE(content.find("[critical]") != std::string::npos);
        REQUIRE(content.find("critical value = 6") != std::string::npos);
    }

}


TEST_CASE("Logger formats multiple arguments correctly", "[logger][levels]") {

    const std::string logFile = "logs/test_log_multiarg.txt";
    CarScraper::Logger::reset();
    CarScraper::Logger::init(logFile);

    CarScraper::Logger::info("[{}].setBrand : {} -> {}", "CAR-123", "old", "new");
    CarScraper::Logger::get()->flush();

    const std::string content = readLogFile(logFile);
    REQUIRE(content.find("[CAR-123].setBrand : old -> new") != std::string::npos);

}