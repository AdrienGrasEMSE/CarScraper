/**
 * @file Car.hpp
 * 
 * @brief This file declares the Car class, which represents a car entity.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 */


// For only one include
#pragma once


// Imports
#include "Entity.hpp"
#include "core/enum/FuelType.hpp"
#include "core/enum/GearboxType.hpp"
#include "core/enum/Co2Class.hpp"
#include <string>
#include <ostream>
#include <iomanip>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <stduuid/uuid.h>
#include <chrono>
#include <optional>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class Car
     * 
     * @brief Represent a car entity with various attributes and methods
     * @details Car is inherited from Entity. Every car has a UUID
     */
    class Car : public Entity {

        // =========================================================================
        // Private data and methods
        // =========================================================================
        private:

            // General
            std::string     _brand;                     ///< Car brand                        | ex: "Renault"
            std::string     _model;                     ///< Car model                        | ex: "Clio"
            std::string     _generation;                ///< Car generation                   | ex: "4"
            std::string     _engine;                    ///< Car engine                       | ex: "1.5 dCi 90hp"
            std::string     _trim;                      ///< Car trim                         | ex: "Expression"
            int             _price;                     ///< Car price                        | ex: 15000

            // Dimensions
            double          _height;                    ///< Car height                       | ex: 1.45
            double          _length;                    ///< Car length                       | ex: 4.05
            double          _width;                     ///< Car width                        | ex: 1.75
            int             _trunkVolume;               ///< Car trunk volume                 | ex: 300
            int             _weight;                    ///< Car weight                       | ex: 1200
            int             _seatCount;                 ///< Car seat count                   | ex: 5

            // Transmission
            CarScraper::GearboxType _gearboxType;       ///< Car gearbox type                 | AUTOMATIC / MANUAL
            int                     _gearCount;         ///< Car gear count                   | ex: 5

            // Power
            CarScraper::FuelType    _fuelType;          ///< Car fuel type                    | ex: EE / EL / ES / GL / GO / GP / H2
            int                     _horsePower;        ///< Car horse power                  | ex: 90
            int                     _taxHorsePower;     ///< Car tax horse power              | ex: 5

            // Consumption
            int                     _tankCapacity;      ///< Car tank capacity                | ex: 50
            double                  _fuelConsumption;   ///< Car fuel consumption             | ex: 4.5
            int                     _co2Emissions;      ///< Car co2 emissions                | ex: 120
            CarScraper::Co2Class    _co2Class;          ///< Car co2 class                    | A / B / C / D / E / F / G

            // Commercialisation
            std::optional<std::chrono::year_month_day>  _commercialisationStart;    ///< Car commercialisation start year | ex: "01/01/2012"
            std::optional<std::chrono::year_month_day>  _commercialisationEnd;      ///< Car commercialisation end year   | ex: "08/09/2019"
            bool                                        _stillInSale;               ///< Car still in sale                | ex: false




        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default constructor : initializes all the car data with default values
             */
            Car();




            
            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /** @brief Gets the brand of the car
             *  @return The brand of the car
             */
            const std::string& getBrand() const { return _brand; }


            /** @brief Gets the model of the car
             *  @return The model of the car
             */
            const std::string& getModel() const { return _model; }


            /** @brief Gets the generation of the car
             *  @return The generation of the car
             */
            const std::string& getGeneration() const { return _generation; }


            /** @brief Gets the engine of the car
             *  @return The engine of the car
             */
            const std::string& getEngine() const { return _engine; }


            /** @brief Gets the trim of the car
             *  @return The trim of the car
             */
            const std::string& getTrim() const { return _trim; }

            
            /** @brief Gets the price of the car
             *  @return The price of the car
             */
            int getPrice() const { return _price; }

            
            /** @brief Gets the height of the car
             *  @return The height of the car
             */
            double getHeight() const { return _height; }


            /** @brief Gets the length of the car
             *  @return The length of the car
             */
            double getLength() const { return _length; }


            /** @brief Gets the width of the car
             *  @return The width of the car
             */
            double getWidth() const { return _width; }


            /** @brief Gets the trunk volume of the car
             *  @return The trunk volume of the car
             */
            int getTrunkVolume() const { return _trunkVolume; }

            
            /** @brief Gets the weight of the car
             *  @return The weight of the car
             */
            int getWeight() const { return _weight; }


            /** @brief Gets the seat count of the car
             *  @return The seat count of the car
             */
            int getSeatCount() const { return _seatCount; }


            /** @brief Gets the type of the gearbox
             *  @return The type of the gearbox
             */
            CarScraper::GearboxType getGearboxType() const { return _gearboxType; }


            /** @brief Gets the number of gears
             *  @return The number of gears
             */
            int getGearCount() const { return _gearCount; }


            /** @brief Gets the type of fuel
             *  @return The type of fuel
             */
            CarScraper::FuelType getFuelType() const { return _fuelType; }


            /** @brief Gets the horse power of the car
             *  @return The horse power of the car
             */
            int getHorsePower() const { return _horsePower; }


            /** @brief Gets the tax horse power of the car
             *  @return The tax horse power of the car
             */
            int getTaxHorsePower() const { return _taxHorsePower; }

            
            /** @brief Gets the tank capacity of the car
             *  @return The tank capacity of the car
             */
            int getTankCapacity() const { return _tankCapacity; }


            /** @brief Gets the fuel consumption of the car
             *  @return The fuel consumption of the car
             */
            double getFuelConsumption() const { return _fuelConsumption; }


            /** @brief Gets the CO2 emissions of the car
             *  @return The CO2 emissions of the car
             */
            int getCo2Emissions() const { return _co2Emissions; }


            /** @brief Gets the CO2 class of the car
             *  @return The CO2 class of the car
             */
            CarScraper::Co2Class getCo2Class() const { return _co2Class; }


            /** @brief Gets the commercialisation start year of the car
             *  @return The commercialisation start year of the car
             */
            const std::optional<std::chrono::year_month_day>& getCommercialisationStart() const { return _commercialisationStart; }


            /** @brief Gets the commercialisation end year of the car
             *  @return The commercialisation end year of the car
             */
            const std::optional<std::chrono::year_month_day>& getCommercialisationEnd() const { return _commercialisationEnd; }


            /** @brief Gets whether the car is still in sale
             *  @return Whether the car is still in sale
             */
            bool isStillInSale() const { return _stillInSale; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the brand value
             * @param brand The brand value
             */
            void setBrand(const std::string& brand);


            /**
             * @brief Sets the model value
             * @param model The model value
             */
            void setModel(const std::string& model);


            /**
             * @brief Sets the generation value
             * @param generation The generation value
             */
            void setGeneration(const std::string& generation);


            /**
             * @brief Sets the engine value
             * @param engine The engine value
             */
            void setEngine(const std::string& engine);


            /**
             * @brief Sets the trim value
             * @param trim The trim value
             */
            void setTrim(const std::string& trim);


            /**
             * @brief Sets the price value
             * @param price The price value
             */
            void setPrice(int price);


            /**
             * @brief Sets the height value
             * @param height The height value
             */
            void setHeight(double height);


            /**
             * @brief Sets the length value
             * @param length The length value
             */
            void setLength(double length);


            /**
             * @brief Sets the width value
             * @param width The width value
             */
            void setWidth(double width);


            /**
             * @brief Sets the trunk volume value
             * @param trunkVolume The trunk volume value
             */
            void setTrunkVolume(int trunkVolume);


            /**
             * @brief Sets the weight value
             * @param weight The weight value
             */
            void setWeight(int weight);


            /**
             * @brief Sets the seat count value
             * @param seatCount The seat count value
             */
            void setSeatCount(int seatCount);


            /**
             * @brief Sets the gear box type
             * @param gearboxType The gear box type
             */
            void setGearboxType(const CarScraper::GearboxType gearboxType);


            /**
             * @brief Sets the gear box type
             * @param gearboxType The gear box type
             */
            void setGearboxType(const std::string& gearboxType);


            /**
             * @brief Sets the gear count value
             * @param gearCount The gear count value
             */
            void setGearCount(int gearCount);


            /**
             * @brief Sets the fuel type
             * @param fuelType The fuel type
             */
            void setFuelType(const CarScraper::FuelType fuelType);


            /**
             * @brief Sets the fuel type
             * @param fuelType The fuel type
             */
            void setFuelType(const std::string& fuelType);


            /**
             * @brief Sets the horse power value
             * @param horsePower The horse power value
             */
            void setHorsePower(int horsePower);


            /**
             * @brief Sets the tax horse power value
             * @param taxHorsePower The tax horse power value
             */
            void setTaxHorsePower(int taxHorsePower);


            /**
             * @brief Sets the tank capacity value
             * @param tankCapacity The tank capacity value
             */
            void setTankCapacity(int tankCapacity);


            /**
             * @brief Sets the fuel consumption value
             * @param fuelConsumption The fuel consumption value
             */
            void setFuelConsumption(double fuelConsumption);


            /**
             * @brief Sets the CO2 emissions value
             * @param co2Emissions The CO2 emissions value
             */
            void setCo2Emissions(int co2Emissions);


            /**
             * @brief Sets the CO2 class value
             * @param co2Class The CO2 class value
             */
            void setCo2Class(const CarScraper::Co2Class co2Class);


            /**
             * @brief Sets the CO2 class value
             * @param co2Class The CO2 class value
             */
            void setCo2Class(const std::string& co2Class);


            /**
             * @brief Sets the commercialisation start date
             * @param commercialisationStart The commercialisation start date
             */
            void setCommercialisationStart(const std::string& commercialisationStart);


            /**
             * @brief Sets the commercialisation end date
             * @param commercialisationEnd The commercialisation end date
             */
            void setCommercialisationEnd(const std::string& commercialisationEnd);


            /**
             * Sets the still in sale value
             * @param stillInSale The still in sale value
             */
            void setStillInSale(bool stillInSale);





            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            /**
             * @brief Checks if the car data has been filled (no default sentinel values)
             *
             * @return true if the car data is complete and valid, false otherwise
             */
            bool isComplete() const;


            /**
             * @brief Checks if the car data has been properly filled (no error sentinel values)
             *
             * @return true if the car data is valid, false otherwise
             */
            bool isValid() const;


            /**
             * @brief Returns a string representation of the car (for debugging purposes)
             * 
             * @return the string representation of the car
             */
            std::string toString() const;

    };





    // =========================================================================
    // Operators
    // =========================================================================

    /**
     * @brief Operator << overload for the stream compatibility (uses the Car.toString() method)
     * 
     * @param os the targeted stream
     * @param car the targeted car
     */
    std::ostream& operator<<(std::ostream& os, const Car& car);

} // namespace CarScraper