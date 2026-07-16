/**
 * @file ArgusMapper.cpp
 * 
 * @brief This file implements the ArgusMapper class, which is a mapper able to generate a Car Entity from a technical sheet from largus.fr.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include "ArgusMapper.hpp"
#include "core/logger/Logger.hpp"
#include "core/utils/Constant.hpp"
#include "io/htmlParser/HtmlParser.hpp"
#include <regex>


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
    ArgusMapper::ArgusMapper() : GenericCarMapper("ARGUS_MAPPER") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Map a car from the Argus website
     * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
     * @note The car brand and model will be not extracted from the technical sheet
     *       some cases unable to be handled. ex: "ALFA ROMEO 159 SW" extracted from
     *       the title cannot be simply split into the brand ALFA ROMEO and the model
     *       159 SW.
     */
    int ArgusMapper::mapToCar() {

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
        auto brand      = parser.getText("//div[@class='breadCrumbList']//li[3]/a");
        auto model      = parser.getText("//div[@class='breadCrumbList']//li[4]/a");
        auto engine     = parser.getText("//table[@class='versions-table']//tr[td[1]='Nom du moteur']/td[2]");
        auto title      = parser.getText("//h1[@class='ft-version-title']");
        auto prix       = parser.getText("//div[@class='ft-version-price']/strong");
        this->_car.setBrand(brand.value_or(ERROR_STR));
        this->_car.setModel(model.value_or(ERROR_STR));
        this->_car.setEngine(engine.value_or(ERROR_STR));
        this->_car.setPrice(toInt(prix));


        // Trim processing
        static const std::regex pattern1(R"(.*?\b\d+\s*(?:,\d+)?\s*(?:ch|cv)\b\s*(.*)$)",std::regex::icase);
        std::string trimStr = title.value_or(ERROR_STR);
        std::smatch match;
        if (std::regex_match(trimStr, match, pattern1)) {
            this->_car.setTrim(match[1].str());
        }


        // ------- Dimensions ---------------------------------------------------------------------
        auto height     = parser.getText("//table[@class='versions-table']//tr[td[1]='Hauteur']/td[2]");
        auto width      = parser.getText("//table[@class='versions-table']//tr[td[1]='Largeur']/td[2]");
        auto length     = parser.getText("//table[@class='versions-table']//tr[td[1]='Longueur']/td[2]");
        auto trunk      = parser.getText("//table[@class='versions-table']//tr[td[1]='Volume de coffre']/td[2]");
        auto weight     = parser.getText("//table[@class='versions-table']//tr[td[1]='Poids à vide']/td[2]");
        auto seatCount  = parser.getText("//table[@class='versions-table']//tr[td[1]='Nombre de places']/td[2]");
        this->_car.setHeight(toDouble(height));
        this->_car.setWidth(toDouble(width));
        this->_car.setLength(toDouble(length));
        this->_car.setTrunkVolume(toInt(trunk));
        this->_car.setWeight(toInt(weight));
        this->_car.setSeatCount(toInt(seatCount));


        // ------- Transmission -------------------------------------------------------------------
        auto gearBoxType        = parser.getText("//table[@class='versions-table']//tr[td[1]='Boîte de vitesses']/td[2]");
        this->_car.setGearboxType(gearBoxType.value_or(ERROR_STR));


        // ------- Power --------------------------------------------------------------------------
        auto fueltype           = parser.getText("//table[@class='versions-table']//tr[td[1]='Énergie']/td[2]");
        auto power              = parser.getText("//table[@class='versions-table']//tr[td[1]='Puissance réelle maxi']/td[2]");
        auto taxPower           = parser.getText("//table[@class='versions-table']//tr[td[1]='Puissance fiscale']/td[2]");
        this->_car.setFuelType(fueltype.value_or(ERROR_STR));
        this->_car.setHorsePower(toInt(power));
        this->_car.setTaxHorsePower(toInt(taxPower));


        // ------- Consumption --------------------------------------------------------------------
        auto capacity           = parser.getText("//table[@class='versions-table']//tr[td[1]='Réservoir']/td[2]");
        auto fuelConsumption    = parser.getText("//table[@class='versions-table']//tr[td[1]='Mixte']/td[2]");
        auto co2emissions       = parser.getText("//table[@class='versions-table']//tr[td[1]='Émission de CO2']/td[2]");
        auto co2Class           = parser.getText("//table[@class='versions-table']//tr[td[1]='Réservoir']/td[2]");
        this->_car.setTankCapacity(toInt(capacity));
        this->_car.setFuelConsumption(toDouble(fuelConsumption));
        this->_car.setCo2Emissions(toDouble(co2emissions));
        this->_car.setCo2Class(co2ClassCalculation(this->_car.getCo2Emissions()));


        // ------- Dates --------------------------------------------------------------------------
        auto label              = parser.getText("//div[@class='ft-version-price']/text()");
        auto endDate            = parser.getText("//table[@class='versions-table']//tr[td[1]='Date de fin de commercialisation']/td[2]");
        
        
        // Start date processing
        static const std::regex pattern2(R"((\d{2}/\d{2}/\d{4}))");
        std::string dateStr = label.value_or(ERROR_STR);
        if (std::regex_search(dateStr, match, pattern2)) {
            this->_car.setCommercialisationStart(match[1].str());
        }


        // End date processing
        Logger::info("{}", endDate.value_or(ERROR_STR));
        if (endDate == "En cours") {
            this->_car.setStillInSale(true);
        } else {
            this->_car.setStillInSale(false);
            this->_car.setCommercialisationEnd(endDate.value_or(ERROR_STR));
        }


        // ------- Technical Data -----------------------------------------------------------------
        this->_car.setDataSource(CarScraper::DataSource::ARGUS);


        // Debug
        Logger::debug("[{}].mapToCar : success", getFullId());
        return SUCCESS_CODE;  

    }

}