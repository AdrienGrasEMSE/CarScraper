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
     * @details Car is inherited from Entity and uses the prefix "CAR".
     *
     * Every car has a UUID, some general information, dimensions, transmission
     * information, power information, consumption information and commercialisation
     * information.
     *
     * The class provides getters and setters for each attribute, as well as utility methods.
     * Every values are set with their corresponding default sentinel value. Every setter method
     * validates the input value. In a case of invalid value, it sets the error sentinel value
     * and logs an error message.
     *
     * @see Entity
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
            int             _price;                     ///< Car price (in €)                 | ex: 15000

            // Dimensions
            double          _height;                    ///< Car height (in m)                | ex: 1.45
            double          _length;                    ///< Car length (in m)                | ex: 4.05
            double          _width;                     ///< Car width (in m)                 | ex: 1.75
            int             _trunkVolume;               ///< Car trunk volume (in L)          | ex: 300
            int             _weight;                    ///< Car weight (in kg)               | ex: 1200
            int             _seatCount;                 ///< Car seat count                   | ex: 5

            // Transmission
            CarScraper::GearboxType _gearboxType;       ///< Car gearbox type                 | AUTOMATIC / MANUAL
            int                     _gearCount;         ///< Car gear count                   | ex: 5

            // Power
            CarScraper::FuelType    _fuelType;          ///< Car fuel type                    | ex: EE / EL / ES / GL / GO / GP / H2
            int                     _horsePower;        ///< Car horse power (in DIN hp)      | ex: 90
            int                     _taxHorsePower;     ///< Car tax horse power              | ex: 5

            // Consumption
            int                     _tankCapacity;      ///< Car tank capacity (in L)         | ex: 50
            double                  _fuelConsumption;   ///< Car fuel consumption (in L/100km)| ex: 4.5
            int                     _co2Emissions;      ///< Car co2 emissions (in g/km)      | ex: 120
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
             * @brief Default constructor
             * @details Initializes all the car data with sentinel default values
             * @note The car entity uses the CAR prefix for its full ID (ex: "CAR-123e45...")
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
             * @details The brand value is normalized (converted to uppercase)
             * @param brand The brand value
             * @note The maximum length accepted is 100 characters
             */
            void setBrand(const std::string& brand);


            /**
             * @brief Sets the model value
             * @details The model value is normalized (converted to title case)
             * @param model The model value
             * @note The maximum length accepted is 200 characters
             */
            void setModel(const std::string& model);


            /**
             * @brief Sets the generation value
             * @details The generation value is normalized
             * @param generation The generation value
             * @note The maximum length accepted is 50 characters
             */
            void setGeneration(const std::string& generation);


            /**
             * @brief Sets the engine value
             * @details The engine value is normalized
             * @param engine The engine value
             * @note The maximum length accepted is 200 characters
             */
            void setEngine(const std::string& engine);


            /**
             * @brief Sets the trim value
             * @details The trim value is normalized (converted to title case)
             * @param trim The trim value
             * @note The maximum length accepted is 200 characters
             */
            void setTrim(const std::string& trim);


            /**
             * @brief Sets the price value
             * @param price The price value
             * @note The price value must be between 0 and 10 million (inclusive)
             */
            void setPrice(int price);


            /**
             * @brief Sets the height value
             * @param height The height value
             * @note The height value must be between 0 and 10 meters (inclusive)
             */
            void setHeight(double height);


            /**
             * @brief Sets the length value
             * @param length The length value
             * @note The length value must be between 0 and 10 meters (inclusive)
             */
            void setLength(double length);


            /**
             * @brief Sets the width value
             * @param width The width value
             * @note The width value must be between 0 and 10 meters (inclusive)
             */
            void setWidth(double width);


            /**
             * @brief Sets the trunk volume value
             * @param trunkVolume The trunk volume value
             * @note The trunk volume value must be between 0 and 1000 liters (inclusive)
             */
            void setTrunkVolume(int trunkVolume);


            /**
             * @brief Sets the weight value
             * @param weight The weight value
             * @note The weight value must be between 0 and 5000 kg (inclusive)
             */
            void setWeight(int weight);


            /**
             * @brief Sets the seat count value
             * @param seatCount The seat count value
             * @note The seat count value must be between 0 and 10 (inclusive)
             */
            void setSeatCount(int seatCount);


            /**
             * @brief Sets the gear box type using an enum value
             * @param gearboxType The gear box type
             */
            void setGearboxType(const CarScraper::GearboxType gearboxType);


            /**
             * @brief Sets the gear box type using a string value
             * @details The string value will be converted to the corresponding enum value
             * using the CarScraper::gearBoxTypeFromString() method. If the string value is invalid,
             * the gear boxtype will be set to NA and an error message will be logged.
             * @param gearboxType The gear box type
             */
            void setGearboxType(const std::string& gearboxType);


            /**
             * @brief Sets the gear count value
             * @param gearCount The gear count value
             * @note The gear count value must be between 0 and 20 (inclusive)
             */
            void setGearCount(int gearCount);


            /**
             * @brief Sets the fuel type using an enum value
             * @param fuelType The fuel type
             */
            void setFuelType(const CarScraper::FuelType fuelType);


            /**
             * @brief Sets the fuel type using a string value
             * @details The string value will be converted to the corresponding enum value
             * using the CarScraper::fuelTypeFromString() method. If the string value is invalid,
             * the fuel type will be set to NA and an error message will be logged.
             * @param fuelType The fuel type
             */
            void setFuelType(const std::string& fuelType);


            /**
             * @brief Sets the horse power value
             * @param horsePower The horse power value
             * @note The horse power value must be between 0 and 3000 (inclusive)
             */
            void setHorsePower(int horsePower);


            /**
             * @brief Sets the tax horse power value
             * @param taxHorsePower The tax horse power value
             * @note The tax horse power value must be between 0 and 200 (inclusive)
             */
            void setTaxHorsePower(int taxHorsePower);


            /**
             * @brief Sets the tank capacity value
             * @param tankCapacity The tank capacity value
             * @note The tank capacity value must be between 0 and 500 liters (inclusive)
             */
            void setTankCapacity(int tankCapacity);


            /**
             * @brief Sets the fuel consumption value
             * @param fuelConsumption The fuel consumption value
             * @note The fuel consumption value must be between 0 and 50 L/100 km (inclusive)
             */
            void setFuelConsumption(double fuelConsumption);


            /**
             * @brief Sets the CO2 emissions value
             * @param co2Emissions The CO2 emissions value
             * @note The CO2 emissions value must be between 0 and 1000 g/km (inclusive)
             */
            void setCo2Emissions(int co2Emissions);


            /**
             * @brief Sets the CO2 class value using an enum value
             * @param co2Class The CO2 class value
             */
            void setCo2Class(const CarScraper::Co2Class co2Class);


            /**
             * @brief Sets the CO2 class value using a string value
             * @details The string value will be converted to the corresponding enum value
             * using the CarScraper::co2ClassFromString() method. If the string value is invalid,
             * the CO2 class will be set to NA and an error message will be logged.
             * @param co2Class The CO2 class value
             */
            void setCo2Class(const std::string& co2Class);


            /**
             * @brief Sets the commercialisation start date
             * @param commercialisationStart The commercialisation start date
             * @note The commercialisation start date must be in the format "DD/MM/YYYY" and must be a valid date
             */
            void setCommercialisationStart(const std::string& commercialisationStart);


            /**
             * @brief Sets the commercialisation end date
             * @param commercialisationEnd The commercialisation end date
             * @note The commercialisation end date must be in the format "DD/MM/YYYY" and must be a valid date
             */
            void setCommercialisationEnd(const std::string& commercialisationEnd);


            /**
             * @brief Sets the still in sale value
             * @param stillInSale The still in sale value
             */
            void setStillInSale(bool stillInSale);





            // -------------------------------------------------------------------------
            // Utilities
            // -------------------------------------------------------------------------

            /**
             * @brief Checks if the car data has been filled (no default sentinel values)
             * @return true if the car data is complete and valid, false otherwise
             */
            bool isComplete() const;


            /**
             * @brief Checks if the car data has been properly filled (no error sentinel values)
             * @return true if the car data is valid, false otherwise
             */
            bool isValid() const;


            /**
             * @brief Returns a string representation of the car (for debugging purposes)
             * @return the string representation of the car
             */
            std::string toString() const;


            /**
             * @brief Saves the car data to a file in JSON format
             * @param filename The file path (without the filename). If empty, the file will saved at data/car
             * @return The path of the saved file
             * @note The file will be named with the car full ID (ex: "CAR-123e45...json")
             */
            std::string save(const std::string& filename = "") const;

    };





    // =========================================================================
    // Operators
    // =========================================================================

    /**
     * @brief Operator << overload for the stream compatibility
     * @details Uses the Car.toString() method
     * @param os the targeted stream
     * @param car the targeted car
     * @return the stream with the car string representation inserted
     */
    std::ostream& operator<<(std::ostream& os, const Car& car);

} // namespace CarScraper