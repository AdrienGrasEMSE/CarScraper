// For only one include
#pragma once


// Imports
#include <string>
#include "stduuid/uuid.h"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * Class Entity:
     *
     * Represents an entity with the following characteristics:
     * - UUID: unique identifier for the entity (generated automatically)
     *      -> prefix   | string    | ex: "CAR"
     *      -> uuid     | string    | ex: "123e45..."
     * 
     * Methods:
     * - getUuid()       : returns the UUID of the entity       | ex: "123e45..."
     * - getPrefix()     : returns the prefix of the entity     | ex: "CAR"
     * - getFullId()     : returns the full ID of the entity    | ex: "CAR-123e45..."
     *
     */
    class Entity {

    // =========================================================================
    // Private data and methods
    // =========================================================================
    private:

        // Prefix - UUID
        std::string     _uuid;
        std::string     _prefix;




    // =========================================================================
    // Public data and methods
    // =========================================================================
    public:

        // -------------------------------------------------------------------------
        // Constructor / Destructor
        // -------------------------------------------------------------------------
        Entity();
        Entity(const std::string& prefix);
        virtual ~Entity();





        // -------------------------------------------------------------------------
        // Getters
        // -------------------------------------------------------------------------
        const std::string& getUuid()       const { return _uuid; }
        const std::string& getPrefix()     const { return _prefix; }
        std::string        getFullId()     const { return _prefix + "-" + _uuid; }

    };

} // namespace CarScraper