/**
 * @file baseCar.cpp
 *
 * @brief Unit tests for CarScraper::Car class.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Imports
#include <catch2/catch.hpp>
#include "core/logger/Logger.hpp"
#include "core/enum/FuelType.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/Co2Class.hpp"
#include "core/utils/Constant.hpp"
#include "core/utils/Validation.hpp"
#include "core/models/Car.hpp"


/**
 * Using namespace CarScraper for constants values and logging
 */
using namespace CarScraper;


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

    SECTION("Two cars have different UUIDs") {
        Car c1;
        Car c2;
        REQUIRE(c1.getUuid() != c2.getUuid());
    }

    SECTION("All string attributes are DEFAULT_STR") {
        Car c;
        REQUIRE(c.getBrand()      == DEFAULT_STR);
        REQUIRE(c.getModel()      == DEFAULT_STR);
        REQUIRE(c.getGeneration() == DEFAULT_STR);
        REQUIRE(c.getEngine()     == DEFAULT_STR);
        REQUIRE(c.getTrim()       == DEFAULT_STR);
    }

    SECTION("All int attributes are DEFAULT_INT") {
        Car c;
        REQUIRE(c.getPrice()        == DEFAULT_INT);
        REQUIRE(c.getTrunkVolume()  == DEFAULT_INT);
        REQUIRE(c.getWeight()       == DEFAULT_INT);
        REQUIRE(c.getSeatCount()    == DEFAULT_INT);
        REQUIRE(c.getGearCount()    == DEFAULT_INT);
        REQUIRE(c.getHorsePower()   == DEFAULT_INT);
        REQUIRE(c.getTaxHorsePower()== DEFAULT_INT);
        REQUIRE(c.getTankCapacity() == DEFAULT_INT);
        REQUIRE(c.getCo2Emissions() == DEFAULT_INT);
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
        REQUIRE(c.getGearboxType() == GearboxType::NA);
        REQUIRE(c.getFuelType()    == FuelType::NA);
        REQUIRE(c.getCo2Class()    == Co2Class::NA);
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

    SECTION("setTrunkVolume stores the value") {
        Car c;
        c.setTrunkVolume(300);
        REQUIRE(c.getTrunkVolume() == 300);
    }

    SECTION("setWeight stores the value") {
        Car c;
        c.setWeight(1200);
        REQUIRE(c.getWeight() == 1200);
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

}


// =============================================================================
// Tests — Setters : invalid values
// =============================================================================

TEST_CASE("Car Setters — Invalid Values", "[car][setters][invalid]") {

    SECTION("setBrand with empty string stores ERROR_STR") {
        Car c;
        c.setBrand("");
        REQUIRE_THAT(c.getBrand(), Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
    }

    SECTION("setModel with empty string stores ERROR_STR") {
        Car c;
        c.setModel("");
        REQUIRE_THAT(c.getModel(), Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
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
        REQUIRE_THAT(c.getTrim(), Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));

    }

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

    SECTION("setTrunkVolume with negative value stores ERROR_INT") {
        Car c;
        c.setTrunkVolume(-1);
        REQUIRE(c.getTrunkVolume() == ERROR_INT);
    }

    SECTION("setWeight with negative value stores ERROR_INT") {
        Car c;
        c.setWeight(-1);
        REQUIRE(c.getWeight() == ERROR_INT);
    }

    SECTION("setSeatCount with negative value stores ERROR_INT") {
        Car c;
        c.setSeatCount(-1);
        REQUIRE(c.getSeatCount() == ERROR_INT);
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
// Tests — isComplete / isValid
// =============================================================================

TEST_CASE("Car isComplete and isValid", "[car][utilities]") {

    SECTION("Default car is not complete") {
        Car c;
        REQUIRE_FALSE(c.isComplete());
    }

    SECTION("Default car is valid (no error values)") {
        Car c;
        REQUIRE(c.isValid());
    }

    SECTION("Car with error values is not valid") {
        Car c;
        c.setBrand("");                 // → ERROR_STR
        REQUIRE_FALSE(c.isValid());
    }

    SECTION("Fully populated car is complete and valid") {
        Car c;
        c.setBrand("Toyota");
        c.setModel("Corolla");
        c.setGeneration("E210");
        c.setEngine("1.8L Hybrid");
        c.setTrim("Dynamic");
        c.setPrice(25000);
        c.setHeight(1.43);
        c.setLength(4.37);
        c.setWidth(1.79);
        c.setTrunkVolume(361);
        c.setWeight(1375);
        c.setSeatCount(5);
        c.setGearboxType(GearboxType::AUTOMATIC);
        c.setGearCount(1);
        c.setFuelType(FuelType::EE);
        c.setHorsePower(122);
        c.setTaxHorsePower(6);
        c.setTankCapacity(43);
        c.setFuelConsumption(4.3);
        c.setCo2Emissions(98);
        c.setCo2Class(Co2Class::A);
        c.setCommercialisationStart("01/01/2019");
        c.setStillInSale(true);
        REQUIRE(c.isComplete());
        REQUIRE(c.isValid());
    }

}