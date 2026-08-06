/**
 * @file baseCar.cpp
 *
 * @brief Unit tests for CarScraper::Car class.
 *        Covers: construction, getters, setters (valid/invalid/boundary),
 *                string formatting, enum conversions, dates, isComplete, isValid
 *                (every field branch), toString and operator<<.
 *
 * @author Adrien GRAS
 * @date 2026-05-30
 */


// Imports
#include <catch2/catch.hpp>
#include "core/enum/DataSource.hpp"
#include "core/enum/FuelType.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/Co2Class.hpp"
#include "core/utils/Constant.hpp"
#include "core/utils/Validation.hpp"
#include "core/models/Car.hpp"
#include <sstream>

using namespace CarScraper;


// =============================================================================
// Helpers
// =============================================================================

/**
 * Builds a fully populated, valid Car for reuse across test sections.
 */
static Car buildValidCar() {
    Car c;
    c.setBrand                  ("Toyota");
    c.setModel                  ("Corolla");
    c.setGeneration             ("E210");
    c.setPhase                  ("2");
    c.setEngine                 ("1.8L Hybrid");
    c.setTrim                   ("Dynamic");
    c.setPrice                  (25000);
    c.setHeight                 (1.43);
    c.setLength                 (4.37);
    c.setWidth                  (1.79);
    c.setWeight                 (1375);
    c.setTrunkVolume            (361);
    c.setDoorCount              (5);
    c.setSeatCount              (5);
    c.setGearboxType            (GearboxType::AUTOMATIC);
    c.setGearCount              (6);
    c.setFuelType               (FuelType::EE);
    c.setHorsePower             (122);
    c.setTaxHorsePower          (6);
    c.setTankCapacity           (43);
    c.setFuelConsumption        (4.3);
    c.setCo2Emissions           (98);
    c.setCo2Class               (Co2Class::A);
    c.setCommercialisationStart ("01/01/2019");
    c.setStillInSale            (true);
    c.setDataSource             (DataSource::ARGUS);
    c.setFromDuplicatedSource   (true);
    return c;
}


/**
 * Builds a fully populated, valid Car, except for one numeric field which is
 * left untouched (i.e. still at its constructor DEFAULT_INT/DEFAULT_DOUBLE sentinel).
 * @details Needed because DEFAULT_INT (9999) is out of range for most numeric
 * setters (e.g. height max 10, weight max 5000), so calling the setter with
 * DEFAULT_INT/DEFAULT_DOUBLE would store ERROR_INT/ERROR_DOUBLE instead — the
 * only way to reach the true "not yet set" state is to skip the setter entirely.
 * @param fieldToSkip The name of the numeric field to leave at its default value.
 */
static Car buildValidCarSkipping(const std::string& fieldToSkip) {
    Car c;
    c.setBrand                  ("Toyota");
    c.setModel                  ("Corolla");
    c.setGeneration             ("E210");
    c.setPhase                  ("2");
    c.setEngine                 ("1.8L Hybrid");
    c.setTrim                   ("Dynamic");
    c.setPrice                  (25000);
    if (fieldToSkip != "height")           c.setHeight            (1.43);
    if (fieldToSkip != "length")           c.setLength             (4.37);
    if (fieldToSkip != "width")            c.setWidth              (1.79);
    if (fieldToSkip != "weight")           c.setWeight             (1375);
    if (fieldToSkip != "trunkVolume")      c.setTrunkVolume        (361);
    if (fieldToSkip != "doorCount")        c.setDoorCount          (5);
    if (fieldToSkip != "seatCount")        c.setSeatCount          (5);
    c.setGearboxType             (GearboxType::AUTOMATIC);
    if (fieldToSkip != "gearCount")        c.setGearCount          (6);
    c.setFuelType                (FuelType::EE);
    if (fieldToSkip != "horsePower")       c.setHorsePower         (122);
    if (fieldToSkip != "taxHorsePower")    c.setTaxHorsePower      (6);
    if (fieldToSkip != "tankCapacity")     c.setTankCapacity       (43);
    if (fieldToSkip != "fuelConsumption")  c.setFuelConsumption    (4.3);
    if (fieldToSkip != "co2Emissions")     c.setCo2Emissions       (98);
    c.setCo2Class                (Co2Class::A);
    c.setCommercialisationStart  ("01/01/2019");
    c.setStillInSale             (true);
    c.setDataSource               (DataSource::ARGUS);
    return c;
}


// =============================================================================
// Tests — Default construction
// =============================================================================

