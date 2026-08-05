/**
 * @file GenericCarScraper.cpp
 * 
 * @brief This file implements the GenericCarScraper class, which represents a car oriented scraper.
 *
 * @author Adrien GRAS
 * @date 2026-06-28
 */


// Imports
#include "GenericCarScraper.hpp"
#include "core/utils/Constant.hpp"
#include "core/logger/Logger.hpp"
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <filesystem>
#include <string>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default Constructor : define prefix as SCRAPER
     */
    GenericCarScraper::GenericCarScraper() : GenericCarScraper("SCRAPER") {}


    /**
     * @brief Constructor which define the scraper prefix
     * @param prefix The prefix for the full id
     */
    GenericCarScraper::GenericCarScraper(const std::string& prefix) : Entity(prefix) {

        // Default text case
        _carBrandCase = CarScraper::TextCase::LOWER;
        _carModelCase = CarScraper::TextCase::LOWER;

        // Default value
        _carBrand           = DEFAULT_STR;
        _carModel           = DEFAULT_STR;
        _outputDirectory    = HTML_DIR;

        // Importing the list of already saved link
        _saver.setLinkFile(LINK_FILE);
        _saver.importSavedLink();

        // Anti-block policy tailored for discrete scraping
        AntiBlockPolicy policy;
        policy.minDelayBetweenRequests  = std::chrono::milliseconds(8000);
        policy.maxDelayBetweenRequests  = std::chrono::milliseconds(30000);
        policy.maxRetries               = 2;
        policy.retryOn429               = true;
        policy.retryOn503               = true;
        policy.retryBaseDelay           = std::chrono::milliseconds(60000);
        policy.rotateUserAgent          = true;
        policy.rotateProxy              = false;
        policy.sendAcceptHeaders        = true;
        policy.sendReferer              = true;
        _client.setPolicy(policy);

    }


    /**
     * @brief Default destructor
     */
    GenericCarScraper::~GenericCarScraper() {

        // Exporting the list of already saved link
        _saver.exportSavedLink();

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the output directory path
     * @param outputDirectory The output directory path
     * @note Checks if the directory exists
     */
    void GenericCarScraper::setOutputDirectory(const std::string& outputDirectory) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the directory exists
        if (fs::exists(outputDirectory)) {
            _outputDirectory = outputDirectory;
            Logger::trace("[{}].setOutputDirectory : {}", getFullId(), _outputDirectory);
        } else {
            _outputDirectory = HTML_DIR;
            Logger::debug("[{}].setOutputDirectory : Directory {} does not exists", getFullId(), outputDirectory);
        }

    }


    /**
     * @brief Sets the car brand case
     * @param carBrandCase The car brand case
     */
    void GenericCarScraper::setCarBrandCase(CarScraper::TextCase carBrandCase) {

       // Set value
        _carBrandCase = carBrandCase;


        // Log message
        if (_carBrandCase == CarScraper::TextCase::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CarBrandCase");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "CarBrandCase", CarScraper::textCaseToString(_carBrandCase));
        }

    }


    /** 
     * @brief Sets the car model case
     * @param carModelCase The car model case
     */
    void GenericCarScraper::setCarModelCase(CarScraper::TextCase carModelCase) {

       // Set value
        _carModelCase = carModelCase;


        // Log message
        if (_carModelCase == CarScraper::TextCase::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CarModelCase");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "CarModelCase", CarScraper::textCaseToString(_carModelCase));
        }

    }


    /**
     * @brief Set the car brand
     * @param carBrand The car brand
     * @note Applies the case defined by _carBrandCase
     */
    void GenericCarScraper::setCarBrand(const std::string& carBrand) {

        // Applying case
        if (_carBrandCase == TextCase::TITLE) {
            _carBrand = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carBrand));
        }
        else if (_carBrandCase == TextCase::UPPER) {
            _carBrand = una::cases::to_uppercase_utf8(una::norm::to_nfc_utf8(carBrand));
        }
        else {
            _carBrand = una::cases::to_lowercase_utf8(una::norm::to_nfc_utf8(carBrand));
        }

    }


    /**
     * @brief Set the car model
     * @param carModel The car model
     * @note Applies the case defined by _carModelCase
     */
    void GenericCarScraper::setCarModel(const std::string& carModel) {

        // Applying case
        if (_carModelCase == TextCase::TITLE) {
            _carModel = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carModel));
        }
        else if (_carModelCase == TextCase::UPPER) {
            _carModel = una::cases::to_uppercase_utf8(una::norm::to_nfc_utf8(carModel));
        }
        else {
            _carModel = una::cases::to_lowercase_utf8(una::norm::to_nfc_utf8(carModel));
        }

    }






    // =========================================================================
    // Internal Helpers
    // =========================================================================

    /**
     * @brief Checks whether a given full URL may be fetched according to the
     *        target host's robots.txt ("*" group).
     * @param host The domain (e.g. "www.largus.fr").
     * @param path The path to check (e.g. "/fiche-technique/Peugeot/208.html").
     * @return true if allowed (or if robots.txt is unreachable/absent), false if disallowed.
     * @note Logs a warning when a link is skipped because of a robots.txt rule.
     */
    bool GenericCarScraper::_isPathAllowed(const std::string& host, const std::string& path) {
        bool allowed = _robots.isAllowed(host, path);
        if (!allowed) {
            Logger::warn("[{}]._isPathAllowed({}, {}) → skipped, disallowed by robots.txt",
                getFullId(), host, path);
        }
        return allowed;
    }






    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Scrap a car model on the targeted website
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note Generic method to implement
     */
    int GenericCarScraper::scrapModel() {

        Logger::error("[{}].scrapModel : to implement", getFullId());
        return IGNORED_ACTION_CODE;

    }


}