/**
 * @file CaradisiacMapper.hpp
 * 
 * @brief This file declares the CaradisiacMapper class, which is a mapper able to generate a Car Entity from a technical sheet from caradisiac.com.
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
     * @class CaradisiacMapper
     * 
     * @brief Class representing a Caradisiac mapper
     */
    class CaradisiacMapper : public GenericCarMapper {

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
            CaradisiacMapper();




            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Map a car from the Caradisiac website
             * @return ERROR_CODE / SUCCESS_CODE whether the method is successful or not
             */
            using GenericCarMapper::mapToCar;
            int mapToCar() const;
    };

} // namespace CarScraper