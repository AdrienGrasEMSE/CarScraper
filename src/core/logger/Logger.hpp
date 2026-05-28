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
     * Class Logger:
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

            // Singleton instance of the logger
            static std::shared_ptr<spdlog::logger> _instance;


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
             * Initializes the logger instance with a console sink and a rotating file sink.
             *
             * @param logFile The path to the log file (for the rotating file sink).
             */
            static void init(const std::string& logFile = "logs/car-scraper.log");



            /**
             * Gets the logger instance.
             *
             * @return The logger instance.
             */
            static std::shared_ptr<spdlog::logger> get();



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : TRACE
             */
            template<typename... Args>
            /**
             * Logs a message with the TRACE level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void trace(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->trace(fmt, std::forward<Args>(args)...);
            }



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : DEBUG
             */
            template<typename... Args>
            /**
             * Logs a message with the DEBUG level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void debug(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->debug(fmt, std::forward<Args>(args)...);
            }



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : INFO
             */
            template<typename... Args>
            /**
             * Logs a message with the INFO level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void info(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->info(fmt, std::forward<Args>(args)...);
            }



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : WARNING
             */
            template<typename... Args>
            /**
             * Logs a message with the WARNING level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void warn(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->warn(fmt, std::forward<Args>(args)...);
            }



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : ERROR
             */
            template<typename... Args>
            /**
             * Logs a message with the ERROR level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void error(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->error(fmt, std::forward<Args>(args)...);
            }



            /**
             * Shortcuts for logging calls, redirecting to the spdlog instance.
             * 
             * Type : CRITICAL
             */
            template<typename... Args>
            /**
             * Logs a message with the CRITICAL level.
             *
             * @param fmt The format string.
             * @param args The arguments for the format string.
             */
            static void critical(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->critical(fmt, std::forward<Args>(args)...);
            }

    };

} // namespace CarScraper