TEST_CASE("Car Default Construction", "[car][construction]") {

    SECTION("UUID is generated and non-empty") {
        Car c;
        REQUIRE_FALSE(c.getUuid().empty());
    }

    SECTION("Prefix is CAR") {
        Car c;
        REQUIRE(c.getPrefix() == "CAR");
    }

    SECTION("FullId starts with CAR-") {
        Car c;
        REQUIRE(c.getFullId().rfind("CAR-", 0) == 0);
    }

    SECTION("Two cars have different UUIDs") {
        Car c1, c2;
        REQUIRE(c1.getUuid() != c2.getUuid());
    }

    SECTION("All string attributes are DEFAULT_STR") {
        Car c;
        REQUIRE(c.getBrand()      == DEFAULT_STR);
        REQUIRE(c.getModel()      == DEFAULT_STR);
        REQUIRE(c.getGeneration() == DEFAULT_STR);
        REQUIRE(c.getPhase()      == DEFAULT_STR);
        REQUIRE(c.getEngine()     == DEFAULT_STR);
        REQUIRE(c.getTrim()       == DEFAULT_STR);
    }

    SECTION("All int attributes are DEFAULT_INT") {
        Car c;
        REQUIRE(c.getPrice()         == DEFAULT_INT);
        REQUIRE(c.getWeight()        == DEFAULT_INT);
        REQUIRE(c.getTrunkVolume()   == DEFAULT_INT);
        REQUIRE(c.getDoorCount()     == DEFAULT_INT);
        REQUIRE(c.getSeatCount()     == DEFAULT_INT);
        REQUIRE(c.getGearCount()     == DEFAULT_INT);
        REQUIRE(c.getHorsePower()    == DEFAULT_INT);
        REQUIRE(c.getTaxHorsePower() == DEFAULT_INT);
        REQUIRE(c.getTankCapacity()  == DEFAULT_INT);
        REQUIRE(c.getCo2Emissions()  == DEFAULT_INT);
    }

    SECTION("All double attributes are DEFAULT_DOUBLE") {
        Car c;
        REQUIRE(c.getHeight()          == DEFAULT_DOUBLE);
        REQUIRE(c.getLength()          == DEFAULT_DOUBLE);
        REQUIRE(c.getWidth()           == DEFAULT_DOUBLE);
        REQUIRE(c.getFuelConsumption() == DEFAULT_DOUBLE);
    }

    SECTION("All enum attributes are NA") {
        Car c;
        REQUIRE(c.getCo2Class()    == Co2Class::NA);
        REQUIRE(c.getGearboxType() == GearboxType::NA);
        REQUIRE(c.getFuelType()    == FuelType::NA);
        REQUIRE(c.getDataSource()  == DataSource::NA);
    }

    SECTION("Commercialisation dates are nullopt") {
        Car c;
        REQUIRE_FALSE(c.getCommercialisationStart().has_value());
        REQUIRE_FALSE(c.getCommercialisationEnd().has_value());
    }

    SECTION("StillInSale is false") {
        Car c;
        REQUIRE(c.isStillInSale() == false);
    }

}


// =============================================================================
// Tests — Setters : valid values
// =============================================================================

TEST_CASE("Car Setters — Valid Values", "[car][setters][valid]") {

    SECTION("setBrand stores the value") {
        Car c;
        c.setBrand("Renault");
        REQUIRE(c.getBrand() == "RENAULT");
    }

    SECTION("setModel stores capitalized value") {
        Car c;
        c.setModel("clio");
        REQUIRE(c.getModel() == "Clio");
    }

    SECTION("setGeneration stores the value") {
        Car c;
        c.setGeneration("IV");
        REQUIRE(c.getGeneration() == "IV");
    }

    SECTION("setPhase stores the value") {
        Car c;
        c.setPhase("2");
        REQUIRE(c.getPhase() == "2");
    }

    SECTION("setEngine stores the value") {
        Car c;
        c.setEngine("1.5 dCi 90hp");
        REQUIRE(c.getEngine() == "1.5 dCi 90hp");
    }

    SECTION("setTrim stores capitalized value") {
        Car c;
        c.setTrim("expression");
        REQUIRE(c.getTrim() == "Expression");
    }

    SECTION("setPrice stores the value") {
        Car c;
        c.setPrice(15000);
        REQUIRE(c.getPrice() == 15000);
    }

    SECTION("setHeight stores the value") {
        Car c;
        c.setHeight(1.45);
        REQUIRE(c.getHeight() == Approx(1.45));
    }

    SECTION("setLength stores the value") {
        Car c;
        c.setLength(4.05);
        REQUIRE(c.getLength() == Approx(4.05));
    }

    SECTION("setWidth stores the value") {
        Car c;
        c.setWidth(1.75);
        REQUIRE(c.getWidth() == Approx(1.75));
    }

    SECTION("setWeight stores the value") {
        Car c;
        c.setWeight(1200);
        REQUIRE(c.getWeight() == 1200);
    }

    SECTION("setTrunkVolume stores the value") {
        Car c;
        c.setTrunkVolume(300);
        REQUIRE(c.getTrunkVolume() == 300);
    }

    SECTION("setDoorCount stores the value") {
        Car c;
        c.setDoorCount(5);
        REQUIRE(c.getDoorCount() == 5);
    }

    SECTION("setSeatCount stores the value") {
        Car c;
        c.setSeatCount(5);
        REQUIRE(c.getSeatCount() == 5);
    }

    SECTION("setGearboxType (string) stores the enum value") {
        Car c;
        c.setGearboxType("Automatique");
        REQUIRE(c.getGearboxType() == GearboxType::AUTOMATIC);
    }

    SECTION("setGearboxType (enum) stores the value directly") {
        Car c;
        c.setGearboxType(GearboxType::MANUAL);
        REQUIRE(c.getGearboxType() == GearboxType::MANUAL);
    }

    SECTION("setGearCount stores the value") {
        Car c;
        c.setGearCount(6);
        REQUIRE(c.getGearCount() == 6);
    }

    SECTION("setFuelType (string) stores the enum value") {
        Car c;
        c.setFuelType("Essence");
        REQUIRE(c.getFuelType() == FuelType::ES);
    }

    SECTION("setFuelType (enum) stores the value directly") {
        Car c;
        c.setFuelType(FuelType::EL);
        REQUIRE(c.getFuelType() == FuelType::EL);
    }

    SECTION("setHorsePower stores the value") {
        Car c;
        c.setHorsePower(90);
        REQUIRE(c.getHorsePower() == 90);
    }

    SECTION("setTaxHorsePower stores the value") {
        Car c;
        c.setTaxHorsePower(5);
        REQUIRE(c.getTaxHorsePower() == 5);
    }

    SECTION("setTankCapacity stores the value") {
        Car c;
        c.setTankCapacity(50);
        REQUIRE(c.getTankCapacity() == 50);
    }

    SECTION("setFuelConsumption stores the value") {
        Car c;
        c.setFuelConsumption(4.5);
        REQUIRE(c.getFuelConsumption() == Approx(4.5));
    }

    SECTION("setCo2Emissions stores the value") {
        Car c;
        c.setCo2Emissions(120);
        REQUIRE(c.getCo2Emissions() == 120);
    }

    SECTION("setCo2Class (string) stores the enum value") {
        Car c;
        c.setCo2Class("A");
        REQUIRE(c.getCo2Class() == Co2Class::A);
    }

    SECTION("setCo2Class (enum) stores the value directly") {
        Car c;
        c.setCo2Class(Co2Class::B);
        REQUIRE(c.getCo2Class() == Co2Class::B);
    }

    SECTION("setStillInSale stores the value") {
        Car c;
        c.setStillInSale(true);
        REQUIRE(c.isStillInSale() == true);
    }

    SECTION("setDataSource (string) stores the enum value") {
        Car c;
        c.setDataSource("Argus");
        REQUIRE(c.getDataSource() == DataSource::ARGUS);
    }

    SECTION("setDataSource (enum) stores the value directly") {
        Car c;
        c.setDataSource(DataSource::ARGUS);
        REQUIRE(c.getDataSource() == DataSource::ARGUS);
    }

}


