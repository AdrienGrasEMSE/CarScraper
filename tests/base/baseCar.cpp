/**
 * @file baseCar.cpp
 *
 * @brief Unit tests for CarScraper::Car functions.
 *
 * @author Adrien GRAS
 * @date 2026-05-29
 */


// Imports
#include <catch2/catch.hpp>
#include "core/models/Car.hpp"
#include "core/utils/Constant.hpp"
#include "core/logger/Logger.hpp"


/**
 * Using namespace CarScraper for constants values and logging
 */
using namespace CarScraper;


// =============================================================================
// Tests — Car Getters and Setters
// =============================================================================

TEST_CASE("Car Getters", "[car][getters]") {

    SECTION("Car has default values after default construction") {
        Car e;
        REQUIRE_FALSE(e.getUuid().empty());
        REQUIRE_THAT(e.getBrand(),              Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getModel(),              Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getGeneration(),         Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getEngine(),             Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getTrim(),               Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getPrice()                    == DEFAULT_INT);
        REQUIRE(e.getHeight()                   == DEFAULT_DOUBLE);
        REQUIRE(e.getLength()                   == DEFAULT_DOUBLE);
        REQUIRE(e.getWidth()                    == DEFAULT_DOUBLE);
        REQUIRE(e.getTrunkVolume()              == DEFAULT_INT);
        REQUIRE(e.getWeight()                   == DEFAULT_INT);
        REQUIRE(e.getSeatCount()                == DEFAULT_INT);
        REQUIRE_THAT(e.getGearboxType(),        Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getGearCount()                == DEFAULT_INT);
        REQUIRE_THAT(e.getFuelType(),           Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getHorsePower()               == DEFAULT_INT);
        REQUIRE(e.getTaxHorsePower()            == DEFAULT_INT);
        REQUIRE(e.getTankCapacity()             == DEFAULT_INT);
        REQUIRE(e.getFuelConsumption()          == DEFAULT_DOUBLE);
        REQUIRE(e.getCo2Emissions()             == DEFAULT_INT);
        REQUIRE_THAT(e.getCo2Class(),           Catch::Matchers::Equals(DEFAULT_STR, Catch::CaseSensitive::No));
    }

}


