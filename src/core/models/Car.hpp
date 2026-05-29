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
     * @class Car (inheriting from @class Entity):
     *
     * @attributes:
     * - General:
     *  -> brand            : (string) Car brand                        | ex: "Renault"
     *  -> model            : (string) Car model                        | ex: "Clio"
     *  -> generation       : (string) Car generation                   | ex: "4"
     *  -> engine           : (string) Car engine                       | ex: "1.5 dCi 90hp"
     *  -> trim             : (string) Car trim                         | ex: "Expression"
     *  -> price            : (int) Car price                           | ex: 15000             | in €
     * - Dimensions:
     *  -> height           : (double) Car height                       | ex: 1.45              | in meters
     *  -> length           : (double) Car length                       | ex: 4.05              | in meters
     *  -> width            : (double) Car width                        | ex: 1.75              | in meters
     *  -> trunkVolume      : (int) Car trunk volume                    | ex: 300               | in liters
     *  -> weight           : (int) Car weight                          | ex: 1200              | in kg
     *  -> seatCount        : (int) Car seat count                      | ex: 5
     * - Transmission:
     *  -> gearboxType      : (string) Car gearbox type                 | ex: "Manual"
     *  -> gearCount        : (int) Car gear count                      | ex: 5
     * - Power:
     *  -> fuelType         : (string) Car fuel type                    | ex: "Diesel"
     *  -> horsePower       : (int) Car horse power                     | ex: 90                | in DIN hp
     *  -> taxHorsePower    : (int) Car tax horse power                 | ex: 5
     * - Consumption:
     *  -> tankCapacity     : (int) Car tank capacity                   | ex: 50                | in liters
     *  -> fuelConsumption  : (double) Car fuel consumption             | ex: 4.5               | in L/100km
     *  -> co2Emissions     : (int) Car co2 emissions                   | ex: 120               | in g/km
     *  -> co2Class         : (string) Car co2 class                    | ex: "B"
     * - Years:
     *  -> yearStart        : (string) Car commercialisation start year | ex: "2012"
     *  -> yearEnd          : (string) Car commercialisation end year   | ex: "2019"
     *
     * @getters:
     * - General:
     *  -> getBrand()           : (string) returns the brand of the car
     *  -> getModel()           : (string) returns the model of the car
     *  -> getGeneration()      : (string) returns the generation of the car
     *  -> getEngine()          : (string) returns the engine of the car
     *  -> getTrim()            : (string) returns the trim of the car
     *  -> getPrice()           : (int) returns the price of the car
     * - Dimensions:
     *  -> getHeight()          : (double) returns the height of the car
     *  -> getLength()          : (double) returns the length of the car
     *  -> getWidth()           : (double) returns the width of the car
     *  -> getTrunkVolume()     : (int) returns the trunk volume of the car
     *  -> getWeight()          : (int) returns the weight of the car
     *  -> getSeatCount()       : (int) returns the seat count of the car
     * - Transmission:
     *  -> getGearboxType()     : (string) returns the gearbox type of the car
     *  -> getGearCount()       : (int) returns the gear count of the car
     * - Power:
     *  -> getFuelType()        : (string) returns the fuel type of the car
     *  -> getHorsePower()      : (int) returns the horse power of the car
     *  -> getTaxHorsePower()   : (int) returns the tax horse power of the car
     * - Consumption:
     *  -> getTankCapacity()    : (int) returns the tank capacity of the car
     *  -> getFuelConsumption() : (double) returns the fuel consumption of the car
     *  -> getCo2Emissions()    : (int) returns the CO2 emissions of the car
     *  -> getCo2Class()        : (string) returns the CO2 class of the car
     * - Years:
     *  -> getYearStart()       : (string) returns the start year of the car
     *  -> getYearEnd()         : (string) returns the end year of the car
     *
     * @setters:
     * - General:
     *  -> setBrand()           : sets the brand of the car             | max length: 100 characters
     *  -> setModel()           : sets the model of the car             | max length: 200 characters
     *  -> setGeneration()      : sets the generation of the car        | max length: 50 characters
     *  -> setEngine()          : sets the engine of the car            | max length: 200 characters
     *  -> setTrim()            : sets the trim of the car              | max length: 200 characters
     *  -> setPrice()           : sets the price of the car             | min value: 0 €, max value: 10 000 000 €
     * - Dimensions:
     *  -> setHeight()          : sets the height of the car            | min value: 0.0 m, max value: 10.0 m
     *  -> setLength()          : sets the length of the car            | min value: 0.0 m, max value: 10.0 m
     *  -> setWidth()           : sets the width of the car             | min value: 0.0 m, max value: 10.0 m
     *  -> setTrunkVolume()     : sets the trunk volume of the car      | min value: 0 L, max value: 1000 L
     *  -> setWeight()          : sets the weight of the car            | min value: 0 kg, max value: 5000 kg
     *  -> setSeatCount()       : sets the seat count of the car        | min value: 0, max value: 10
     * - Transmission:
     *  -> setGearboxType()     : sets the gearbox type of the car      | max length: 50 characters
     *  -> setGearCount()       : sets the gear count of the car        | min value: 0, max value: 20
     * - Power:
     *  -> setFuelType()        : sets the fuel type of the car         | max length: 50 characters
     *  -> setHorsePower()      : sets the horse power of the car       | min value: 0 hp, max value: 3000 hp
     *  -> setTaxHorsePower()   : sets the tax horse power of the car   | min value: 0 hp, max value: 200
     * - Consumption:
     *  -> setTankCapacity()    : sets the tank capacity of the car     | min value: 0 L, max value: 500 L
     *  -> setFuelConsumption() : sets the fuel consumption of the car  | min value: 0.0 L/100km, max value: 50.0 L/100km
     *  -> setCo2Emissions()    : sets the CO2 emissions of the car     | min value: 0 g/km, max value: 200 g/km
     *  -> setCo2Class()        : sets the CO2 class of the car         | max length: 10 characters
     * - Years:
     *  -> setYearStart()       : sets the start year of the car
     *  -> setYearEnd()         : sets the end year of the car
     *
     * @utilities:
     * - isComplete()           : returns true if the car data has been properly filled (no default values or error values)
     * - isValid()              : returns true if the car data is valid (no error values)
     * - stringValidation()     : validates and normalizes a string value (trimming, length check, UTF-8 normalization)
     * - intValidation()        : validates an int value (range check)
     * - doubleValidation()     : validates a double value (range check)
     * - toString()             : returns a string representation of the car (for debugging purposes)
     *
     * @operators:
     * - operator<<             : stream operator overload for easy display of the car information
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


            // String validation and normalization
            static std::string stringValidation(    const std::string&      strVal,
                                                    const std::string&      attributeName,
                                                    const long unsigned int maxLength = 255);

            
            // Int validation
            static int intValidation(   const int           intVal,
                                        const std::string&  attributeName,
                                        const int           minValue = 0,
                                        const int           maxValue = 9999);


            // Double validation
            static double doubleValidation( const double        doubleVal,
                                            const std::string&  attributeName,
                                            const double        minValue = 0.0,
                                            const double        maxValue = 9999.0);


            // Debug display
            std::string toString() const;

    };





    // =========================================================================
    // Operators
    // =========================================================================
    std::ostream& operator<<(std::ostream& os, const Car& car);

} // namespace CarScraper