// Imports
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <regex>
#include "core/models/Entity.hpp"
#include "core/Logger/Logger.hpp"


// =============================================================================
// Helpers
// =============================================================================


// Regex UUID v4 : xxxxxxxx-xxxx-4xxx-[89ab]xxx-xxxxxxxxxxxx
static const std::regex UUID_V4_REGEX(
    "^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"
);


// Minimal mock class to test Entity via inheritance (constructor is protected)
class MockEntity : public CarScraper::Entity {
public:
    MockEntity() : Entity("MOCK") {}
    MockEntity(const std::string& prefix) : Entity(prefix) {}
};


// Mock class to test the default constructor (UNDEFINED prefix)
class MockEntityDefault : public CarScraper::Entity {
public:
    MockEntityDefault() : Entity() {}
};




// =============================================================================
// Tests — UUID
// =============================================================================

TEST_CASE("Entity UUID", "[entity][uuid]") {

    SECTION("UUID is not empty after construction") {
        MockEntity e("TEST");
        REQUIRE_FALSE(e.getUuid().empty());
    }

    SECTION("UUID matches v4 format") {
        MockEntity e("TEST");
        REQUIRE(std::regex_match(e.getUuid(), UUID_V4_REGEX));
    }

    SECTION("Two instances have different UUIDs") {
        MockEntity e1("TEST");
        MockEntity e2("TEST");
        REQUIRE(e1.getUuid() != e2.getUuid());
    }
}




// =============================================================================
// Tests — Prefix
// =============================================================================

TEST_CASE("Entity Prefix", "[entity][prefix]") {

    SECTION("Prefix is correctly set when provided") {
        MockEntity e("CAR");
        REQUIRE(e.getPrefix() == "CAR");
    }

    SECTION("Default prefix is UNDEFINED") {
        MockEntityDefault e;
        REQUIRE(e.getPrefix() == "UNDEFINED");
    }

    SECTION("Prefix is immutable — getPrefix() always returns construction value") {
        MockEntity e("CAR");
        const std::string& p1 = e.getPrefix();
        const std::string& p2 = e.getPrefix();
        REQUIRE(p1 == p2);
        REQUIRE(p1 == "CAR");
    }
}




// =============================================================================
// Tests — FullId
// =============================================================================

TEST_CASE("Entity FullId", "[entity][fullid]") {

    SECTION("getFullId() starts with the prefix") {
        MockEntity e("CAR");
        REQUIRE(e.getFullId().rfind("CAR-", 0) == 0);
    }

    SECTION("getFullId() contains the UUID") {
        MockEntity e("CAR");
        REQUIRE(e.getFullId().find(e.getUuid()) != std::string::npos);
    }

    SECTION("getFullId() format is PREFIX-UUID") {
        MockEntity e("CAR");
        REQUIRE(e.getFullId() == "CAR-" + e.getUuid());
    }

    SECTION("getFullId() is stable across multiple calls (cached value)") {
        MockEntity e("CAR");
        REQUIRE(e.getFullId() == e.getFullId());
    }

    SECTION("Default entity getFullId() starts with UNDEFINED-") {
        MockEntityDefault e;
        REQUIRE(e.getFullId().rfind("UNDEFINED-", 0) == 0);
    }
}




// =============================================================================
// Tests — Inheritance
// =============================================================================

TEST_CASE("Entity Inheritance", "[entity][inheritance]") {

    SECTION("Derived class receives correct prefix") {
        MockEntity m;
        REQUIRE(m.getPrefix() == "MOCK");
    }

    SECTION("Derived class has a valid UUID") {
        MockEntity m;
        REQUIRE(std::regex_match(m.getUuid(), UUID_V4_REGEX));
    }

    SECTION("Derived class getFullId() starts with MOCK-") {
        MockEntity m;
        REQUIRE(m.getFullId().rfind("MOCK-", 0) == 0);
    }

    SECTION("Derived class getFullId() is consistent with prefix and UUID") {
        MockEntity m;
        REQUIRE(m.getFullId() == m.getPrefix() + "-" + m.getUuid());
    }
}




// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // Logger Init
    CarScraper::Logger::init();

    // Catch2 runner
    return Catch::Session().run(argc, argv);

}