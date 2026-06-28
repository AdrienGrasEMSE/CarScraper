/**
 * @file ArgusScraper.cpp
 * 
 * @brief This file implements the ArgusScraper class, which represents a scraper for the Argus website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// Imports
#include "ArgusScraper.hpp"
#include "core/utils/Constant.hpp"
#include "core/logger/Logger.hpp"
#include "io/htmlParser/HtmlParser.hpp"
#include <filesystem>
#include <algorithm>
#include <random>
#include <string>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <regex>
#include <numeric>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor
     */
    ArgusScraper::ArgusScraper() : Entity("ARGUS_SCRAPER") {

        // Default value
        _carBrand = DEFAULT_STR;
        _carModel = DEFAULT_STR;

        // Importing the list of already saved link
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
    ArgusScraper::~ArgusScraper() {

        // Exporting the list of already saved link
        _saver.exportSavedLink();

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the input folder path
     * @param inputFolder The input folder path
     * @note Checks if the directory exists
     */
    void ArgusScraper::setInputFolder(const std::string& inputFolder) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the folder exists
        if (fs::exists(inputFolder)) {
            _inputFolder = inputFolder;
            Logger::trace("[{}].setInputFolder : {}", getFullId(), _inputFolder);
        } else {
            _inputFolder = DEFAULT_STR;
            Logger::debug("[{}].setInputFolder : Folder {} does not exists", getFullId(), inputFolder);
        }

    }


    /**
     * @brief Sets the output folder path
     * @param outputFolder The output folder path
     * @note Checks if the directory exists
     */
    void ArgusScraper::setOutputFolder(const std::string& outputFolder) {
        
        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the folder exists
        if (fs::exists(outputFolder)) {
            _outputFolder = outputFolder;
            Logger::trace("[{}].setOutputFolder : {}", getFullId(), _outputFolder);
        } else {
            _outputFolder = DEFAULT_STR;
            Logger::debug("[{}].setOutputFolder : Folder {} does not exists", getFullId(), outputFolder);
        }

    }


    /**
     * @brief Set the file list
     * @param fileList The file list
     * @todo Check if every file exists
     */
    void ArgusScraper::setFileList(const std::vector<std::string>& fileList) {
        _fileList = fileList;
    }


    /**
     * @brief Set the link list
     * @param linkList The link list
     * @todo Check if every link is valid
     */
    void ArgusScraper::setLinkList(const std::vector<std::string>& linkList) {
        _linkList = linkList;
    }


    /**
     * @brief Set the car brand
     * @param carBrand The car brand
     */
    void ArgusScraper::setCarBrand(const std::string& carBrand) {
        _carBrand = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carBrand));
    }


    /**
     * @brief Set the car model
     * @param carModel The car model
     */
    void ArgusScraper::setCarModel(const std::string& carModel) {
        _carModel = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(carModel));
    }





    // =========================================================================
    // Internal helpers
    // =========================================================================

    /**
     * @brief Shuffler the link list to randomize acces
     */
    template<typename T>
    void ArgusScraper::_shuffle(std::vector<T>& vector) {
        std::random_device  rd;
        std::mt19937        rng = std::mt19937(rd());
        std::ranges::shuffle(vector, rng);
    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Extract a file list from the input folder
     */
    void ArgusScraper::getAllFile() {

        // Checking folder path
        if (_inputFolder == DEFAULT_STR) {
            Logger::error("[{}].getAllLink : File not set", getFullId());
            return;
        }


        // Getting namespace
        namespace fs = std::filesystem;


        // Running through all files
        for (const auto& entry : fs::directory_iterator(_inputFolder))
        {

            // Getting filename
            const std::string filename = entry.path().filename().string();


            // Checking file type and filename
            if (entry.is_regular_file() &&
                filename.size() > 11 &&
                filename.substr(filename.size() - 11) == "_ARGUS.xlsx")
            {
                _fileList.push_back(entry.path().string());
            }

        }

    }


    /**
     * @brief Extract a link list from the file list
     */
    void ArgusScraper::getAllLink() {

        // Checking the file list
        if (_fileList.size() == 0) {
            Logger::error("[{}].getAllLink : no file in the file list", getFullId());
            return;
        }


        // Debug
        Logger::trace("[{}].getAllLink : Starting", getFullId());


        // Going throug all file
        for (const std::string& file : _fileList) {
            if (_xlsxReader.setFilePath(file) && _xlsxReader.excelReadLinkList()) {
                std::vector<std::string> extractedLinkList = _xlsxReader.getLinkList();
                _linkList.insert(_linkList.end(), extractedLinkList.begin(), extractedLinkList.end());
            }
        }


        // Debug
        Logger::trace("[{}].getAllLink : got {} link", getFullId(), _linkList.size());

    }


    /**
     * @brief Scarp all Html using the link list
     */
    void ArgusScraper::scrapAllLink() {

        // Checking the file list
        if (_linkList.size() == 0) {
            Logger::error("[{}].scrapAllLink : no link in the link list", getFullId());
            return;
        }


        // Debug
        Logger::trace("[{}].scrapAllLink : Starting", getFullId());


        // Random link acces
        _shuffle(_linkList);


        // Running through all link list
        for (const std::string& link : _linkList) {

            // Only getting unsaved link
            if (_saver.alreadySaved(link)) {
                Logger::trace("[{}].scrapAllLink : ignoring {}", getFullId(), link);
            } else {

                // Getting response and saving HTML if no error
                CarScraper::HttpResponse response = _client.get(link);
                if (response.statusCode == 200) {
                    _extractedHtml.push_back(response.body);


                    // Setting up the saver
                    // _saver.setName(#); TODO : Scrap the name of the car and the date for the file name
                    _saver.setLink(link);
                    _saver.setContent(response.body);
                    _saver.save();

                } else {
                    Logger::warn("[{}].scrapAllLink : code status {} for {}", getFullId(), response.statusCode, link);
                }

            }

        }


        // Debug
        Logger::trace("[{}].scrapAllLink : got {} raw HTML", getFullId(), _extractedHtml.size());
        
    }


    /**
     * @brief Scrap a car model on the Argus website
     * @param startDate The start date which the scraping start after
     * @param endDate The end date which the scraping start before
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     */
    int ArgusScraper::scrapModel(const std::string& startDateStr, const std::string& endDateStr) {

        // Checking car infos
        if (_carBrand == DEFAULT_STR || _carModel == DEFAULT_STR) {
            Logger::error("[{}].scrapModel : empty brand or model", getFullId());
            return ERROR_CODE;
        }


        // Checking date format
        std::regex pattern("^\\d{4}$");
        if (!std::regex_match(startDateStr, pattern) || !std::regex_match(endDateStr, pattern)) {
            Logger::error("[{}].scrapModel : invalid dates \"{}\" -> \"{}\"", getFullId(), startDateStr, endDateStr);
            return ERROR_CODE;
        }
        std::vector<int> dateInterval(std::stoi(endDateStr) - std::stoi(startDateStr) + 1);
        std::iota(dateInterval.begin(), dateInterval.end(), std::stoi(startDateStr));
        _shuffle(dateInterval);


        // ----- Step 0 - New User-Agent ----------------------------------------------------------
        _client.resetSessionUserAgent();



        // ----- Step 1 - Navigate to largus.fr ---------------------------------------------------
        _client.clearReferer();
        Logger::debug("[{}].scrapModel : starting to scrap {} {}", getFullId(), _carBrand, _carModel);
        std::string main_link = "https://www.largus.fr/";
        CarScraper::HttpResponse response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 2 - Navigate to l'argus.fr/BRAND.html ---------------------------------------
        _client.setReferer(main_link);
        main_link = "https://www.largus.fr/" + _carBrand + ".html";
        response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 3 - Navigate to l'argus.fr/fiche-technique/BRAND.html -----------------------
        _client.setReferer(main_link);
        main_link = "https://www.largus.fr/fiche-technique/" + _carBrand + ".html";
        response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 4 - Navigate to l'argus.fr/fiche-technique/BRAND/MODEL.html -----------------
        _client.setReferer(main_link);
        main_link = "https://www.largus.fr/fiche-technique/" + _carBrand + "/" + _carModel + ".html";
        response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 5 - Going through the year list ---------------------------------------------
        for (int year : dateInterval) {

            // Constructing main link
            _client.setReferer(main_link);
            std::string year_link = "https://www.largus.fr/fiche-technique/" + _carBrand + "/" + _carModel + "/" + std::to_string(year) + ".html";
            response = _client.get(year_link);
            if (response.statusCode == 200) {

                // Debug
                Logger::debug("[{}].scrapModel : get ({}) code {} - accessing link", getFullId(), year_link, response.statusCode);


                // Setting up counters and getting all link
                CarScraper::HtmlParser parser(response.body);
                std::vector<std::string> hrefs = parser.getAllAttributes(
                    "//div[contains(@class,'versions-table-wrapper')]//tbody//td//a",
                    "href"
                );
                int round           = 0;
                int fetched_link    = 0;
                int total_link      = hrefs.size();


                // Running through all link
                for (std::string current_link : hrefs) {

                    // Constructing full link
                    _client.setReferer(year_link);
                    current_link = "https://www.largus.fr" + current_link;
                    response = _client.get( current_link);
                    if (response.statusCode != 200) {
                        Logger::warn("[{}].scrapModel : ({}/{}) get ({}) code {} - ignoring link", getFullId(), round, total_link, current_link, response.statusCode);
                    } else {
                        Logger::trace("[{}].scrapModel : ({}/{}) got {}", getFullId(), round, total_link, current_link);


                        // Getting designation and replacing whitespaces
                        CarScraper::HtmlParser sheetParser(response.body);
                        auto title = sheetParser.getText("//h1[contains(@class,'ft-version-title')]");
                        if (title.has_value()) {
                            std::string name = title.value();
                            std::replace(name.begin(), name.end(), ' ', '_');
                            Logger::trace("[{}].scrapModel : ({}/{}) filename {}", getFullId(), round, total_link, name);
                            _saver.setName(name);
                        } else {
                            Logger::warn("[{}].scrapModel : ({}/{}) got no name", getFullId(), round, total_link);
                            std::string name = _carBrand + "." + _carModel + "_" + std::to_string(round);
                            std::replace(name.begin(), name.end(), ' ', '_');
                            _saver.setName(name);
                        }
                        _saver.setLink(current_link);
                        _saver.setContent(response.body);
                        _saver.save();
                        fetched_link++;

                    }
                    round++;

                }


                // Debug
                int missing_link = total_link - fetched_link;
                if (missing_link == 0) {
                    Logger::trace("[{}].scrapModel : got {} link out of {} from ({})", getFullId(), fetched_link, total_link, year_link);
                } else {
                    Logger::warn("[{}].scrapModel : missing {} link out of {} from ({})", getFullId(), missing_link, total_link, year_link);
                }


            } else {
                Logger::warn("[{}].scrapModel : get ({}) code {} - ignoring link", getFullId(), year_link, response.statusCode);
            }

        }


        // Debug
        Logger::debug("[{}].scrapModel : succes", getFullId(), _extractedHtml.size());
        return SUCCESS_CODE;

    }


}