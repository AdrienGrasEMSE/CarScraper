/**
 * @file MapService.hpp
 * 
 * @brief This file declares a mapping service, which represents a service able to map cars technical sheets.
 *
 * @author Adrien GRAS
 * @date 2026-08-04
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class MapService
     * 
     * @brief A service able to map technical sheets and create a full car object by cross-mapping the data from different sources.
     */
    class MapService : public Entity {

        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor
             */
            MapService();





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Run the mapping process for a specific car model
             * @param brand The target car brand
             * @param model The target car model
             * @return SUCCESS_CODE or ERROR_CODE based on the result of the operation
             */
            int run(const std::string& brand, const std::string& model);

    };

}