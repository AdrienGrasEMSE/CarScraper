/**
 * @file Logger.cpp
 * 
 * @brief This file implements the Logger class, which provides a simple logging interface.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 */


// Imports
#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // Static instance initialization
    std::shared_ptr<spdlog::logger> Logger::_instance = nullptr;


    // =========================================================================
    // Public methods
    // =========================================================================

    /**
     * @brief Initializes the logger instance with a console sink and a rotating file sink.
     *
     * @param logFile The path to the log file (for the rotating file sink).
     */
    void Logger::init(const std::string& logFile)
    {

        // Security
        if (_instance != nullptr) {
            return;
        }


        // ----------------------------------------------------------------------
        // Sink 1 : console output
        // ----------------------------------------------------------------------
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        
        // Trace - all messages (including debug and info)
        consoleSink->set_level(spdlog::level::trace);

        // Debug - debug, info, warning, error and critical messages
        // consoleSink->set_level(spdlog::level::debug);

        // Production - only warnings and errors
        // consoleSink->set_level(spdlog::level::warn);




        
        // ----------------------------------------------------------------------
        // Sink 2: rotating file (5 MB max, 3 files retained)
        // ----------------------------------------------------------------------
        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logFile, 1024 * 1024 * 5, 3
        );
        fileSink->set_level(spdlog::level::trace);


        // Create the logger instance with both sinks
        _instance = std::make_shared<spdlog::logger>(
            "CarScraper",
            spdlog::sinks_init_list{ consoleSink, fileSink }
        );


        // Trace - all messages (including debug and info)
        _instance->set_level(spdlog::level::trace);

        // Debug - debug, info, warning, error and critical messages
        // _instance->set_level(spdlog::level::debug);

        // Production - only warnings and errors
        // _instance->set_level(spdlog::level::warn);


        // Set log pattern
        _instance->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%s:%#] %v");


        // Register the logger instance
        spdlog::register_logger(_instance);

    }


    /**
     * @brief Gets the logger instance.
     *
     * @return The logger instance.
     */
    std::shared_ptr<spdlog::logger> Logger::get()
    {
        return _instance;
    }


    /**
     * @brief Safely gets the logger instance.
     *
     * @return The logger instance even if not initialized.
     */
    spdlog::logger* Logger::_safeGet() {

        // If the logger instance is not initialized
        if (!_instance) {

            // Automatic initialization with default log file path
            init();

        }
        return _instance.get();
    }

} // namespace CarScraper