TEST_CASE("Car Setters", "[car][setters]") {

    SECTION("Setters correctly update the car data") {
        Car e;
        e.setBrand                  ("TOYOTA");
        e.setModel                  ("Corolla");
        e.setGeneration             ("E210");
        e.setEngine                 ("1.8L Hybrid");
        e.setTrim                   ("Dynamic");
        e.setPrice                  (25000);
        e.setHeight                 (1.43);
        e.setLength                 (4.37);
        e.setWidth                  (1.79);
        e.setTrunkVolume            (361);
        e.setWeight                 (1375);
        e.setSeatCount              (5);
        e.setGearboxType            ("Automatic");
        e.setGearCount              (0);
        e.setFuelType               ("Hybrid");
        e.setHorsePower             (122);
        e.setTaxHorsePower          (6);
        e.setTankCapacity           (43);
        e.setFuelConsumption        (4.3);
        e.setCo2Emissions           (98);
        e.setCo2Class               ("A");
        e.setCommercialisationStart ("2019");
        e.setCommercialisationEnd   ("2026");
        e.setStillInSale            (true);

        REQUIRE_THAT(e.getBrand(),              Catch::Matchers::Equals("TOYOTA", Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getModel(),              Catch::Matchers::Equals("Corolla", Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getGeneration(),         Catch::Matchers::Equals("E210", Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getEngine(),             Catch::Matchers::Equals("1.8L Hybrid", Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getTrim(),               Catch::Matchers::Equals("Dynamic", Catch::CaseSensitive::No));
        REQUIRE(e.getPrice()                    == 25000);
        REQUIRE(e.getHeight()                   == 1.43);
        REQUIRE(e.getLength()                   == 4.37);
        REQUIRE(e.getWidth()                    == 1.79);
        REQUIRE(e.getTrunkVolume()              == 361);
        REQUIRE(e.getWeight()                   == 1375);
        REQUIRE(e.getSeatCount()                == 5);
        REQUIRE_THAT(e.getGearboxType(),        Catch::Matchers::Equals("Automatic", Catch::CaseSensitive::No));
        REQUIRE(e.getGearCount()                == 0);
        REQUIRE_THAT(e.getFuelType(),           Catch::Matchers::Equals("Hybrid", Catch::CaseSensitive::No));
        REQUIRE(e.getHorsePower()               == 122);
        REQUIRE(e.getTaxHorsePower()            == 6);
        REQUIRE(e.getTankCapacity()             == 43);
        REQUIRE(e.getFuelConsumption()          == 4.3);
        REQUIRE(e.getCo2Emissions()             == 98);
        REQUIRE_THAT(e.getCo2Class(),           Catch::Matchers::Equals("A", Catch::CaseSensitive::No));
    }


    SECTION("Setters correctly format input values") {
        Car e;
        e.setBrand                  ("toYotA");
        e.setModel                  ("corOLLa");
        e.setGeneration             ("e210");
        e.setEngine                 ("1.8L hyBrid");
        e.setTrim                   ("dynaMic");
        e.setGearboxType            ("auTomAtIC");
        e.setFuelType               ("hyBrid");
        e.setCo2Class               ("a");

        REQUIRE(e.getBrand()        == "TOYOTA");
        REQUIRE(e.getModel()        == "Corolla");
        REQUIRE(e.getGeneration()   == "e210");
        REQUIRE(e.getEngine()       == "1.8L hyBrid");
        REQUIRE(e.getTrim()         == "Dynamic");
        REQUIRE(e.getGearboxType()  == "Automatic");
        REQUIRE(e.getFuelType()     == "Hybrid");
        REQUIRE(e.getCo2Class()     == "A");
    }


    SECTION("Setters correctly handle invalid input values") {
        Car e;
        e.setBrand                  ("");
        e.setModel                  ("");
        e.setGeneration             ("");
        e.setEngine                 ("");
        e.setTrim                   ("");
        e.setPrice                  (-20);
        e.setHeight                 (-20.0);
        e.setLength                 (-20.0);
        e.setWidth                  (-20.0);
        e.setTrunkVolume            (-20);
        e.setWeight                 (-20);
        e.setSeatCount              (-20);
        e.setGearboxType            ("");
        e.setGearCount              (-20);
        e.setFuelType               ("");
        e.setHorsePower             (-20);
        e.setTaxHorsePower          (-20);
        e.setTankCapacity           (-20);
        e.setFuelConsumption        (-20.0);
        e.setCo2Emissions           (-20);
        e.setCo2Class               ("");
        e.setCommercialisationStart ("");
        e.setCommercialisationEnd   ("");

        REQUIRE_THAT(e.getBrand(),              Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getModel(),              Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getGeneration(),         Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getEngine(),             Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE_THAT(e.getTrim(),               Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getPrice()                    == ERROR_INT);
        REQUIRE(e.getHeight()                   == ERROR_DOUBLE);
        REQUIRE(e.getLength()                   == ERROR_DOUBLE);
        REQUIRE(e.getWidth()                    == ERROR_DOUBLE);
        REQUIRE(e.getTrunkVolume()              == ERROR_INT);
        REQUIRE(e.getWeight()                   == ERROR_INT);
        REQUIRE(e.getSeatCount()                == ERROR_INT);
        REQUIRE_THAT(e.getGearboxType(),        Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getGearCount()                == ERROR_INT);
        REQUIRE_THAT(e.getFuelType(),           Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
        REQUIRE(e.getHorsePower()               == ERROR_INT);
        REQUIRE(e.getTaxHorsePower()            == ERROR_INT);
        REQUIRE(e.getTankCapacity()             == ERROR_INT);
        REQUIRE(e.getFuelConsumption()          == ERROR_DOUBLE);
        REQUIRE(e.getCo2Emissions()             == ERROR_INT);
        REQUIRE_THAT(e.getCo2Class(),           Catch::Matchers::Equals(ERROR_STR, Catch::CaseSensitive::No));
    }

}
