/**
 * @file Logger.hpp
 * 
 * @brief This file declares the Logger class, which provides a simple logging interface.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 */


// For only one include
#pragma once


// Imports
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class Logger
     * 
     * @brief A simple logger class using spdlog.
     * @details This class provides static methods for logging messages at different levels (trace, debug, info, warn, error, critical).
     *
     * Log levels :
     * - trace    : detailed information, useful for debugging              | A setter that accepts a value
     * - debug    : general debugging information, useful for development   | Parsed value from an Excel file
     * - info     : general information about the application's operation   | Execution of an important method
     * - warn     : warnings about potentially problematic situations       | A setter that refuses a value
     * - error    : errors that do not cause the application to stop        | Failed HTTP request
     * - critical : critical errors that cause the application to stop      | Unable to initialize a critical dependency
     */
    class Logger {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            /**
             * The singleton instance of the logger.
             */
            static std::shared_ptr<spdlog::logger> _instance;


            /**
             * @brief Safely gets the logger instance.
             *
             * @return The logger instance.
             */
            static spdlog::logger* _safeGet();


            // -------------------------------------------------------------------------
            // Constructor / Destructor : private to enforce singleton pattern
            // -------------------------------------------------------------------------
            Logger()  = delete;
            ~Logger() = delete;



            

        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            /**
             * @brief Initializes the logger instance with a console sink and a rotating file sink.
             *
             * @param logFile The path to the log file (for the rotating file sink).
             */
            static void init(const std::string& logFile = "logs/log.txt");


            /**
             * @brief Gets the logger instance.
             *
             * @return The logger instance.
             */
            static std::shared_ptr<spdlog::logger> get();


            /**
             * @brief Logs a message with the TRACE level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void trace(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->trace(fmt, std::forward<Args>(args)...);
            }


            /**
             * @brief Logs a message with the DEBUG level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void debug(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->debug(fmt, std::forward<Args>(args)...);
            }


            /**
             * @brief Logs a message with the INFO level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void info(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->info(fmt, std::forward<Args>(args)...);
            }


            /**
             * @brief Logs a message with the WARNING level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void warn(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->warn(fmt, std::forward<Args>(args)...);
            }


            /**
             * @brief Logs a message with the ERROR level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void error(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->error(fmt, std::forward<Args>(args)...);
            }


            /**
             * @brief Logs a message with the CRITICAL level.
             *
             * @tparam Args The types of the arguments for the format string.
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            template<typename... Args>
            static void critical(fmt::format_string<Args...> fmt, Args&&... args) {
                if (auto* l = _safeGet()) l->critical(fmt, std::forward<Args>(args)...);
            }


            // -------------------------------------------------------------------------
            // Testing utilities
            // -------------------------------------------------------------------------
            #ifdef TESTING

                /**
                * @brief Resets the logger instance (for testing purposes only).
                */
                static void reset();

            #endif

    };

} // namespace CarScraper