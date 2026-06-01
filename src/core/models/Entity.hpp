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
     * @class Entity
     *
     * @brief represent an entity, with an UUID
     */
    class Entity {

    // =========================================================================
    // Private data and methods
    // =========================================================================
    private:

        // Prefix - UUID - Full ID
        std::string     _uuid;      ///< The UUID of the entity generated during construction and immutable after construction.  | Ex : "123e45..."
        std::string     _prefix;    ///< The prefix of the entity, immutable after construction.                                 | Ex: "CAR"
        std::string     _fullId;    ///< Cached full ID (prefix-uuid), immutable after construction.                             | ex: "CAR-123e45..."




    // =========================================================================
    // Protected data and methods
    // =========================================================================
    protected:

        // -------------------------------------------------------------------------
        // Constructor
        // -------------------------------------------------------------------------

        /**
         * @brief Default constructor for the Entity class : Delegating constructor to set the "UNDEFINED" prefix
         */
        Entity();



        /**
         * @brief Constructor for the Entity class.
         *
         * @details 1 - Generates a random UUID for the entity and sets the prefix.
         * @details 2 - Logs the creation of the instance with its full ID.
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
         * @brief Destructor for the Entity class : Logs the destruction of the instance with its full ID.
         */
        virtual ~Entity();





        // -------------------------------------------------------------------------
        // Getters
        // -------------------------------------------------------------------------

        /**
         * @brief Gets the UUID of the entity.
         *
         * @return The UUID of the entity.
         */
        const std::string& getUuid()    const { return _uuid; }



        /**
         * @brief Gets the prefix of the entity.
         *
         * @return The prefix of the entity.
         */
        const std::string& getPrefix()  const { return _prefix; }



        /**
         * @brief Gets the full ID of the entity.
         *
         * @return The full ID of the entity.
         */
        const std::string& getFullId()  const { return _fullId; }

    };

} // namespace CarScraper