// =============================================================================
// Tests — Setters : string formatting
// =============================================================================

TEST_CASE("Car Setters — String Formatting", "[car][setters][formatting]") {

    SECTION("setBrand uppercases the value") {
        Car c;
        c.setBrand("toyota");
        REQUIRE(c.getBrand() == "TOYOTA");
    }

    SECTION("setBrand uppercases mixed case") {
        Car c;
        c.setBrand("tOyOtA");
        REQUIRE(c.getBrand() == "TOYOTA");
    }

    SECTION("setModel capitalizes the first letter only") {
        Car c;
        c.setModel("corolla");
        REQUIRE(c.getModel() == "Corolla");
    }

    SECTION("setTrim capitalizes the first letter only") {
        Car c;
        c.setTrim("expression");
        REQUIRE(c.getTrim() == "Expression");
    }

    SECTION("setGeneration preserves the original case") {
        Car c;
        c.setGeneration("e210");
        REQUIRE(c.getGeneration() == "e210");
    }
    
    SECTION("setPhase preserves the original case") {
        Car c;
        c.setPhase("phAse2");
        REQUIRE(c.getPhase() == "phAse2");
    }

    SECTION("setEngine preserves the original case") {
        Car c;
        c.setEngine("1.8L Hybrid");
        REQUIRE(c.getEngine() == "1.8L Hybrid");
    }

    SECTION("setFuelType (string) is case-insensitive") {
        Car c;
        c.setFuelType("ESSENCE");
        REQUIRE(c.getFuelType() == FuelType::ES);
        c.setFuelType("essence");
        REQUIRE(c.getFuelType() == FuelType::ES);
    }

    SECTION("setGearboxType (string) is case-insensitive") {
        Car c;
        c.setGearboxType("AUTOMATIQUE");
        REQUIRE(c.getGearboxType() == GearboxType::AUTOMATIC);
    }

    SECTION("setCo2Class (string) is case-insensitive") {
        Car c;
        c.setCo2Class("a");
        REQUIRE(c.getCo2Class() == Co2Class::A);
    }

    SECTION("setDataSource (string) is case-insensitive") {
        Car c;
        c.setDataSource("ARguS");
        REQUIRE(c.getDataSource() == DataSource::ARGUS);
    }

}


// =============================================================================
// Tests — Setters : invalid values
// =============================================================================

