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
    CaradisiacMapper::CaradisiacMapper() : GenericCarMapper("CARADISIAC_MAPPER") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Map a car from the Caradisiac website
     * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
     */
    int CaradisiacMapper::mapToCar() const {

        // Cheking if the input file path is set
        if (_inputFile == DEFAULT_STR) {
            Logger::error("[{}].mapToCar : input file path is not set", getFullId());
            return ERROR_CODE;  
        }


        // Debug
        Logger::debug("[{}].mapToCar : starting to map {}", getFullId(), _inputFile);


        // Parsing the HTML file
        HtmlParser parser(_inputFile);


        // Debug
        Logger::debug("[{}].mapToCar : success", getFullId());
        return SUCCESS_CODE;  


    }

}