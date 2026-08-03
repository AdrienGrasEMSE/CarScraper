/**
 * @file ArgusMapper.hpp
 * 
 * @brief This file declares the ArgusMapper class, which is a mapper able to generate a Car Entity from a technical sheet from largus.fr.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// For only one include
#pragma once


// Imports
#include "GenericCarMapper.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class ArgusMapper
     * 
     * @brief Class representing an Argus mapper
     */
    class ArgusMapper : public GenericCarMapper {

        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default Constructor.
             */
            ArgusMapper();




            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Map a car from the Argus website
             * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
             */
            using GenericCarMapper::mapToCar;
            int mapToCar();
    };

} // namespace CarScraper