TEST_CASE("Car Setters — Invalid Values", "[car][setters][invalid]") {

    // --- Strings vides ---

    SECTION("setBrand with empty string stores ERROR_STR") {
        Car c;
        c.setBrand("");
        REQUIRE(c.getBrand() == ERROR_STR);
    }

    SECTION("setModel with empty string stores ERROR_STR") {
        Car c;
        c.setModel("");
        REQUIRE(c.getModel() == ERROR_STR);
    }

    SECTION("setGeneration with empty string stores ERROR_STR") {
        Car c;
        c.setGeneration("");
        REQUIRE(c.getGeneration() == ERROR_STR);
    }

    SECTION("setEngine with empty string stores ERROR_STR") {
        Car c;
        c.setEngine("");
        REQUIRE(c.getEngine() == ERROR_STR);
    }

    SECTION("setTrim with empty string stores ERROR_STR") {
        Car c;
        c.setTrim("");
        REQUIRE(c.getTrim() == ERROR_STR);
    }

    // --- Numériques hors borne inférieure ---

    SECTION("setPrice with negative value stores ERROR_INT") {
        Car c;
        c.setPrice(-1);
        REQUIRE(c.getPrice() == ERROR_INT);
    }

    SECTION("setPrice above max stores ERROR_INT") {
        Car c;
        c.setPrice(10000001);
        REQUIRE(c.getPrice() == ERROR_INT);
    }

    SECTION("setHeight with negative value stores ERROR_DOUBLE") {
        Car c;
        c.setHeight(-1.0);
        REQUIRE(c.getHeight() == ERROR_DOUBLE);
    }

    SECTION("setLength with negative value stores ERROR_DOUBLE") {
        Car c;
        c.setLength(-1.0);
        REQUIRE(c.getLength() == ERROR_DOUBLE);
    }

    SECTION("setWidth with negative value stores ERROR_DOUBLE") {
        Car c;
        c.setWidth(-1.0);
        REQUIRE(c.getWidth() == ERROR_DOUBLE);
    }

    SECTION("setWeight with negative value stores ERROR_INT") {
        Car c;
        c.setWeight(-1);
        REQUIRE(c.getWeight() == ERROR_INT);
    }

    SECTION("setTrunkVolume with negative value stores ERROR_INT") {
        Car c;
        c.setTrunkVolume(-1);
        REQUIRE(c.getTrunkVolume() == ERROR_INT);
    }

    SECTION("setDoorCount with negative value stores ERROR_INT") {
        Car c;
        c.setDoorCount(-1);
        REQUIRE(c.getDoorCount() == ERROR_INT);
    }

    SECTION("setSeatCount above max (10) stores ERROR_INT") {
        Car c;
        c.setSeatCount(11);
        REQUIRE(c.getSeatCount() == ERROR_INT);
    }

    SECTION("setGearboxType (string) with unknown value stores NA") {
        Car c;
        c.setGearboxType("unknown_gearbox");
        REQUIRE(c.getGearboxType() == GearboxType::NA);
    }

    SECTION("setGearboxType (string) with empty string stores NA") {
        Car c;
        c.setGearboxType("");
        REQUIRE(c.getGearboxType() == GearboxType::NA);
    }

    SECTION("setGearCount with negative value stores ERROR_INT") {
        Car c;
        c.setGearCount(-1);
        REQUIRE(c.getGearCount() == ERROR_INT);
    }

    SECTION("setFuelType (string) with unknown value stores NA") {
        Car c;
        c.setFuelType("unknown_fuel");
        REQUIRE(c.getFuelType() == FuelType::NA);
    }

    SECTION("setFuelType (string) with empty string stores NA") {
        Car c;
        c.setFuelType("");
        REQUIRE(c.getFuelType() == FuelType::NA);
    }

    SECTION("setHorsePower with negative value stores ERROR_INT") {
        Car c;
        c.setHorsePower(-1);
        REQUIRE(c.getHorsePower() == ERROR_INT);
    }

    SECTION("setHorsePower above max (3000) stores ERROR_INT") {
        Car c;
        c.setHorsePower(3001);
        REQUIRE(c.getHorsePower() == ERROR_INT);
    }

    SECTION("setTaxHorsePower with negative value stores ERROR_INT") {
        Car c;
        c.setTaxHorsePower(-1);
        REQUIRE(c.getTaxHorsePower() == ERROR_INT);
    }

    SECTION("setTankCapacity with negative value stores ERROR_INT") {
        Car c;
        c.setTankCapacity(-1);
        REQUIRE(c.getTankCapacity() == ERROR_INT);
    }

    SECTION("setFuelConsumption with negative value stores ERROR_DOUBLE") {
        Car c;
        c.setFuelConsumption(-1.0);
        REQUIRE(c.getFuelConsumption() == ERROR_DOUBLE);
    }

    SECTION("setFuelConsumption above max (50.0) stores ERROR_DOUBLE") {
        Car c;
        c.setFuelConsumption(50.1);
        REQUIRE(c.getFuelConsumption() == ERROR_DOUBLE);
    }

    SECTION("setCo2Emissions with negative value stores ERROR_INT") {
        Car c;
        c.setCo2Emissions(-1);
        REQUIRE(c.getCo2Emissions() == ERROR_INT);
    }

    SECTION("setCo2Class (string) with unknown value stores NA") {
        Car c;
        c.setCo2Class("Z");
        REQUIRE(c.getCo2Class() == Co2Class::NA);
    }

    SECTION("setCo2Class (string) with empty string stores NA") {
        Car c;
        c.setCo2Class("");
        REQUIRE(c.getCo2Class() == Co2Class::NA);
    }

    SECTION("setDataSource (string) with unknown value stores NA") {
        Car c;
        c.setDataSource("InvalidSource");
        REQUIRE(c.getDataSource() == DataSource::NA);
    }

    SECTION("setDataSource (string) with empty string stores NA") {
        Car c;
        c.setDataSource("");
        REQUIRE(c.getDataSource() == DataSource::NA);
    }

}


// =============================================================================
// Tests — Setters : boundary values
// =============================================================================

