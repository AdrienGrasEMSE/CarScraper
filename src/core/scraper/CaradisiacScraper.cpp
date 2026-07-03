/**
 * @file CaradisiacScraper.cpp
 * 
 * @brief This file implements the CaradisiacScraper class, which represents a scraper for the Caradisiac website.
 *
 * @author Adrien GRAS
 * @date 2026-06-13
 */


// Imports
#include "CaradisiacScraper.hpp"
#include "core/logger/Logger.hpp"
#include "core/utils/Constant.hpp"
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
    CaradisiacScraper::CaradisiacScraper() : GenericCarScraper("CARADISIAC_SCRAPER") {

        // Setting up brand and model case
        this->setCarBrandCase(CarScraper::TextCase::LOWER);
        this->setCarModelCase(CarScraper::TextCase::LOWER);
    
    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Scrap a car model on the Caradisiac website
     * @param startDate The start date which the scraping start after
     * @param endDate The end date which the scraping start before
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note The accepted date format is "AAAA"
     */
    int CaradisiacScraper::scrapModel(const std::string& startDateStr, const std::string& endDateStr) {

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



        // ----- Step 1 - Navigate to caradisiac.com ----------------------------------------------
        _client.clearReferer();
        Logger::debug("[{}].scrapModel : starting to scrap \"{}-{}\"", getFullId(), _carBrand, _carModel);
        std::string main_link = "https://www.caradisiac.com/";
        CarScraper::HttpResponse response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 2 - Navigate to caradisiac.com/auto--BRAND.html -----------------------------
        _client.setReferer(main_link);
        main_link = "https://www.caradisiac.com/auto--" + _carBrand + "/";
        response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 3 - Navigate to caradisiac.com/fiches-techniques/auto--BRAND/ ---------------
        _client.setReferer(main_link);
        main_link = "https://www.caradisiac.com/fiches-techniques/auto--" + _carBrand + "/";
        response = _client.get(main_link);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
            return ERROR_CODE;
        }



        // ----- Step 4 - Differenciation between only one generation or generation list ----------
        std::string monoGen     = "/modele--" + _carBrand + "-" + _carModel + "/";
        std::string multiGen    = "/gamme--" + _carBrand + "-" + _carModel + "/";
        if (response.body.find(monoGen) != std::string::npos) {

            // Accessing the generation
            _client.setReferer(main_link);
            main_link = "https://www.caradisiac.com/fiches-techniques/modele--" + _carBrand + "-" + _carModel + "/";
            response = _client.get(main_link);
            if (response.statusCode != 200) {
                Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), main_link, response.statusCode);
                return ERROR_CODE;
            }

            // Getting all years of the generation
            yearScraper(monoGen, response, dateInterval);


        } else if (response.body.find(multiGen) != std::string::npos) {

            // Getting all generations of the model
            int result = modelScraper(multiGen, response, dateInterval);
            if (result == ERROR_CODE) {
                Logger::error("[{}].scrapModel : failed to scrap all generations", getFullId());
                return ERROR_CODE;
            } else if (result == IGNORED_ACTION_CODE) {
                Logger::warn("[{}].scrapModel : no generation to scrap", getFullId());
                return IGNORED_ACTION_CODE;
            }

        } else {
            Logger::error("[{}].scrapModel : unhandled brand architecture", getFullId());
            return ERROR_CODE;
        }

        // Debug
        Logger::debug("[{}].scrapModel : succes", getFullId());
        return SUCCESS_CODE;

    }


    /**
     * @brief Scrap a car all generations of a model in the specified interval
     * @param modelLink The link of the model to scrap
     * @param response The previous HTTP response containing the model page
     * @param dateInterval The interval of dates to scrap
     * @note The accepted date format is "AAAA"
     */
    int CaradisiacScraper::modelScraper(std::string& modelLink, CarScraper::HttpResponse response, std::vector<int>& dateInterval) {

        // ----- Step 4 - Navigate to caradisiac.com/fiches-techniques/gamme--BRAND-MODEL/ --------
        _client.setReferer(modelLink);
        modelLink = "https://www.caradisiac.com/fiches-techniques/gamme--" + _carBrand + "-" + _carModel + "/";
        response = _client.get(modelLink);
        if (response.statusCode != 200) {
            Logger::error("[{}].scrapModel : get ({}) code {}", getFullId(), modelLink, response.statusCode);
            return ERROR_CODE;
        }


        // ----- Step 5 - Running through all generation ------------------------------------------
        CarScraper::HtmlParser      genParer(response.body);
        std::vector<std::string>    genList = genParer.getAllAttributes(
            "//div[contains(@class,'txtModeleGalerie')]//a[p[contains(@class,'h3')]]",
            "href"
        );
        _shuffle(genList);
        int gen_round   = 1;
        int gen_fetch   = 0;
        int total_gen   = genList.size();
        for (std::string genlink : genList) {

            // Constructing link
            _client.setReferer(modelLink);
            genlink = "https://www.caradisiac.com" + genlink;


            // Accessing generation
            response = _client.get(genlink);
            if (response.statusCode != 200) {
                Logger::warn("[{}].scrapModel : (Gen {}/{}) get ({}) code {} - ignoring link",
                    getFullId(), gen_round, total_gen, genlink, response.statusCode);
            } else {

                // Debug
                Logger::debug("[{}].scrapModel : (Gen {}/{}) get ({}) code {} - accessing link",
                    getFullId(), gen_round, total_gen, genlink, response.statusCode);
                gen_fetch++;



                // Getting all years of the generation
                yearScraper(genlink, response, dateInterval);

            }
            gen_round++;

        }


        // Debug
        int missing_gen = total_gen - gen_fetch;
        if (missing_gen == 0) {
            Logger::trace("[{}].scrapModel : got {} generation out of {}",
                getFullId(), gen_fetch, total_gen);
        } else {
            Logger::warn("[{}].scrapModel : missing {} generation out of {}",
                getFullId(), missing_gen, total_gen);
        }


        return SUCCESS_CODE;

    }


    /**
     * @brief Scrap a car all years of a generation in the specified interval
     * @param genLink The link of the generation to scrap
     * @param response The previous HTTP response containing the generation page
     * @param dateInterval The interval of dates to scrape
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note The accepted date format is "AAAA"
     */
    void CaradisiacScraper::yearScraper(std::string& genLink, CarScraper::HttpResponse response, std::vector<int>& dateInterval) {
        
        // Getting all years of the generation
        CarScraper::HtmlParser      yearParser(response.body);
        std::vector<std::string>    yearList = yearParser.getAllAttributes(
            "//ul[contains(@class,'listingAnneeComm')]//li//a",
            "href"
        );
        _shuffle(yearList);
        int year_round      = 1;
        int year_fetch      = 0;
        int year_ignored    = 0;
        int total_year      = yearList.size();
        for (std::string yearLink : yearList) {

            // Year extraction
            std::regex re(R"(/(\d{4})/?$)");
            std::smatch match;
            if (!std::regex_search(yearLink, match, re)) {
                Logger::warn("[{}].scrapModel : (Year {}/{}) -> ({}) does not have a year",
                    getFullId(), year_round, total_year, yearLink, response.statusCode);
                continue;
            }


            // Checking if the year is in the interval
            int year = std::stoi(match[1].str());
            if (std::find(dateInterval.begin(), dateInterval.end(), year) != dateInterval.end()) {

                // Accessing year
                _client.setReferer(genLink);
                yearLink = "https://www.caradisiac.com" + yearLink;
                response = _client.get(yearLink);
                if (response.statusCode != 200) {
                    Logger::warn("[{}].scrapModel : (Year {}/{}) get ({}) code {} - ignoring link",
                        getFullId(), year_round, total_year, yearLink, response.statusCode);
                } else {
                    Logger::debug("[{}].scrapModel : (Year {}/{}) get ({}) code {} - accessing link",
                        getFullId(), year_round, total_year, yearLink, response.statusCode);
                    year_fetch++;



                    // ----- Step 7 - Running through technical file ----------------------
                    CarScraper::HtmlParser      linkParser(response.body);
                    std::vector<std::string>    linkList = linkParser.getAllAttributes(
                        "//table[@id='tableListingVersion']//tbody//tr[not(contains(@class,'bgdNone'))]//td//a",
                        "href"
                    );
                    _shuffle(linkList);
                    int link_round      = 1;
                    int fetched_link    = 0;
                    int total_link      = linkList.size();
                    for (std::string link : linkList) {

                        // Accessing link
                        _client.setReferer(yearLink);
                        link = "https://www.caradisiac.com" + link;
                        response = _client.get(link);
                        if (response.statusCode != 200) {
                            Logger::warn("[{}].scrapModel : (Link {}/{}) get ({}) code {} - ignoring link",
                                getFullId(), link_round, total_link, link, response.statusCode);
                        } else {
                            Logger::trace("[{}].scrapModel : (Link {}/{}) got ({})",
                                getFullId(), link_round, total_link, link);


                            // Getting designation and replacing whitespaces
                            CarScraper::HtmlParser sheetParser(response.body);
                            auto title = sheetParser.getText("//h1[contains(@class,'h1 line margB_L')]");
                            if (title.has_value()) {
                                std::string name = "CARADISIAC_" + title.value();
                                std::replace(name.begin(), name.end(), ' ', '_');
                                Logger::trace("[{}].scrapModel : (Link {}/{}) filename {}", getFullId(), link_round, total_link, name);
                                _saver.setName(name);
                            } else {
                                Logger::warn("[{}].scrapModel : (Link {}/{}) got no name", getFullId(), link_round, total_link);
                                std::string name = "CARADISIAC_" + _carBrand + "." + _carModel + "_" + std::to_string(link_round);
                                std::replace(name.begin(), name.end(), ' ', '_');
                                _saver.setName(name);
                            }
                            _saver.setLink(link);
                            _saver.setContent(response.body);
                            _saver.save();
                            fetched_link++;

                        }
                        link_round++;

                    }


                    // Debug
                    int missing_link = total_link - fetched_link;
                    if (missing_link == 0) {
                        Logger::trace("[{}].scrapModel : got {} link out of {}",
                            getFullId(), fetched_link, total_link);
                    } else {
                        Logger::warn("[{}].scrapModel : missing {} link out of {}",
                            getFullId(), missing_link, total_link);
                    }

                }
                
            } else {
                Logger::trace("[{}].scrapModel : (Year {}/{}) -> ({}) is not in the interval",
                    getFullId(), year_round, total_year, year);
                year_ignored++;
            }
            year_round++;

        }


        // Debug
        if (year_ignored == total_year) {
            Logger::debug("[{}].scrapModel : no year in the interval", getFullId());
        } else {
            total_year = total_year - year_ignored;
            int missing_year = total_year - year_fetch;
            if (missing_year == 0) {
                Logger::trace("[{}].scrapModel : got {} year out of {}",
                    getFullId(), year_fetch, total_year);
            } else {
                Logger::warn("[{}].scrapModel : missing {} year out of {}",
                    getFullId(), missing_year, total_year);
            }
        }

    }


}