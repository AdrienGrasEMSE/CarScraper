// Imports
#include "Entity.hpp"
#include "core/logger/Logger.hpp"
#include <stduuid/uuid.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * Constructor for the Entity class.
     */
    Entity::Entity() : Entity("UNDEFINED") {
        // Delegating constructor to set the undefined prefix
    }


    /**
     * Constructor for the Entity class.
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


        // Debug
        Logger::debug("[{}] New instance created", getFullId());

    }


    /**
     * Destructor for the Entity class.
     */
    Entity::~Entity(){
        Logger::debug("[{}] Instance destroyed", getFullId());
    }

} // namespace CarScraper