TEST_CASE("Car Setters — Boundary Values", "[car][setters][boundary]") {

    SECTION("setPrice at min boundary (0) is valid") {
        Car c;
        c.setPrice(0);
        REQUIRE(c.getPrice() == 0);
    }

    SECTION("setPrice at max boundary (10 000 000) is valid") {
        Car c;
        c.setPrice(10000000);
        REQUIRE(c.getPrice() == 10000000);
    }

    SECTION("setSeatCount at min boundary (0) is valid") {
        Car c;
        c.setSeatCount(0);
        REQUIRE(c.getSeatCount() == 0);
    }

    SECTION("setSeatCount at max boundary (10) is valid") {
        Car c;
        c.setSeatCount(10);
        REQUIRE(c.getSeatCount() == 10);
    }

    SECTION("setDoorCount at min boundary (0) is valid") {
        Car c;
        c.setDoorCount(0);
        REQUIRE(c.getDoorCount() == 0);
    }

    SECTION("setDoorCount at max boundary (10) is valid") {
        Car c;
        c.setDoorCount(10);
        REQUIRE(c.getDoorCount() == 10);
    }

    SECTION("setHorsePower at min boundary (0) is valid") {
        Car c;
        c.setHorsePower(0);
        REQUIRE(c.getHorsePower() == 0);
    }

    SECTION("setHorsePower at max boundary (3000) is valid") {
        Car c;
        c.setHorsePower(3000);
        REQUIRE(c.getHorsePower() == 3000);
    }

    SECTION("setFuelConsumption at min boundary (0.0) is valid") {
        Car c;
        c.setFuelConsumption(0.0);
        REQUIRE(c.getFuelConsumption() == Approx(0.0));
    }

    SECTION("setFuelConsumption at max boundary (50.0) is valid") {
        Car c;
        c.setFuelConsumption(50.0);
        REQUIRE(c.getFuelConsumption() == Approx(50.0));
    }

}


// =============================================================================
// Tests — Setters : commercialisation dates
// =============================================================================

TEST_CASE("Car Setters — Commercialisation Dates", "[car][setters][dates]") {

    SECTION("setCommercialisationStart with valid DD/MM/YYYY stores the date") {
        Car c;
        c.setCommercialisationStart("01/01/2012");
        REQUIRE(c.getCommercialisationStart().has_value());
        REQUIRE(static_cast<int>(c.getCommercialisationStart()->year())         == 2012);
        REQUIRE(static_cast<unsigned>(c.getCommercialisationStart()->month())   == 1);
        REQUIRE(static_cast<unsigned>(c.getCommercialisationStart()->day())     == 1);
    }

    SECTION("setCommercialisationEnd with valid DD/MM/YYYY stores the date") {
        Car c;
        c.setCommercialisationEnd("31/12/2019");
        REQUIRE(c.getCommercialisationEnd().has_value());
        REQUIRE(static_cast<int>(c.getCommercialisationEnd()->year())           == 2019);
        REQUIRE(static_cast<unsigned>(c.getCommercialisationEnd()->month())     == 12);
        REQUIRE(static_cast<unsigned>(c.getCommercialisationEnd()->day())       == 31);
    }

    SECTION("setCommercialisationStart with malformed string stores nullopt") {
        Car c;
        c.setCommercialisationStart("2019");
        REQUIRE_FALSE(c.getCommercialisationStart().has_value());
    }

    SECTION("setCommercialisationStart with empty string stores nullopt") {
        Car c;
        c.setCommercialisationStart("");
        REQUIRE_FALSE(c.getCommercialisationStart().has_value());
    }

    SECTION("setCommercialisationStart with impossible date stores nullopt") {
        Car c;
        c.setCommercialisationStart("31/02/2012");
        REQUIRE_FALSE(c.getCommercialisationStart().has_value());
    }

    SECTION("Unset commercialisationEnd means still in sale") {
        Car c;
        c.setCommercialisationStart("01/01/2020");
        REQUIRE(c.getCommercialisationStart().has_value());
        REQUIRE_FALSE(c.getCommercialisationEnd().has_value());
    }

    SECTION("formatDate round-trip is consistent with stored date") {
        Car c;
        c.setCommercialisationStart("15/06/2015");
        REQUIRE(c.getCommercialisationStart().has_value());
        REQUIRE(Validation::formatDate(c.getCommercialisationStart().value()) == "15/06/2015");
    }

}


// =============================================================================
// Tests — Co2Class enum conversions
// =============================================================================

TEST_CASE("Car Co2Class Conversions", "[car][enum][co2class]") {

    SECTION("setCo2Class A through G via string") {
        Car c;
        const std::vector<std::pair<std::string, Co2Class>> cases = {
            {"A", Co2Class::A}, {"B", Co2Class::B}, {"C", Co2Class::C},
            {"D", Co2Class::D}, {"E", Co2Class::E}, {"F", Co2Class::F},
            {"G", Co2Class::G}
        };
        for (const auto& [str, expected] : cases) {
            c.setCo2Class(str);
            REQUIRE(c.getCo2Class() == expected);
        }
    }

    SECTION("setCo2Class unknown string stores NA") {
        Car c;
        c.setCo2Class("H");
        REQUIRE(c.getCo2Class() == Co2Class::NA);
    }

}


// =============================================================================
// Tests — FuelType enum conversions
// =============================================================================

TEST_CASE("Car FuelType Conversions", "[car][enum][fueltype]") {

    SECTION("setFuelType Diesel (GO)") {
        Car c;
        c.setFuelType("Diesel");
        REQUIRE(c.getFuelType() == FuelType::GO);
    }

    SECTION("setFuelType Hybride essence/electrique (EE)") {
        Car c;
        c.setFuelType("Hybride : Essence/Electrique");
        REQUIRE(c.getFuelType() == FuelType::EE);
    }

    SECTION("setFuelType Electrique (EL)") {
        Car c;
        c.setFuelType("Electrique");
        REQUIRE(c.getFuelType() == FuelType::EL);
    }

    SECTION("setFuelType GPL (GP)") {
        Car c;
        c.setFuelType("GPL");
        REQUIRE(c.getFuelType() == FuelType::GP);
    }

    SECTION("setFuelType Hydrogene (H2)") {
        Car c;
        c.setFuelType("hydrogene");
        REQUIRE(c.getFuelType() == FuelType::H2);
    }

    SECTION("setFuelType unknown string stores NA") {
        Car c;
        c.setFuelType("fusion nucléaire");
        REQUIRE(c.getFuelType() == FuelType::NA);
    }

}


