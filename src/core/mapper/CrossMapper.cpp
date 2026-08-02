/**
 * @file CrossMapper.cpp
 * 
 * @brief This file implements the CrossMapper class, which is a mapper able to complete a Car Entity using
 * two Car generated from largus and caradisiac technical file.
 *
 * @author Adrien GRAS
 * @date 2026-07-28
 */


// Imports
#include "CrossMapper.hpp"
#include "core/utils/Constant.hpp"
#include "core/models/Entity.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor
     */
    CrossMapper::CrossMapper() : Entity("CROSS-MAPPER") {}





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Join two Car entity to create a more complete one
     * @param largus The Car entity extracted from a largus technical file
     * @param caradisac The Car entity extracted from a cardisiac technical file
     * @return The completed Car entity
     */
    Car CrossMapper::crossMap(Car largus, Car caradisiac) {

        // Generating a new Car using the most viable data from wheter largus or caradisiac
        Car newCar;


        /**
         * - Consumption
         *     -> TankCapacity     : Largus
         *     -> FuelConsumption  : Largus
         *     -> Co2Emissions     : Largus
         *     -> Co2Class         : Largus
         * - Commercialisation
         *     -> Start            : Caradisiac
         *     -> End              : Caradisiac
         *     -> Still in Sale    : Caradisiac
         */


        /** ----- General -------------------------------------------------------------------------
         *  -> Brand        : Largus
         *  -> Model        : Largus
         *  -> Generation   : Caradisiac
         *  -> Phase        : Caradisiac
         *  -> Engine       : Largus
         *  -> Trim         : Caradisiac
         *  -> Price        : Largus
         */
        newCar.setBrand(largus.getBrand());
        newCar.setModel(largus.getModel());
        newCar.setGeneration(caradisiac.getGeneration());
        newCar.setPhase(caradisiac.getPhase());
        newCar.setEngine(largus.getEngine());
        newCar.setTrim(caradisiac.getTrim());
        newCar.setPrice(largus.getPrice());



        /** ----- Dimension -----------------------------------------------------------------------
         *  -> Everything from Largus
         */
        newCar.setHeight(largus.getHeight());
        newCar.setLength(largus.getLength());
        newCar.setWidth(largus.getWidth());
        newCar.setWeight(largus.getWeight());



        /** ----- Liveability ---------------------------------------------------------------------
         *  -> TrunkVolume  : Largus
         *  -> DoorCount    : Caradisiac
         *  -> SeatCount    : Largus
         */
        newCar.setTrunkVolume(largus.getTrunkVolume());
        newCar.setDoorCount(caradisiac.getDoorCount());
        newCar.setSeatCount(largus.getSeatCount());



        /** ----- Transmission --------------------------------------------------------------------
         *  -> Everything from Caradisiac
         */
        newCar.setGearboxType(caradisiac.getGearboxType());
        newCar.setGearCount(caradisiac.getGearCount());



        /** ----- Power ---------------------------------------------------------------------------
         *  -> Everything from Largus
         */
        newCar.setFuelType(largus.getFuelType());
        newCar.setHorsePower(largus.getHorsePower());
        newCar.setTaxHorsePower(largus.getTaxHorsePower());



        /** ----- Consumption ---------------------------------------------------------------------
         *  -> Everything from Largus
         */
        newCar.setTankCapacity(largus.getTankCapacity());
        newCar.setFuelConsumption(largus.getFuelConsumption());
        newCar.setCo2Emissions(largus.getCo2Emissions());
        newCar.setCo2Class(largus.getCo2Class());



        /** ----- Commercialisation ---------------------------------------------------------------
         *  -> Everything from Caradisiac
         */
        newCar.setCommercialisationStart(caradisiac.getCommercialisationStart());
        newCar.setCommercialisationEnd(caradisiac.getCommercialisationEnd());
        newCar.setStillInSale(caradisiac.isStillInSale());



        /** ----- Technical Data ------------------------------------------------------------------
         */
        newCar.setDataSource(CarScraper::DataSource::MIXED);
        newCar.setSourceFile(NONE_STR);



        // Returning the generated car
        return newCar;

    }

} // namespace CarScraper