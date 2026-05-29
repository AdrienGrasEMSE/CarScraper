// Imports
#include "Entity.hpp"
#include "core/logger/Logger.hpp"
#include <stduuid/uuid.h>
#include <random>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * Constructor for the Entity class.
     * - Delegating constructor to set the undefined prefix
     */
    Entity::Entity() : Entity("UNDEFINED") {}


    /**
     * Constructor for the Entity class.
     * - Generates a random UUID for the entity and sets the prefix.
     * - Logs the creation of the instance with its full ID.
     *
     * @param prefix The prefix for the entity.
     */
    Entity::Entity(const std::string& prefix) {

        // UUID generation (random v4)
        std::random_device  rd;
        std::mt19937        engine{rd()};
        uuids::uuid_random_generator gen{engine};
        _uuid               = uuids::to_string(gen());
        _prefix             = prefix;
        _fullId             = _prefix + "-" + _uuid;


        // Debug
        Logger::debug("[{}] New instance created", getFullId());

    }


    /**
     * Destructor for the Entity class.
     *
     * Logs the destruction of the instance with its full ID.
     */
    Entity::~Entity(){
        Logger::debug("[{}] Instance destroyed", getFullId());
    }

} // namespace CarScraper