// =============================================================================
// Tests — GearboxType enum conversions
// =============================================================================

TEST_CASE("Car GearboxType Conversions", "[car][enum][gearbox]") {

    SECTION("setGearboxType Automatique") {
        Car c;
        c.setGearboxType("Automatique");
        REQUIRE(c.getGearboxType() == GearboxType::AUTOMATIC);
    }

    SECTION("setGearboxType Manuelle") {
        Car c;
        c.setGearboxType("Manuelle");
        REQUIRE(c.getGearboxType() == GearboxType::MANUAL);
    }

    SECTION("setGearboxType unknown string stores NA") {
        Car c;
        c.setGearboxType("robotisée");
        REQUIRE(c.getGearboxType() == GearboxType::NA);
    }

}


// =============================================================================
// Tests — DataSource enum conversions
// =============================================================================

TEST_CASE("Car DataSource Conversions", "[car][enum][datasource]") {

    SECTION("setDataSource Argus") {
        Car c;
        c.setDataSource("Argus");
        REQUIRE(c.getDataSource() == DataSource::ARGUS);
    }

    SECTION("setDataSource Argus") {
        Car c;
        c.setDataSource("Argus");
        REQUIRE(c.getDataSource() == DataSource::ARGUS);
    }

    SECTION("setDataSource unknown string stores NA") {
        Car c;
        c.setDataSource("robotisée");
        REQUIRE(c.getDataSource() == DataSource::NA);
    }

}


// =============================================================================
// Tests — isComplete / isValid
// =============================================================================

