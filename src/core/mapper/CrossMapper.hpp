/**
 * @file CrossMapper.hpp
 * 
 * @brief This file declares the CrossMapper class, which is a mapper able to complete a Car Entity using
 * two Car generated from largus and caradisiac technical file.
 *
 * @author Adrien GRAS
 * @date 2026-07-28
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include "core/models/Car.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class CrossMapper
     * 
     * @brief Class representing a Cross Mapper
     */
    class CrossMapper : public Entity {

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
            CrossMapper();




            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Join two Car entity to create a more complete one
             * @param largus The Car entity extracted from a largus technical file
             * @param caradisac The Car entity extracted from a cardisiac technical file
             * @return The completed Car entity
             */
            Car crossMap(Car largus, Car caradisiac);
            
    };

} // namespace CarScraper