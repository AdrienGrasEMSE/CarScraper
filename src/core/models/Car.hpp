// For only one include
#pragma once


// Imports
#include "Entity.hpp"
#include <string>
#include <ostream>
#include <iomanip>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <stduuid/uuid.h>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * Class Car (exetension of Entity):
     *
     * Represents a car with the following characteristics:
     * - General:
     *      -> brand            | string    | ex: "Renault"         | no unit
     *      -> model            | string    | ex: "Clio"            | no unit
     *      -> generation       | string    | ex: "4"               | no unit
     *      -> engine           | string    | ex: "1.5 dCi 90hp"    | no unit
     *      -> trim             | string    | ex: "Expression"      | no unit
     *      -> price            | int       | ex: 15000             | in €
     * - Dimensions:
     *      -> height           | double    | ex: 1.45              | in meters
     *      -> length           | double    | ex: 4.05              | in meters
     *      -> width            | double    | ex: 1.75              | in meters
     *      -> trunkVolume      | int       | ex: 300               | in liters
     *      -> weight           | int       | ex: 1200              | in kg
     *      -> seatCount        | int       | ex: 5                 | no unit
     * - Transmission:
     *      -> gearboxType      | string    | ex: "Manual"          | no unit
     *      -> gearCount        | int       | ex: 5                 | no unit
     * - Power:
     *      -> fuelType         | string    | ex: "Diesel"          | no unit
     *      -> horsePower       | int       | ex: 90                | in DIN hp
     *      -> taxHorsePower    | int       | ex: 5                 | in fiscal hp
     * - Consumption:
     *      -> tankCapacity     | int       | ex: 50                | in liters
     *      -> fuelConsumption  | double    | ex: 4.5               | in L/100km
     *      -> co2Emissions     | int       | ex: 120               | in g/km
     *      -> co2Class         | string    | ex: "B"               | no unit
     * - Years:
     *      -> yearStart        | string    | ex: "2012"            | no unit
     *      -> yearEnd          | string    | ex: "2019"            | no unit
     *
     */
    class Car : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            // General
            std::string     _brand;
            std::string     _model;
            std::string     _generation;
            std::string     _engine;
            std::string     _trim;
            int             _price;

            // Dimensions
            double          _height;
            double          _length;
            double          _width;
            int             _trunkVolume;
            int             _weight;
            int             _seatCount;

            // Transmission
            std::string     _gearboxType;
            int             _gearCount;

            // Power
            std::string     _fuelType;
            int             _horsePower;
            int             _taxHorsePower;

            // Consumption
            int             _tankCapacity;
            double          _fuelConsumption;
            int             _co2Emissions;
            std::string     _co2Class;

            // Years
            std::string     _yearStart;
            std::string     _yearEnd;





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------
            Car();




            
            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            // General
            const std::string&  getBrand()              const { return _brand; }
            const std::string&  getModel()              const { return _model; }
            const std::string&  getGeneration()         const { return _generation; }
            const std::string&  getEngine()             const { return _engine; }
            const std::string&  getTrim()               const { return _trim; }
            int                 getPrice()              const { return _price; }

            // Dimensions
            double              getHeight()             const { return _height; }
            double              getLength()             const { return _length; }
            double              getWidth()              const { return _width; }
            int                 getTrunkVolume()        const { return _trunkVolume; }
            int                 getWeight()             const { return _weight; }
            int                 getSeatCount()          const { return _seatCount; }

            // Transmission
            const std::string&  getGearboxType()        const { return _gearboxType; }
            int                 getGearCount()          const { return _gearCount; }

            // Power
            const std::string&  getFuelType()           const { return _fuelType; }
            int                 getHorsePower()         const { return _horsePower; }
            int                 getTaxHorsePower()      const { return _taxHorsePower; }

            // Consumption
            int                 getTankCapacity()       const { return _tankCapacity; }
            double              getFuelConsumption()    const { return _fuelConsumption; }
            int                 getCo2Emissions()       const { return _co2Emissions; }
            const std::string&  getCo2Class()           const { return _co2Class; }

            // Years
            const std::string&  getYearStart()          const { return _yearStart; }
            const std::string&  getYearEnd()            const { return _yearEnd; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------

            // General
            void setBrand           (const  std::string& brand);
            void setModel           (const  std::string& model);
            void setGeneration      (const  std::string& generation);
            void setEngine          (const  std::string& engine);
            void setTrim            (const  std::string& trim);
            void setPrice           (int    price);

            // Dimensions
            void setHeight          (double height);
            void setLength          (double length);
            void setWidth           (double width);
            void setTrunkVolume     (int    trunkVolume);
            void setWeight          (int    weight);
            void setSeatCount       (int    seatCount);

            // Transmission
            void setGearboxType     (const  std::string& gearboxType);
            void setGearCount       (int    gearCount);

            // Power
            void setFuelType        (const  std::string& fuelType);
            void setHorsePower      (int    horsePower);
            void setTaxHorsePower   (int    taxHorsePower);

            // Consumption
            void setTankCapacity    (int    tankCapacity);
            void setFuelConsumption (double fuelConsumption);
            void setCo2Emissions    (int    co2Emissions);
            void setCo2Class        (const  std::string& co2Class);

            // Years
            void setYearStart       (const  std::string& yearStart);
            void setYearEnd         (const  std::string& yearEnd);





            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            // Returns true if the car data has been properly filled (no default values or error values)
            bool isComplete() const;
            bool isValid() const;

            // Debug display
            std::string toString() const;

    };





    // =========================================================================
    // Operators
    // =========================================================================
    std::ostream& operator<<(std::ostream& os, const Car& car);

} // namespace CarScraper