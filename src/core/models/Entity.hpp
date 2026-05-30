/**
 * @file Entity.hpp
 * 
 * @brief This file define the Entity class, a basic class with a UUID system.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 */


// For only one include
#pragma once


// Imports
#include <string>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class Entity:
     *
     * @attributes:
     * - _uuid          : (string) The UUID of the entity,
                            generated during construction and
                            immutable after construction.       | Ex : "123e45..."
     * - _prefix        : (string) The prefix of the entity,    
     *                      immutable after construction.       | Ex: "CAR"
     * - _fullId        : (string) Cached full ID (prefix-uuid),
                            immutable after construction.       | Ex: "CAR-123e45..."
     * 
     * @getters:
     * - getUuid()      : returns the UUID of the entity        | ex: "123e45..."
     * - getPrefix()    : returns the prefix of the entity      | ex: "CAR"
     * - getFullId()    : returns the full ID of the entity     | ex: "CAR-123e45..."
     *
     */
    class Entity {

    // =========================================================================
    // Private data and methods
    // =========================================================================
    private:

        // Prefix - UUID - Full ID
        std::string     _uuid;
        std::string     _prefix;
        std::string     _fullId;




    // =========================================================================
    // Protected data and methods
    // =========================================================================
    protected:

        // -------------------------------------------------------------------------
        // Constructor
        // -------------------------------------------------------------------------

        /**
         * Constructor for the Entity class.
         * - Delegating constructor to set the undefined prefix
         */
        Entity();



        /**
         * Constructor for the Entity class.
         * - Generates a random UUID for the entity and sets the prefix.
         * - Logs the creation of the instance with its full ID.
         *
         * @param prefix The prefix for the entity.
         */
        Entity(const std::string& prefix);





    // =========================================================================
    // Public data and methods
    // =========================================================================
    public:

        // -------------------------------------------------------------------------
        // Destructor
        // -------------------------------------------------------------------------

        /**
         * Destructor for the Entity class.
         *
         * Logs the destruction of the instance with its full ID.
         */
        virtual ~Entity();





        // -------------------------------------------------------------------------
        // Getters
        // -------------------------------------------------------------------------

        /**
         * Returns the UUID of the entity.
         *
         * @return The UUID of the entity.
         */
        const std::string& getUuid()    const { return _uuid; }



        /**
         * Returns the prefix of the entity.
         *
         * @return The prefix of the entity.
         */
        const std::string& getPrefix()  const { return _prefix; }



        /**
         * Returns the full ID of the entity.
         *
         * @return The full ID of the entity.
         */
        const std::string& getFullId()  const { return _fullId; }

    };

} // namespace CarScraper