TEST_CASE("Car isComplete and isValid", "[car][utilities]") {

    // --- isComplete ---

    SECTION("Default car is not complete") {
        Car c;
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Fully populated car is complete") {
        Car c = buildValidCar();
        REQUIRE(c.isComplete());
    }

    SECTION("Car missing brand is not complete") {
        Car c = buildValidCar();
        c.setBrand(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing model is not complete") {
        Car c = buildValidCar();
        c.setModel(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing price is not complete") {
        Car c = buildValidCar();
        c.setPrice(DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with FuelType::NA is not complete") {
        Car c = buildValidCar();
        c.setFuelType(FuelType::NA);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with GearboxType::NA is not complete") {
        Car c = buildValidCar();
        c.setGearboxType(GearboxType::NA);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with Co2Class::NA is not complete") {
        Car c = buildValidCar();
        c.setCo2Class(Co2Class::NA);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with DataSource::NA is not complete") {
        Car c = buildValidCar();
        c.setDataSource(DataSource::NA);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with invalid start date is not complete") {
        Car c = buildValidCar();
        c.setCommercialisationStart("31/02/2019");   // date invalide → nullopt
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car with no end date is complete (still in sale)") {
        Car c = buildValidCar();
        // _commercialisationEnd non renseignée = encore commercialisée = valide
        REQUIRE(c.isComplete());
    }

    SECTION("Car missing generation is not complete") {
        Car c = buildValidCar();
        c.setGeneration(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing phase is not complete") {
        Car c = buildValidCar();
        c.setPhase(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing engine is not complete") {
        Car c = buildValidCar();
        c.setEngine(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing trim is not complete") {
        Car c = buildValidCar();
        c.setTrim(DEFAULT_STR);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing height is not complete") {
        Car c = buildValidCarSkipping("height");
        REQUIRE(c.getHeight() == DEFAULT_DOUBLE);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing length is not complete") {
        Car c = buildValidCarSkipping("length");
        REQUIRE(c.getLength() == DEFAULT_DOUBLE);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing width is not complete") {
        Car c = buildValidCarSkipping("width");
        REQUIRE(c.getWidth() == DEFAULT_DOUBLE);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing trunkVolume is not complete") {
        Car c = buildValidCarSkipping("trunkVolume");
        REQUIRE(c.getTrunkVolume() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing weight is not complete") {
        Car c = buildValidCarSkipping("weight");
        REQUIRE(c.getWeight() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing doorCount is not complete") {
        Car c = buildValidCarSkipping("doorCount");
        REQUIRE(c.getDoorCount() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing seatCount is not complete") {
        Car c = buildValidCarSkipping("seatCount");
        REQUIRE(c.getSeatCount() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing gearCount is not complete") {
        Car c = buildValidCarSkipping("gearCount");
        REQUIRE(c.getGearCount() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing horsePower is not complete") {
        Car c = buildValidCarSkipping("horsePower");
        REQUIRE(c.getHorsePower() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing taxHorsePower is not complete") {
        Car c = buildValidCarSkipping("taxHorsePower");
        REQUIRE(c.getTaxHorsePower() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing tankCapacity is not complete") {
        Car c = buildValidCarSkipping("tankCapacity");
        REQUIRE(c.getTankCapacity() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing fuelConsumption is not complete") {
        Car c = buildValidCarSkipping("fuelConsumption");
        REQUIRE(c.getFuelConsumption() == DEFAULT_DOUBLE);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Car missing co2Emissions is not complete") {
        Car c = buildValidCarSkipping("co2Emissions");
        REQUIRE(c.getCo2Emissions() == DEFAULT_INT);
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("buildValidCarSkipping still yields a complete car when nothing is skipped") {
        Car c = buildValidCarSkipping("none");
        REQUIRE(c.isComplete());
    }


    // --- isValid ---

    SECTION("Default car is valid (no error values)") {
        Car c;
        REQUIRE(c.isValid());
    }

    SECTION("Fully populated car is valid") {
        Car c = buildValidCar();
        REQUIRE(c.isValid());
    }

    SECTION("Car with ERROR_STR brand is not valid") {
        Car c;
        c.setBrand("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT price is not valid") {
        Car c;
        c.setPrice(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_DOUBLE height is not valid") {
        Car c;
        c.setHeight(-1.0);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT horsePower is not valid") {
        Car c;
        c.setHorsePower(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_DOUBLE fuelConsumption is not valid") {
        Car c;
        c.setFuelConsumption(-1.0);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_STR model is not valid") {
        Car c;
        c.setModel("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_STR generation is not valid") {
        Car c;
        c.setGeneration("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_STR phase is not valid") {
        Car c;
        c.setPhase("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_STR engine is not valid") {
        Car c;
        c.setEngine("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_STR trim is not valid") {
        Car c;
        c.setTrim("");
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_DOUBLE length is not valid") {
        Car c;
        c.setLength(-1.0);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_DOUBLE width is not valid") {
        Car c;
        c.setWidth(-1.0);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT trunkVolume is not valid") {
        Car c;
        c.setTrunkVolume(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT weight is not valid") {
        Car c;
        c.setWeight(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT doorCount is not valid") {
        Car c;
        c.setDoorCount(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT seatCount is not valid") {
        Car c;
        c.setSeatCount(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT gearCount is not valid") {
        Car c;
        c.setGearCount(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT taxHorsePower is not valid") {
        Car c;
        c.setTaxHorsePower(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT tankCapacity is not valid") {
        Car c;
        c.setTankCapacity(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Car with ERROR_INT co2Emissions is not valid") {
        Car c;
        c.setCo2Emissions(-1);
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("GearboxType::NA alone does not make car invalid") {
        Car c;
        REQUIRE(c.isValid());
    }

    SECTION("FuelType::NA alone does not make car invalid") {
        Car c;
        REQUIRE(c.isValid());
    }

}




// =============================================================================
// Tests — toString
// =============================================================================

TEST_CASE("Car toString", "[car][tostring]") {

    SECTION("Contains the FullId as a header") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find(c.getFullId()) != std::string::npos);
    }

    SECTION("Contains all section headers") {
        Car c = buildValidCar();
        const std::string s = c.toString();
        REQUIRE(s.find("- General")             != std::string::npos);
        REQUIRE(s.find("- Dimensions")          != std::string::npos);
        REQUIRE(s.find("- Liveability")         != std::string::npos);
        REQUIRE(s.find("- Transmission")        != std::string::npos);
        REQUIRE(s.find("- Power")               != std::string::npos);
        REQUIRE(s.find("- Consumption")         != std::string::npos);
        REQUIRE(s.find("- Commercialisation")   != std::string::npos);
    }

    SECTION("Contains the brand normalized value") {
        Car c = buildValidCar();
        // setBrand uppercases the value
        REQUIRE(c.toString().find("\"TOYOTA\"") != std::string::npos);
    }

    SECTION("Contains the model normalized value") {
        Car c = buildValidCar();
        // setModel capitalizes the first letter only
        REQUIRE(c.toString().find("\"Corolla\"") != std::string::npos);
    }

    SECTION("Contains the price with the euro sign") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find("25000 \u20ac") != std::string::npos);
    }

    SECTION("Contains the gearbox type as a string label") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find(gearBoxTypeToString(c.getGearboxType())) != std::string::npos);
    }

    SECTION("Contains the fuel type as a string label") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find(fuelTypeToString(c.getFuelType())) != std::string::npos);
    }

    SECTION("Contains the co2 class as a string label") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find(co2ClassToString(c.getCo2Class())) != std::string::npos);
    }

    SECTION("Contains the data source as a string label") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find(dataSourceToString(c.getDataSource())) != std::string::npos);
    }

    SECTION("Contains the formatted commercialisation start date") {
        Car c = buildValidCar();
        REQUIRE(c.toString().find("\"01/01/2019\"") != std::string::npos);
    }

    SECTION("Default (unset) car shows the sentinel start date as N/A") {
        Car c;
        REQUIRE(c.toString().find("\"N/A\"") != std::string::npos);
    }

    SECTION("Default (unset) car shows DEFAULT_STR sentinel values") {
        Car c;
        REQUIRE(c.toString().find("\"" + DEFAULT_STR + "\"") != std::string::npos);
    }
}




// =============================================================================
// Tests — Operators
// =============================================================================

TEST_CASE("Car operator<<", "[car][operator][stream]") {

    SECTION("Stream insertion produces the same output as toString()") {
        Car c = buildValidCar();
        std::ostringstream oss;
        oss << c;
        REQUIRE(oss.str() == c.toString());
    }

    SECTION("Stream insertion works for a default (unset) car") {
        Car c;
        std::ostringstream oss;
        oss << c;
        REQUIRE(oss.str() == c.toString());
    }

    SECTION("Stream insertion can be chained like a regular ostream") {
        Car c = buildValidCar();
        std::ostringstream oss;
        oss << "Car dump:\n" << c << "--- end ---";
        const std::string s = oss.str();
        REQUIRE(s.find("Car dump:") != std::string::npos);
        REQUIRE(s.find("--- end ---") != std::string::npos);
        REQUIRE(s.find(c.getFullId()) != std::string::npos);
    }

}


TEST_CASE("Car operator isEqual, == and !=", "[car][operator][equalities]") {

    SECTION("isEqual Identifies two identic cars") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        REQUIRE(c1.isEqualTo(c2));
    }

    SECTION("isEqual Identifies two different cars") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        c2.setBrand("Another");
        REQUIRE_FALSE(c1.isEqualTo(c2));
        c2 = c1;
        c2.setHeight(4);
        REQUIRE_FALSE(c1.isEqualTo(c2));
        c2 = c1;
        c2.setGearboxType(GearboxType::MANUAL);
        REQUIRE_FALSE(c1.isEqualTo(c2));
    }
    
    SECTION("== Identifies two identic cars") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        REQUIRE(c1 == c2);
    }

    SECTION("!= Identifies two different cars") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        c2.setBrand("Another");
        REQUIRE(c1 != c2);
        c2 = c1;
        c2.setHeight(4);
        REQUIRE(c1 != c2);
        c2 = c1;
        c2.setGearboxType(GearboxType::MANUAL);
        REQUIRE(c1 != c2);
    }

    SECTION("== Ignore Commercialisation") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        Car c3 = c1;
        c2.setCommercialisationStart("01/01/2009");
        c2.setCommercialisationEnd("01/01/2023");
        REQUIRE(c1 == c2);
        REQUIRE(c1 == c3);
        REQUIRE(c2 == c3);
    }


    SECTION("== Ignore Technical Data") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        Car c3 = c1;
        c2.setDataSource(DataSource::CARADISIAC);
        c3.setFromDuplicatedSource(false);
        REQUIRE(c1 == c2);
        REQUIRE(c1 == c3);
        REQUIRE(c2 == c3);
    }

}

TEST_CASE("Car operator isEquivalent", "[car][operator][equivalence]") {

    // ---- Same car ------------------------------------------------------------------------------
    SECTION("isEquivalent is true when cars are the same") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        REQUIRE(c1.isEquivalentTo(c2));
        REQUIRE(c2.isEquivalentTo(c1));
    }

    SECTION("isEquivalent is case insensitve with brand and model") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        c2.setBrand("ToyOTa");
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setModel("corOLLa");
        REQUIRE(c1.isEquivalentTo(c2));
    }


    // ---- Equivalent cars -----------------------------------------------------------------------
    SECTION("isEquivalent spots two car with the same trim (case-insensitive)") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setTrim                   ("DYNAMIC");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent spots two car with the same egine (case-insensitive)") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setEngine                 ("1.8L HYBRID");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent spots two car with the same horsePower (5 hp allowance)") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setHorsePower             (122);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setHorsePower(127);
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setHorsePower(117);
        REQUIRE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent spots two car with the same weight (50 kg allowance)") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setEngine                 ("1.8L HYBRID");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setWeight                 (1375);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setWeight(1325);
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setWeight(1425);
        REQUIRE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent spots two car with the same price (500 € allowance)") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setEngine                 ("1.8L HYBRID");
        c2.setPrice                  (25000);
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setPrice(24500);
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setPrice(25500);
        REQUIRE(c1.isEquivalentTo(c2));
    }


    // ---- Not equivalent cars -------------------------------------------------------------------
    SECTION("isEquivalent is false when not enough info") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2019");
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        REQUIRE_FALSE(c2.isEquivalentTo(c1));
    }

    SECTION("isEquivalent is false when year are different") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setTrim                   ("DYNAMIC");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        c2.setCommercialisationStart ("01/01/2018");
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent does not throw when date is not set and is false") {
        Car c1 = buildValidCar();
        Car c2;
        c2.setBrand                  ("Toyota");
        c2.setModel                  ("Corolla");
        c2.setTrim                   ("DYNAMIC");
        c2.setHeight                 (1.43);
        c2.setLength                 (4.37);
        c2.setWidth                  (1.79);
        c2.setSeatCount              (5);
        c2.setGearboxType            (GearboxType::AUTOMATIC);
        c2.setFuelType               (FuelType::EE);
        c2.setTaxHorsePower          (6);
        REQUIRE_NOTHROW(c1.isEquivalentTo(c2));
        REQUIRE_NOTHROW(c2.isEquivalentTo(c1));
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
    }

    SECTION("isEquivalent is false when basic check not true") {
        Car c1 = buildValidCar();
        Car c2 = c1;
        REQUIRE(c1.isEquivalentTo(c2));
        c2.setBrand("Toyot");
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setBrand("Toyota");
        c2.setModel("Coroll");
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setModel("Corolla");
        c2.setHeight(1.42);
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setHeight(1.43);
        c2.setSeatCount(4);
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setSeatCount(5);
        c2.setGearboxType(GearboxType::MANUAL);
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setGearboxType(GearboxType::AUTOMATIC);
        c2.setFuelType(FuelType::GO);
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
        c2.setFuelType(FuelType::EE);
        c2.setTaxHorsePower(5);
        REQUIRE_FALSE(c1.isEquivalentTo(c2));
    }

}