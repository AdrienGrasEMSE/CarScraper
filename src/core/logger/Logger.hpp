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

    // ============================================================
    // Logger — Wrapper singleton autour de spdlog
    // Fournit un point d'accès unique aux logs pour tout le projet.
    //
    // Niveaux disponibles : trace, debug, info, warn, error, critical
    // Sinks actifs        : stdout (coloré) + fichier rotatif (logs/)
    // ============================================================


    /**
     * Class Logger:
     * - Singleton : instance statique accessible via Logger::get()
     * - Initialisation : Logger::init(logFile) à appeler au démarrage
     * - Méthodes de log : Logger::info("Message {}", arg)
     * - Usage avancé : accès direct à l'instance spdlog pour config fine
     * 
     * Niveaux de log :
     * - trace    : informations très détaillées, utiles pour le débogage approfondi    | Un setter qui accepte une valeur
     * - debug    : informations de débogage générales, utiles pour le développement    | Valeur parsée d'un EXCEL
     * - info     : informations générales sur le déroulement de l'application          | Exécution d'une méthode importante
     * - warn     : avertissements sur des situations potentiellement problématiques    | Setter qui refuse une valeur
     * - error    : erreurs qui ne provoquent pas l'arrêt de l'application              | Requête HTTP échouée
     * - critical : erreurs critiques qui provoquent l'arrêt de l'application           | Impossible d'initialiser une dépendance critique
     */
    class Logger {

        // ============================= PRIVATE =============================
        private:
            static std::shared_ptr<spdlog::logger> _instance;

            Logger()  = delete;
            ~Logger() = delete;



            

        // ============================= PUBLIC ==============================
        public:

            // --- Initialisation ---
            static void init(const std::string& logFile = "logs/car-scraper.log");


            // --- Accès à l'instance brute (usage avancé) ---
            static std::shared_ptr<spdlog::logger> get();


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : TRACE
             */
            template<typename... Args>
            static void trace(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->trace(fmt, std::forward<Args>(args)...);
            }


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : DEBUG
             */
            template<typename... Args>
            static void debug(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->debug(fmt, std::forward<Args>(args)...);
            }


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : INFO
             */
            template<typename... Args>
            static void info(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->info(fmt, std::forward<Args>(args)...);
            }


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : WARNING
             */
            template<typename... Args>
            static void warn(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->warn(fmt, std::forward<Args>(args)...);
            }


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : ERROR
             */
            template<typename... Args>
            static void error(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->error(fmt, std::forward<Args>(args)...);
            }


            /**
             * Raccourcis pour les appels de log, redirigeant vers l'instance spdlog.
             * 
             * Type : CRITICAL
             */
            template<typename... Args>
            static void critical(fmt::format_string<Args...> fmt, Args&&... args) {
                _instance->critical(fmt, std::forward<Args>(args)...);
            }

    };

} // namespace CarScraper