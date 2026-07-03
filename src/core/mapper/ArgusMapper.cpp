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
    int ArgusMapper::mapToCar() const {

        // Cheking if the input file path is set
        if (_inputFile == DEFAULT_STR) {
            Logger::error("[{}].mapToCar : input file path is not set", getFullId());
            return ERROR_CODE;  
        }


        // Debug
        Logger::debug("[{}].mapToCar : starting to map {}", getFullId(), _inputFile);


        // Parsing the HTML file
        HtmlParser parser(_inputFile);


        // ------- Dimensions ---------------------------------------------------------------------
        auto height = parser.getText("//table[@class='versions-table']//tr[td[1]='Longueur']/td[2]");
        auto width  = parser.getText("//table[@class='versions-table']//tr[td[1]='Largeur']/td[2]");
        auto length = parser.getText("//table[@class='versions-table']//tr[td[1]='Longueur']/td[2]");


        // Getting the title
        auto title = parser.getText("//h1[contains(@class,'ft-version-title')]");
        if (title.has_value()) {
            Logger::warn("{}", getFullId(), title);
        } else {
            Logger::warn("[{}].mapToCar : no title found in {}", getFullId(), _inputFile);
        }


        // Debug
        Logger::debug("[{}].mapToCar : success", getFullId());
        return SUCCESS_CODE;  


    }

}