/**
 * @file Car.cpp
 * 
 * @brief This file implements the Car class, which represents a car entity.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 *
 * @todo Setup an Energy enum for fuel type and a corresponding setter that accepts both string and enum values (with string validation)
 * @todo Setup a CO2Class enum for CO2 class and a corresponding setter that accepts both string and enum values (with string validation)
 * @todo Setup a GearboxType enum for gearbox type and a corresponding setter that accepts both string and enum values (with string validation)
 */


// Imports
#include "Car.hpp"
#include "core/utils/Constant.hpp"
#include "core/utils/Validation.hpp"
#include "core/logger/Logger.hpp"
#include <sstream>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <stduuid/uuid.h>
#include <random>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * Default constructor: initializes all the car data with default values
     */
    Car::Car() : Entity("CAR") {

        // General
        _brand              = DEFAULT_STR;
        _model              = DEFAULT_STR;
        _generation         = DEFAULT_STR;
        _engine             = DEFAULT_STR;
        _trim               = DEFAULT_STR;
        _price              = DEFAULT_INT;

        // Dimensions
        _height             = DEFAULT_DOUBLE;
        _length             = DEFAULT_DOUBLE;
        _width              = DEFAULT_DOUBLE;
        _trunkVolume        = DEFAULT_INT;
        _weight             = DEFAULT_INT;
        _seatCount          = DEFAULT_INT;

        // Transmission
        _gearboxType        = CarScraper::GearboxType::NA;;
        _gearCount          = DEFAULT_INT;

        // Power
        _fuelType           = CarScraper::FuelType::NA;
        _horsePower         = DEFAULT_INT;
        _taxHorsePower      = DEFAULT_INT;

        // Consumption
        _tankCapacity       = DEFAULT_INT;
        _fuelConsumption    = DEFAULT_DOUBLE;
        _co2Emissions       = DEFAULT_INT;
        _co2Class           = DEFAULT_STR;

        // Commercialisation
        _commercialisationStart  = std::nullopt;
        _commercialisationEnd    = std::nullopt;
        _stillInSale             = false;

    }





    // =========================================================================
    // Getters
    // =========================================================================





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the brand value
     * 
     * @param brand The brand value
     */
    void Car::setBrand(const std::string& brand) {

        // Verification + Normalize UTF-8 (NFC) then apply upper case
        this->_brand = una::cases::to_uppercase_utf8(
            una::norm::to_nfc_utf8(Validation::stringValidation(brand, this->getFullId(), "Brand", 100)));

    }


    /**
     * @brief Sets the model value
     * 
     * @param model The model value
     */
    void Car::setModel(const std::string& model) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_model = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(Validation::stringValidation(model, this->getFullId(), "Model", 200)));

    }


    /**
     * @brief Sets the generation value
     *
     * @param generation The generation value
     */
    void Car::setGeneration(const std::string& generation) {

        // Verification + Normalize UTF-8 (NFC) only — generation is a technical identifier (ex: "4", "IV")
        this->_generation = una::norm::to_nfc_utf8(Validation::stringValidation(generation, this->getFullId(), "Generation", 50));

    }


    /**
     * @brief Sets the engine value
     *
     * @param engine The engine value
     */
    void Car::setEngine(const std::string& engine) {

        // Verification + Normalize UTF-8 (NFC) only — engine is a technical identifier (ex: "1.5 dCi 90hp")
        this->_engine = una::norm::to_nfc_utf8(Validation::stringValidation(engine, this->getFullId(), "Engine", 200));

    }


    /**
     * @brief Sets the trim value
     * 
     * @param trim The trim value
     */
    void Car::setTrim(const std::string& trim) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_trim = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(Validation::stringValidation(trim, this->getFullId(), "Trim", 200)));
    
    }


    /**
     * @brief Sets the price value
     * 
     * @param price The price value
     */
    void Car::setPrice(int price) {

        // Verification
        this->_price = Validation::intValidation(price, this->getFullId(), "Price", 0, 10000000);

    }


    /**
     * @brief Sets the height value
     * 
     * @param height The height value
     */
    void Car::setHeight(double height) {

        // Verification
        this->_height = Validation::doubleValidation(height, this->getFullId(), "Height", 0.0, 10.0);

    }


    /**
     * @brief Sets the length value
     * 
     * @param length The length value
     */
    void Car::setLength(double length) {

        // Verification
        this->_length = Validation::doubleValidation(length, this->getFullId(), "Length", 0.0, 10.0);

    }


    /**
     * @brief Sets the width value
     * 
     * @param width The width value
     */
    void Car::setWidth(double width) {

        // Verification
        this->_width = Validation::doubleValidation(width, this->getFullId(), "Width", 0.0, 10.0);

    }


    /**
     * @brief Sets the trunk volume value
     * 
     * @param trunkVolume The trunk volume value
     */
    void Car::setTrunkVolume(int trunkVolume) {

        // Verification
        this->_trunkVolume = Validation::intValidation(trunkVolume, this->getFullId(), "TrunkVolume", 0, 1000);

    }


    /**
     * @brief Sets the weight value
     * 
     * @param weight The weight value
     */
    void Car::setWeight(int weight) {

        // Verification
        this->_weight = Validation::intValidation(weight, this->getFullId(), "Weight", 0, 5000);

    }


    /**
     * @brief Sets the seat count value
     * 
     * @param seatCount The seat count value
     */
    void Car::setSeatCount(int seatCount) {

        // Verification
        this->_seatCount = Validation::intValidation(seatCount, this->getFullId(), "SeatCount", 0, 10);

    }


    /**
     * @brief Sets the gear box type
     * 
     * @param gearboxType (enum) The gear box type
     */
    void Car::setGearboxType(const CarScraper::GearboxType gearboxType) {

       // Set value
        _gearboxType = gearboxType;


        // Log message
        if (_gearboxType == CarScraper::GearboxType::NA) {
            Logger::error("{}::set{} got an invalid value", this->getFullId(), "GearboxType");
        } else {
            Logger::trace("{}::set{} value: {}", this->getFullId(),
                "GearboxType", CarScraper::gearBoxTypeToString(_gearboxType));
        }

    }


    /**
     * @brief Sets the gear box type
     * 
     * @param gearboxType (string) The gear box type
     */
    void Car::setGearboxType(const std::string& gearboxType) {

       // Set value
        _gearboxType = CarScraper::gearBoxTypeFromString(gearboxType);


        // Log message
        if (_gearboxType == CarScraper::GearboxType::NA) {
            Logger::error("{}::set{} got an invalid value", this->getFullId(), "GearboxType");
        } else {
            Logger::trace("{}::set{} value: {}", this->getFullId(),
                "GearboxType", CarScraper::gearBoxTypeToString(_gearboxType));
        }

    }


    /**
     * @brief Sets the gear count value
     * 
     * @param gearCount The gear count value
     */
    void Car::setGearCount(int gearCount) {

        // Verification
        this->_gearCount = Validation::intValidation(gearCount, this->getFullId(), "GearCount", 0, 20);

    }


    /**
     * @brief Sets the fuel type
     * 
     * @param fuelType (enum) The fuel type
     */
    void Car::setFuelType(const CarScraper::FuelType fuelType) {

        // Set value
        _fuelType = fuelType;


        // Log message
        if (_fuelType == CarScraper::FuelType::NA) {
            Logger::error("{}::set{} got an invalid value", this->getFullId(), "FuelType");
        } else {
            Logger::trace("{}::set{} value: {}", this->getFullId(),
                "FuelType", CarScraper::fuelTypeToString(_fuelType));
        }

    }


    /**
     * @brief Sets the fuel type
     * 
     * @param fuelType (string) The fuel type
     */
    void Car::setFuelType(const std::string& fuelType) {

        // Set value
        _fuelType = CarScraper::fuelTypeFromString(fuelType);


        // Log message
        if (_fuelType == CarScraper::FuelType::NA) {
            Logger::error("{}::set{} got an invalid value", this->getFullId(), "FuelType");
        } else {
            Logger::trace("{}::set{} value: {}", this->getFullId(),
                "FuelType", CarScraper::fuelTypeToString(_fuelType));
        }

    }


    /**
     * @brief Sets the horse power value
     * 
     * @param horsePower The horse power value
     */
    void Car::setHorsePower(int horsePower) {

        // Verification
        this->_horsePower = Validation::intValidation(horsePower, this->getFullId(), "HorsePower", 0, 3000);

    }


    /**
     * @brief Sets the tax horse power value
     * 
     * @param taxHorsePower The tax horse power value
     */
    void Car::setTaxHorsePower(int taxHorsePower) {

        // Verification
        this->_taxHorsePower = Validation::intValidation(taxHorsePower, this->getFullId(), "TaxHorsePower", 0, 200);

    }


    /**
     * @brief Sets the tank capacity value
     * 
     * @param tankCapacity The tank capacity value
     */
    void Car::setTankCapacity(int tankCapacity) {

        // Verification
        this->_tankCapacity = Validation::intValidation(tankCapacity, this->getFullId(), "TankCapacity", 0, 500);

    }


    /**
     * @brief Sets the fuel consumption value
     * 
     * @param fuelConsumption The fuel consumption value
     */
    void Car::setFuelConsumption(double fuelConsumption) {

        // Verification
        this->_fuelConsumption = Validation::doubleValidation(fuelConsumption, this->getFullId(), "FuelConsumption", 0.0, 50.0);

    }


    /**
     * @brief Sets the CO2 emissions value
     * 
     * @param co2Emissions The CO2 emissions value
     */
    void Car::setCo2Emissions(int co2Emissions) {

        // Verification
        this->_co2Emissions = Validation::intValidation(co2Emissions, this->getFullId(), "Co2Emissions", 0, 1000);

    }


    /**
     * @brief Sets the CO2 class value
     * 
     * @param co2Class The CO2 class value
     */
    void Car::setCo2Class(const std::string& co2Class) {

        // Verification + Normalize UTF-8 (NFC) then apply upper case (short code like "A", "B+")
        this->_co2Class = una::cases::to_uppercase_utf8(
            una::norm::to_nfc_utf8(Validation::stringValidation(co2Class, this->getFullId(), "Co2Class", 10)));
    
    }


    /**
     * @brief Sets the commercialisation start date
     * 
     * @param co2Class The commercialisation start date
     */
    void Car::setCommercialisationStart(const std::string& commercialisationStart) {

        // Set value
        _commercialisationStart = Validation::parseDateDMY(commercialisationStart);


        // Log message
        if (_commercialisationStart.has_value()) {
            Logger::trace("{}::set{} value: \"{}\"", this->getFullId(),
                "CommercialisationStart", Validation::formatDate(_commercialisationStart.value()));
        }
        Logger::error("{}::set{} got an empty value", this->getFullId(), "CommercialisationStart");

    }


    /**
     * @brief Sets the commercialisation end date
     * 
     * @param co2Class The commercialisation end date
     */
    void Car::setCommercialisationEnd(const std::string& commercialisationEnd) {

        // Set value
        _commercialisationEnd = Validation::parseDateDMY(commercialisationEnd);


        // Log message
        if (_commercialisationEnd.has_value()) {
            Logger::trace("{}::set{} value: \"{}\"", this->getFullId(),
                "CommercialisationEnd", Validation::formatDate(_commercialisationEnd.value()));
        }
        Logger::error("{}::set{} got an empty value", this->getFullId(), "CommercialisationEnd");

    }


    /**
     * Sets the still in sale value
     * 
     * @param stillInSale The still in sale value
     */
    void Car::setStillInSale(bool stillInSale) {
        this->_stillInSale = stillInSale;
    }





    // =========================================================================
    // Utilities
    // =========================================================================


    /**
     * Checks if the car data has been filled (no default sentinel values)
     *
     * @return true if the car data is complete, false otherwise
     */
    bool Car::isComplete() const {
        return true;
    }


    /**
     * Checks if the car data has been properly filled (no error sentinel values)
     *
     * @return true if the car data is valid, false otherwise
     */
    bool Car::isValid() const {
        return true;
    }


    /**
     * Returns a string representation of the car (for debugging purposes)
     */
    std::string Car::toString() const {
        std::ostringstream oss;
        oss << "[Car] ";
        return oss.str();
    }




    // =========================================================================
    // Stream operator overload
    // =========================================================================
    std::ostream& operator<<(std::ostream& os, const Car& car) {

        os << std::fixed << std::setprecision(2);

        os << car.getFullId() << "\n";
/**
        // General
        os << "- General\n";
        os << "    -> " << std::left << std::setw(16) << "Brand"          << ": \""     << car.getBrand()           << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Model"          << ": \""     << car.getModel()           << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Generation"     << ": \""     << car.getGeneration()      << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Engine"         << ": \""     << car.getEngine()          << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Trim"           << ": \""     << car.getTrim()            << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Price"          << ": "       << car.getPrice()           << " €\n";

        // Dimensions
        os << "- Dimensions\n";
        os << "    -> " << std::left << std::setw(16) << "Height"         << ": "       << car.getHeight()          << " m\n";
        os << "    -> " << std::left << std::setw(16) << "Length"         << ": "       << car.getLength()          << " m\n";
        os << "    -> " << std::left << std::setw(16) << "Width"          << ": "       << car.getWidth()           << " m\n";
        os << "    -> " << std::left << std::setw(16) << "TrunkVolume"    << ": "       << car.getTrunkVolume()     << " L\n";
        os << "    -> " << std::left << std::setw(16) << "Weight"         << ": "       << car.getWeight()          << " kg\n";
        os << "    -> " << std::left << std::setw(16) << "SeatCount"      << ": "       << car.getSeatCount()       << "\n";

        // Transmission
        os << "- Transmission\n";
        os << "    -> " << std::left << std::setw(16) << "GearboxType"    << ": \""     << car.getGearboxType()     << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "GearCount"      << ": "       << car.getGearCount()       << "\n";

        // Power
        os << "- Power\n";
        os << "    -> " << std::left << std::setw(16) << "FuelType"       << ": \""     << car.getFuelType()        << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "HorsePower"     << ": "       << car.getHorsePower()      << " DIN hp\n";
        os << "    -> " << std::left << std::setw(16) << "TaxHorsePower"  << ": "       << car.getTaxHorsePower()   << " fiscal hp\n";

        // Consumption
        os << "- Consumption\n";
        os << "    -> " << std::left << std::setw(16) << "TankCapacity"    << ": "      << car.getTankCapacity()    << " L\n";
        os << "    -> " << std::left << std::setw(16) << "FuelConsumption" << ": "      << car.getFuelConsumption() << " L/100km\n";
        os << "    -> " << std::left << std::setw(16) << "Co2Emissions"    << ": "      << car.getCo2Emissions()    << " g/km\n";
        os << "    -> " << std::left << std::setw(16) << "Co2Class"        << ": \""    << car.getCo2Class()        << "\"\n";

        // Commercialisation
        os << "- Commercialisation\n";
        //os << "    -> " << std::left << std::setw(16) << "Start"           << ": \""    << car.getCommercialisationStart()  << "\"\n";
        //os << "    -> " << std::left << std::setw(16) << "End"             << ": \""    << car.getCommercialisationEnd()    << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "Still in Sale"   << ": "      << car.isStillInSale()      << "\n";
*/

        return os;
    }

} // namespace CarScraper