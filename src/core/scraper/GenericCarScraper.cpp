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
#include <filesystem>
#include <algorithm>
#include <random>
#include <string>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>


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

        // Default value
        _carBrand       = DEFAULT_STR;
        _carModel       = DEFAULT_STR;
        _outputFolder   = HTML_DIR;

        // Importing the list of already saved link
        _saver.setOutputLinkDir(_outputFolder);
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
     * @brief Sets the output folder path
     * @param outputFolder The output folder path
     * @note Checks if the directory exists
     */
    void GenericCarScraper::setOutputFolder(const std::string& outputFolder) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the folder exists
        if (fs::exists(outputFolder)) {
            _outputFolder = outputFolder;
            Logger::trace("[{}].setOutputFolder : {}", getFullId(), _outputFolder);
        } else {
            _outputFolder = HTML_DIR;
            Logger::debug("[{}].setOutputFolder : Folder {} does not exists", getFullId(), outputFolder);
        }
        _saver.setOutputLinkDir(_outputFolder);

    }


    /**
     * @brief Set the car brand
     * @param carBrand The car brand
     */
    void GenericCarScraper::setCarBrand(const std::string& carBrand) {
        _carBrand = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carBrand));
    }


    /**
     * @brief Set the car model
     * @param carModel The car model
     */
    void GenericCarScraper::setCarModel(const std::string& carModel) {
        _carModel = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carModel));
    }





    // =========================================================================
    // Internal helpers
    // =========================================================================

    /**
     * @brief Shuffler the link list to randomize acces
     */
    template<typename T>
    void GenericCarScraper::_shuffle(std::vector<T>& vector) {
        std::random_device  rd;
        std::mt19937        rng = std::mt19937(rd());
        std::ranges::shuffle(vector, rng);
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