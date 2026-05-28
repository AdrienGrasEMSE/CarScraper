// Imports
#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // Initialisation de l'instance statique
    std::shared_ptr<spdlog::logger> Logger::_instance = nullptr;


    // =========================================================================
    // Public methods
    // =========================================================================
    void Logger::init(const std::string& logFile)
    {
        // Sink 1 : console colorée
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();


        // Développement — tout voir jusqu'au moindre setter
        consoleSink->set_level(spdlog::level::trace);

        // Debug ciblé — on masque les TRACE des setters
        // consoleSink->set_level(spdlog::level::debug);

        // Production — seulement les anomalies et erreurs
        // consoleSink->set_level(spdlog::level::warn);


        // Sink 2 : fichier rotatif (5 Mo max, 3 fichiers conservés)
        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logFile, 1024 * 1024 * 5, 3
        );
        fileSink->set_level(spdlog::level::trace);

        _instance = std::make_shared<spdlog::logger>(
            "CarScraper",
            spdlog::sinks_init_list{ consoleSink, fileSink }
        );

        _instance->set_level(spdlog::level::trace);
        _instance->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%s:%#] %v");

        spdlog::register_logger(_instance);
    }

    std::shared_ptr<spdlog::logger> Logger::get()
    {
        return _instance;
    }

} // namespace CarScraper