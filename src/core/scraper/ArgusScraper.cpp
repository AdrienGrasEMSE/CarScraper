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
#include "core/logger/Logger.hpp"
#include "io/htmlParser/HtmlParser.hpp"
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
    ArgusScraper::ArgusScraper() : GenericCarScraper("ARGUS_SCRAPER") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Scrap a car model on the Argus website
     * @param startDate The start date which the scraping start after
     * @param endDate The end date which the scraping start before
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note The accepted date format is "AAAA"
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
        Logger::debug("[{}].scrapModel : starting to scrap \"{}-{}\"", getFullId(), _carBrand, _carModel);
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
        int current_year    = 0;
        int total_year      = dateInterval.size();
        for (int year : dateInterval) {

            // Constructing main link
            _client.setReferer(main_link);
            std::string year_link = "https://www.largus.fr/fiche-technique/"
                + _carBrand + "/" + _carModel + "/" + std::to_string(year) + ".html";
            response = _client.get(year_link);
            if (response.statusCode == 200) {

                // Debug
                Logger::debug("[{}].scrapModel : (Year {}/{}) get ({}) code {} - accessing link",
                    getFullId(), current_year, total_year, year_link, response.statusCode);


                // Setting up counters and getting all link
                CarScraper::HtmlParser parser(response.body);
                std::vector<std::string> hrefs = parser.getAllAttributes(
                    "//div[contains(@class,'versions-table-wrapper')]//tbody//td//a",
                    "href"
                );
                _shuffle(hrefs);
                int round           = 1;
                int fetched_link    = 0;
                int total_link      = hrefs.size();


                // Running through all link
                for (std::string current_link : hrefs) {

                    // Constructing full link
                    _client.setReferer(year_link);
                    current_link = "https://www.largus.fr" + current_link;


                    // Skip already saved links
                    if (_saver.alreadySaved(current_link)) {
                        Logger::trace("[{}].scrapModel : (Link {}/{}) ignoring already saved {}",
                            getFullId(), round, total_link, current_link);
                        round++;
                    } else {

                        // Accessing link
                        response = _client.get(current_link);
                        if (response.statusCode != 200) {
                            Logger::warn("[{}].scrapModel : (Link {}/{}) get ({}) code {} - ignoring link",
                                getFullId(), round, total_link, current_link, response.statusCode);
                        } else {
                            Logger::trace("[{}].scrapModel : (Link {}/{}) got {}", getFullId(), round, total_link, current_link);


                            // Getting designation and replacing whitespaces
                            CarScraper::HtmlParser sheetParser(response.body);
                            auto title = sheetParser.getText("//h1[contains(@class,'ft-version-title')]");
                            if (title.has_value()) {
                                std::string name = title.value();
                                std::replace(name.begin(), name.end(), ' ', '_');
                                Logger::trace("[{}].scrapModel : (Link {}/{}) filename {}", getFullId(), round, total_link, name);
                                _saver.setName(name);
                            } else {
                                Logger::warn("[{}].scrapModel : (Link {}/{}) got no name", getFullId(), round, total_link);
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

                }


                // Debug
                int missing_link = total_link - fetched_link;
                if (missing_link == 0) {
                    Logger::trace("[{}].scrapModel : got {} link out of {} from ({})",
                        getFullId(), fetched_link, total_link, year_link);
                } else {
                    Logger::warn("[{}].scrapModel : missing {} link out of {} from ({})",
                        getFullId(), missing_link, total_link, year_link);
                }


            } else {
                Logger::warn("[{}].scrapModel : (Year {}/{}) get ({}) code {} - ignoring link",
                    getFullId(), current_year, total_year, year_link, response.statusCode);
            }
            current_year++;

        }


        // Debug
        Logger::debug("[{}].scrapModel : succes", getFullId());
        return SUCCESS_CODE;

    }


}