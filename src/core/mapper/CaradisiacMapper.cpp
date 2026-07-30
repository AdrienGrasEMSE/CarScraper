/**
 * @file CaradisiacMapper.cpp
 * 
 * @brief This file implements the CaradisiacMapper class, which is a mapper able to generate a Car Entity from a technical sheet from caradisiac.com.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include "CaradisiacMapper.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/logger/Logger.hpp"
#include "core/utils/Constant.hpp"
#include "io/htmlParser/HtmlParser.hpp"
#include <cctype>
#include <string>
#include <regex>
#include <unordered_map>


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
    CaradisiacMapper::CaradisiacMapper() : GenericCarMapper("CARADISIAC_MAPPER") {}





    // =========================================================================
    // Internal Helpers
    // =========================================================================

    /**
     * @brief Converts a string to uppercase
     * @param str The string to convert
     * @return The uppercase string
     */
    std::string CaradisiacMapper::toUpperCase(const std::string& str) {
        std::string upperStr = str;
        std::transform(
            upperStr.begin(), 
            upperStr.end(), 
            upperStr.begin(), 
            ::toupper);
        return upperStr;
    }


    /**
     * @brief Converts a string to lowercase
     * @param str The string to convert
     * @return The lowercase string
     */
    std::string CaradisiacMapper::toLowerCase(const std::string& str) {
        std::string lowerStr = str;
        std::transform(
            lowerStr.begin(), 
            lowerStr.end(), 
            lowerStr.begin(), 
            ::tolower);
        return lowerStr;
    }


    /**
     * @brief Remove a prefix from a string
     * @param str The base string
     * @param prefix The prefix to remove
     * @return The string without the prefix or ERROR_STR if the prefix is not found
     * @note Case insensitive, also removes all whitespace at the beginning of the string
     */
    std::string CaradisiacMapper::removePrefix(const std::string& str, const std::string& prefix) {

        // Detecting the prefix
        if (toLowerCase(str).starts_with(toLowerCase(prefix))) {

            // Deleting the prefix
            std::string result = str.substr(prefix.size());


            // Whitespace removing
            while (!result.empty() && result.front() == ' ') {
                result.erase(0, 1);
            }
            return result;
            

        } else {
            return ERROR_STR;
        }

    }


    /**
     * @brief Remove a suffix from a string
     * @param str The base string
     * @param suffix The suffix to remove
     * @return The string without the suffix or ERROR_STR if the suffix is not found
     * @note Case insensitive, also removes all whitespace at the end of the string
     */
    std::string CaradisiacMapper::removeSuffix(const std::string& str, const std::string& suffix) {

        // Checking the size
        if (str.size() >= suffix.size()) {


            // Getting the position of the trim in the title
            auto pos = toLowerCase(str).rfind(toLowerCase(suffix));
            if (pos != std::string::npos && pos + suffix.size() == str.size()) {

                // Deleting the suffix
                std::string result = str.substr(0, pos);


                // Removing trailing spaces
                while (!result.empty() && result.back() == ' ') {
                    result.pop_back();
                }
                return result;
                
            }

        }


        // Error case
        return ERROR_STR;

    }

    
    /**
     * @brief Extract a roman number from a string
     * @param str The base string
     * @return The roman number or ERROR_STR if nothing found
     * @note Case insensitive (forced uppercase)
     */
    std::string CaradisiacMapper::extractRomanNumbers(const std::string& str) {

        // Extracting the roman number
        std::string upperStr = toUpperCase(str);
        std::regex  pattern(R"(\b[IVXLCDM]+\b)");
        std::smatch match;
        if (std::regex_search(upperStr, match, pattern)) {
            return match.str();
        }
        return ERROR_STR;

    }


    /**
     * @brief Convert a roman number into a arabic one
     * @param romanNumber The base string
     * @return The converted arabic number
     */
    int CaradisiacMapper::romanToInt(const std::string& romanNumber) {

        // Converter definition
        static const std::unordered_map<char, int> valConverter = {
            {'I', 1},
            {'V', 5},
            {'X', 10},
            {'L', 50},
            {'C', 100},
            {'D', 500},
            {'M', 1000}
        };


        // Calculating the value
        int result = 0;
        for (size_t i = 0; i < romanNumber.size(); ++i) {

            // Adding or removing the current value found
            int current = valConverter.at(romanNumber[i]);
            if (i + 1 < romanNumber.size() && current < valConverter.at(romanNumber[i + 1])) {
                result -= current;
            }
            else {
                result += current;
            }
        }
        return result;
        
    }


    /**
     * @brief Extract the generation, phase and engine from the title
     * @param title The title
     */
    void CaradisiacMapper::titleProcessing(const auto& rawTitle) {

        // Verifying if the title has a value
        std::string title = rawTitle.value_or(ERROR_STR);
        if (title != ERROR_STR) {

            // Removing the trim
            std::string titleStd = toLowerCase(title);
            titleStd = removeSuffix(titleStd, this->_car.getTrim());


            // Checking if the method is still succesfull
            if (titleStd != ERROR_STR) {

                // Looking for the start of an engine name (engine displacement)
                std::regex  enginePattern(R"(\d+[.,]\d+)");
                std::smatch match;
                if (std::regex_search(titleStd, match, enginePattern)) {

                    // Extracting the engine
                    std::size_t pos = match.position();
                    this->_car.setEngine(titleStd.substr(pos));


                    // Getting what's left
                    titleStd = titleStd.substr(0, pos);
                    if (!titleStd.empty()) {

                        // Looking for the start of an phase number (between '(' and ')')
                        std::regex phasePattern(R"(\((\d+)\))");
                        if (std::regex_search(titleStd, match, phasePattern)) {

                            // Extracting the phase
                            this->_car.setPhase(match[1].str());


                            // Getting what's left
                            titleStd = titleStd.substr(0, pos);
                            if (!titleStd.empty()) {

                                // Trying the get the generation
                                std::string romanGeneration = extractRomanNumbers(titleStd);
                                if (romanGeneration != ERROR_STR) {
                                    this->_car.setGeneration(std::to_string(romanToInt(romanGeneration)));
                                } else {
                                    Logger::warn("[{}].titleProcessing : no generation extracted from the title \"{}\"",
                                        getFullId(), titleStd);
                                    this->_car.setGeneration("1");
                                }

                            } else {
                                Logger::warn("[{}].titleProcessing : no generation extracted from the title \"{}\"",
                                    getFullId(), titleStd);
                                this->_car.setGeneration("1");
                            }


                        } else {
                            Logger::warn("[{}].titleProcessing : no phase extracted from the title \"{}\"",
                                getFullId(), titleStd);
                            this->_car.setPhase("1");
                        }

                    } else {
                        Logger::warn("[{}].titleProcessing : no generation and phase extracted from the title \"{}\"",
                            getFullId(), titleStd);
                        this->_car.setGeneration("1");
                        this->_car.setPhase("1");
                    }


                } else {
                    Logger::warn("[{}].titleProcessing : no engine extracted from the title \"{}\"",
                    getFullId(), titleStd);
                }


            } else {
                Logger::warn("[{}].titleProcessing : nothing extracted from the title \"{}\"",
                    getFullId(), rawTitle.value_or(ERROR_STR));
            }

        } else {
            Logger::warn("[{}].titleProcessing : no title", getFullId());   
        }

    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Map a car from the Caradisiac website
     * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
     */
    int CaradisiacMapper::mapToCar() {

        // Cheking if the input file path is set
        if (_inputFile == DEFAULT_STR) {
            Logger::error("[{}].mapToCar : input file path is not set", getFullId());
            return ERROR_CODE;  
        }


        // Debug
        Logger::debug("[{}].mapToCar : starting to map {}", getFullId(), _inputFile);


        // Parsing the HTML file
        HtmlParser parser(readFile(_inputFile));


        // ------- General ------------------------------------------------------------------------
        auto brand              = parser.getText("//div[@class='filAriane line hidden-xs hidden-sm']//ol/li[3]/a/span");
        auto model              = parser.getText("//div[@class='filAriane line hidden-xs hidden-sm']//ol/li[4]/a/span");
        auto title              = parser.getText("//h1[@class='h1 line margB_L']/span");
        auto price              = parser.getText("//span[@class='blocPrixVal']");
        auto trim               = parser.getText("//tbody[@class='JScaracCarac1']//tr[td[1]='Finition']/td[2]");
        this->_car.setBrand(brand.value_or(ERROR_STR));
        this->_car.setModel(model.value_or(ERROR_STR));
        this->_car.setPrice(toInt(price));
        this->_car.setTrim(trim.value_or(ERROR_STR));


        // Title processing
        titleProcessing(title);


        // ------- Dimensions ---------------------------------------------------------------------
        auto height             = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Hauteur']/td[2]");
        auto width              = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Largeur sans rétros']/td[2]");
        auto length             = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Longueur']/td[2]");
        auto weight             = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Poids à vide']/td[2]");
        this->_car.setHeight(toDouble(height));
        this->_car.setWidth(toDouble(width));
        this->_car.setLength(toDouble(length));
        this->_car.setWeight(toInt(weight));


        // ------- Liveability --------------------------------------------------------------------
        auto trunk              = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Volume de coffre mini/maxi']/td[2]");
        auto doorCount          = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Nombre de portes']/td[2]");
        auto seatCount          = parser.getText("//tbody[@class='JScaracCarac2']//tr[td[1]='Nombre de places assises']/td[2]");
        this->_car.setTrunkVolume(toInt(trunk));
        this->_car.setDoorCount(toInt(doorCount));
        this->_car.setSeatCount(toInt(seatCount));


        // ------- Transmission -------------------------------------------------------------------
        auto gearBoxLabel       = parser.getText("//tbody[@class='JScaracCarac3']//tr[td[1]='Boîte de vitesse, Nb de rapports']/td[2]");


        // GearBox processing
        std::string gearBoxLabelStr = gearBoxLabel.value_or(ERROR_STR);
        std::regex  pattern(R"((Mécanique|Automatique)\s+à\s+(\d+)\s+rapports)");
        std::smatch match;
        if (gearBoxLabelStr != ERROR_STR && std::regex_search(gearBoxLabelStr, match, pattern)) {

            // Setting up Gearbox type
            if (match[1].str() == "Automatique") {
                this->_car.setGearboxType(CarScraper::GearboxType::AUTOMATIC);
            } else if (match[1].str() == "Mécanique") {
                this->_car.setGearboxType(CarScraper::GearboxType::MANUAL);
            } else {
                this->_car.setGearboxType(CarScraper::GearboxType::NA);
            }


            // Setting up gearcount
            this->_car.setGearCount(std::stoi(match[2].str()));
            

        } else {
            Logger::warn("[{}].mapToCar : no gearBox informations found", getFullId());
        }


        // ------- Power --------------------------------------------------------------------------
        auto fueltype           = parser.getText("//tbody[@class='JScaracCarac3']//tr[td[1]='Motorisation']/td[2]");
        auto power              = parser.getText("//tbody[@class='JScaracCarac3']//tr[td[1]='Puissance']/td[2]");
        auto taxPower           = parser.getText("//tbody[@class='JScaracCarac3']//tr[td[1]='Puissance fiscale']/td[2]");
        this->_car.setFuelType(fueltype.value_or(ERROR_STR));
        this->_car.setHorsePower(toInt(power));
        this->_car.setTaxHorsePower(toInt(taxPower));


        // ------- Consumption --------------------------------------------------------------------
        auto capacity           = parser.getText("//tbody[@class='JScaracCarac5']//tr[td[1]='Volume du réservoir']/td[2]");
        auto fuelConsumption    = parser.getText("//tbody[@class='JScaracCarac5']//tr[td[1]='Consommation mixte']/td[2]");
        auto co2emissions       = parser.getText("//tbody[@class='JScaracCarac5']//tr[td[1]='Emission de CO2 (NEDC)']/td[2]");
        this->_car.setTankCapacity(toInt(capacity));
        this->_car.setFuelConsumption(toDouble(fuelConsumption));
        this->_car.setCo2Emissions(toDouble(co2emissions));
        this->_car.setCo2Class(co2ClassCalculation(this->_car.getCo2Emissions()));


        // ------- Dates --------------------------------------------------------------------------
        auto startDate          = parser.getText("//tbody[@class='JScaracCarac1']//tr[td[1]='Date de commercialisation']/td[2]");
        auto endDate            = parser.getText("//tbody[@class='JScaracCarac1']//tr[td[1]='Date de fin de commercialisation']/td[2]");
        this->_car.setCommercialisationStart(startDate.value_or(ERROR_STR));


        // End date processing
        if (endDate == "--") {
            this->_car.setStillInSale(true);
        } else {
            this->_car.setStillInSale(false);
            this->_car.setCommercialisationEnd(endDate.value_or(ERROR_STR));
        }


        // ------- Technical Data -----------------------------------------------------------------
        this->_car.setDataSource(CarScraper::DataSource::CARADISIAC);


        // Debug
        Logger::debug("[{}].mapToCar : success", getFullId());
        return SUCCESS_CODE;  


    }

}