/**
 * @file Car.cpp
 * 
 * @brief This file implements the Car class, which represents a car entity.
 *
 * @author Adrien GRAS
 * @date 2026-05-26
 *
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


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor
     * @details Initializes all the car data with sentinel default values
     * @note The car entity uses the CAR prefix for its full ID (ex: "CAR-123e45...")
     */
    Car::Car() : Entity("CAR") {

        // General
        _brand              = DEFAULT_STR;
        _model              = DEFAULT_STR;
        _generation         = DEFAULT_STR;
        _phase              = DEFAULT_STR;
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
        _co2Class           = CarScraper::Co2Class::NA;

        // Commercialisation
        _commercialisationStart  = std::nullopt;
        _commercialisationEnd    = std::nullopt;
        _stillInSale             = false;

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the brand value
     * @details The brand value is normalized (converted to uppercase)
     * @param brand The brand value
     * @note The maximum length accepted is 100 characters
     */
    void Car::setBrand(const std::string& brand) {

        // Validation
        _brand = Validation::stringValidation(brand, this->getFullId(), "Brand", 100);

        // Normalize UTF-8 (NFC) then apply upper case if no error
        if (_brand != CarScraper::ERROR_STR) {
            this->_brand = una::cases::to_uppercase_utf8(una::norm::to_nfc_utf8(_brand));
        }

    }


    /**
     * @brief Sets the model value
     * @details The model value is normalized (converted to title case)
     * @param model The model value
     * @note The maximum length accepted is 200 characters
     */
    void Car::setModel(const std::string& model) {
        
        // Validation
        _model = Validation::stringValidation(model, this->getFullId(), "Model", 200);

        // Normalize UTF-8 (NFC) then apply title case if no error
        if (_model != CarScraper::ERROR_STR) {
            this->_model = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(_model));
        }

    }


    /**
     * @brief Sets the generation value
     * @details The generation value is normalized
     * @param generation The generation value
     * @note The maximum length accepted is 50 characters
     */
    void Car::setGeneration(const std::string& generation) {
        
        // Validation
        _generation = Validation::stringValidation(generation, this->getFullId(), "Generation", 50);

        // Normalize UTF-8 (NFC) only — generation is a technical identifier
        if (_generation != CarScraper::ERROR_STR) {
            this->_generation = una::norm::to_nfc_utf8(_generation);
        }

    }


    /**
     * @brief Sets the phase value
     * @details The phase value is normalized
     * @param phase The phase value
     * @note The maximum length accepted is 50 characters
     */
    void Car::setPhase(const std::string& phase) {
        
        // Validation
        _phase = Validation::stringValidation(phase, this->getFullId(), "Phase", 50);

        // Normalize UTF-8 (NFC) only — phase is a technical identifier
        if (_phase != CarScraper::ERROR_STR) {
            this->_phase = una::norm::to_nfc_utf8(_phase);
        }

    }


    /**
     * @brief Sets the engine value
     * @details The engine value is normalized
     * @param engine The engine value
     * @note The maximum length accepted is 200 characters
     */
    void Car::setEngine(const std::string& engine) {

        // Validation
        _engine = Validation::stringValidation(engine, this->getFullId(), "Engine", 200);

        // Normalize UTF-8 (NFC) only — engine is a technical identifier
        if (_engine != CarScraper::ERROR_STR) {
            this->_engine = una::norm::to_nfc_utf8(_engine);
        }

    }


    /**
     * @brief Sets the trim value
     * @details The trim value is normalized (converted to title case)
     * @param trim The trim value
     * @note The maximum length accepted is 200 characters
     */
    void Car::setTrim(const std::string& trim) {

        // Validation
        _trim = Validation::stringValidation(trim, this->getFullId(), "Trim", 200);

        // Normalize UTF-8 (NFC) then apply title case if no error
        if (_trim != CarScraper::ERROR_STR) {
            this->_trim = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(_trim));
        }
    
    }


    /**
     * @brief Sets the price value
     * @param price The price value
     * @note The price value must be between 0 and 10 million (inclusive)
     */
    void Car::setPrice(int price) {

        // Verification
        this->_price = Validation::intValidation(price, this->getFullId(), "Price", 0, 10000000);

    }


    /**
     * @brief Sets the height value
     * @param height The height value
     * @note The height value must be between 0 and 10 meters (inclusive)
     */
    void Car::setHeight(double height) {

        // Verification
        this->_height = Validation::doubleValidation(height, this->getFullId(), "Height", 0.0, 10.0);

    }


    /**
     * @brief Sets the length value
     * @param length The length value
     * @note The length value must be between 0 and 10 meters (inclusive)
     */
    void Car::setLength(double length) {

        // Verification
        this->_length = Validation::doubleValidation(length, this->getFullId(), "Length", 0.0, 10.0);

    }


    /**
     * @brief Sets the width value
     * @param width The width value
     * @note The width value must be between 0 and 10 meters (inclusive)
     */
    void Car::setWidth(double width) {

        // Verification
        this->_width = Validation::doubleValidation(width, this->getFullId(), "Width", 0.0, 10.0);

    }


    /**
     * @brief Sets the trunk volume value
     * @param trunkVolume The trunk volume value
     * @note The trunk volume value must be between 0 and 1000 liters (inclusive)
     */
    void Car::setTrunkVolume(int trunkVolume) {

        // Verification
        this->_trunkVolume = Validation::intValidation(trunkVolume, this->getFullId(), "TrunkVolume", 0, 1000);

    }


    /**
     * @brief Sets the weight value
     * @param weight The weight value
     * @note The weight value must be between 0 and 5000 kg (inclusive)
     */
    void Car::setWeight(int weight) {

        // Verification
        this->_weight = Validation::intValidation(weight, this->getFullId(), "Weight", 0, 5000);

    }


    /**
     * @brief Sets the seat count value
     * @param seatCount The seat count value
     * @note The seat count value must be between 0 and 10 (inclusive)
     */
    void Car::setSeatCount(int seatCount) {

        // Verification
        this->_seatCount = Validation::intValidation(seatCount, this->getFullId(), "SeatCount", 0, 10);

    }


    /**
     * @brief Sets the gear box type using an enum value
     * @param gearboxType The gear box type
     */
    void Car::setGearboxType(const CarScraper::GearboxType gearboxType) {

       // Set value
        _gearboxType = gearboxType;


        // Log message
        if (_gearboxType == CarScraper::GearboxType::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "GearboxType");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "GearboxType", CarScraper::gearBoxTypeToString(_gearboxType));
        }

    }


    /**
     * @brief Sets the gear box type using a string value
     * @details The string value will be converted to the corresponding enum value
     * using the CarScraper::gearBoxTypeFromString() method. If the string value is invalid,
     * the gear boxtype will be set to NA and an error message will be logged.
     * @param gearboxType The gear box type
     */
    void Car::setGearboxType(const std::string& gearboxType) {

       // Set value
        _gearboxType = CarScraper::gearBoxTypeFromString(gearboxType);


        // Log message
        if (_gearboxType == CarScraper::GearboxType::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "GearboxType");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "GearboxType", CarScraper::gearBoxTypeToString(_gearboxType));
        }

    }


    /**
     * @brief Sets the gear count value
     * @param gearCount The gear count value
     * @note The gear count value must be between 0 and 20 (inclusive)
     */
    void Car::setGearCount(int gearCount) {

        // Verification
        this->_gearCount = Validation::intValidation(gearCount, this->getFullId(), "GearCount", 0, 20);

    }


    /**
     * @brief Sets the fuel type using an enum value
     * @param fuelType The fuel type
     */
    void Car::setFuelType(const CarScraper::FuelType fuelType) {

        // Set value
        _fuelType = fuelType;


        // Log message
        if (_fuelType == CarScraper::FuelType::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "FuelType");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "FuelType", CarScraper::fuelTypeToString(_fuelType));
        }

    }


    /**
     * @brief Sets the fuel type using a string value
     * @details The string value will be converted to the corresponding enum value
     * using the CarScraper::fuelTypeFromString() method. If the string value is invalid,
     * the fuel type will be set to NA and an error message will be logged.
     * @param fuelType The fuel type
     */
    void Car::setFuelType(const std::string& fuelType) {

        // Set value
        _fuelType = CarScraper::fuelTypeFromString(fuelType);


        // Log message
        if (_fuelType == CarScraper::FuelType::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "FuelType");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "FuelType", CarScraper::fuelTypeToString(_fuelType));
        }

    }


    /**
     * @brief Sets the horse power value
     * @param horsePower The horse power value
     * @note The horse power value must be between 0 and 3000 (inclusive)
     */
    void Car::setHorsePower(int horsePower) {

        // Verification
        this->_horsePower = Validation::intValidation(horsePower, this->getFullId(), "HorsePower", 0, 3000);

    }


    /**
     * @brief Sets the tax horse power value
     * @param taxHorsePower The tax horse power value
     * @note The tax horse power value must be between 0 and 200 (inclusive)
     */
    void Car::setTaxHorsePower(int taxHorsePower) {

        // Verification
        this->_taxHorsePower = Validation::intValidation(taxHorsePower, this->getFullId(), "TaxHorsePower", 0, 200);

    }


    /**
     * @brief Sets the tank capacity value
     * @param tankCapacity The tank capacity value
     * @note The tank capacity value must be between 0 and 500 liters (inclusive)
     */
    void Car::setTankCapacity(int tankCapacity) {

        // Verification
        this->_tankCapacity = Validation::intValidation(tankCapacity, this->getFullId(), "TankCapacity", 0, 500);

    }


    /**
     * @brief Sets the fuel consumption value
     * @param fuelConsumption The fuel consumption value
     * @note The fuel consumption value must be between 0 and 50 L/100 km (inclusive)
     */
    void Car::setFuelConsumption(double fuelConsumption) {

        // Verification
        this->_fuelConsumption = Validation::doubleValidation(fuelConsumption, this->getFullId(), "FuelConsumption", 0.0, 50.0);

    }


    /**
     * @brief Sets the CO2 emissions value
     * @param co2Emissions The CO2 emissions value
     * @note The CO2 emissions value must be between 0 and 1000 g/km (inclusive)
     */
    void Car::setCo2Emissions(int co2Emissions) {

        // Verification
        this->_co2Emissions = Validation::intValidation(co2Emissions, this->getFullId(), "Co2Emissions", 0, 1000);

    }


    /**
     * @brief Sets the CO2 class value using an enum value
     * @param co2Class The CO2 class value
     */
    void Car::setCo2Class(const CarScraper::Co2Class co2Class) {

        // Set value
        _co2Class = co2Class;


        // Log message
        if (_co2Class == CarScraper::Co2Class::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "Co2Class");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "Co2Class", CarScraper::co2ClassToString(_co2Class));
        }
    
    }


    /**
     * @brief Sets the CO2 class value using a string value
     * @details The string value will be converted to the corresponding enum value
     * using the CarScraper::co2ClassFromString() method. If the string value is invalid,
     * the CO2 class will be set to NA and an error message will be logged.
     * @param co2Class The CO2 class value
     */
    void Car::setCo2Class(const std::string& co2Class) {

        // Set value
        _co2Class = CarScraper::co2ClassFromString(co2Class);


        // Log message
        if (_co2Class == CarScraper::Co2Class::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "Co2Class");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "Co2Class", CarScraper::co2ClassToString(_co2Class));
        }
    
    }


    /**
     * @brief Sets the commercialisation start date
     * @param commercialisationStart The commercialisation start date
     * @note The commercialisation start date must be in the format "DD/MM/YYYY" and must be a valid date
     */
    void Car::setCommercialisationStart(const std::string& commercialisationStart) {

        // Set value
        _commercialisationStart = Validation::parseDateDMY(commercialisationStart);


        // Log message
        if (_commercialisationStart.has_value()) {
            Logger::trace("[{}].set{} value: \"{}\"", this->getFullId(),
                "CommercialisationStart", Validation::formatDate(_commercialisationStart.value()));
        } else {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CommercialisationStart");
        }

    }


    /**
     * @brief Sets the commercialisation end date
     * @param commercialisationEnd The commercialisation end date
     * @note The commercialisation end date must be in the format "DD/MM/YYYY" and must be a valid date
     */
    void Car::setCommercialisationEnd(const std::string& commercialisationEnd) {

        // Set value
        _commercialisationEnd = Validation::parseDateDMY(commercialisationEnd);


        // Log message
        if (_commercialisationEnd.has_value()) {
            Logger::trace("[{}].set{} value: \"{}\"", this->getFullId(),
                "CommercialisationEnd", Validation::formatDate(_commercialisationEnd.value()));
        } else {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CommercialisationEnd");
        }

    }


    /**
     * @brief Sets the still in sale value
     * @param stillInSale The still in sale value
     */
    void Car::setStillInSale(bool stillInSale) {
        this->_stillInSale = stillInSale;
    }





    // =========================================================================
    // Utilities
    // =========================================================================


    /**
     * @brief Checks if the car data has been filled (no default sentinel values)
     * @return true if the car data is complete and valid, false otherwise
     */
    bool Car::isComplete() const {

        // General
        if (_brand      == DEFAULT_STR)     return false;
        if (_model      == DEFAULT_STR)     return false;
        if (_generation == DEFAULT_STR)     return false;
        if (_phase      == DEFAULT_STR)     return false;
        if (_engine     == DEFAULT_STR)     return false;
        if (_trim       == DEFAULT_STR)     return false;
        if (_price      == DEFAULT_INT)     return false;


        // Dimensions
        if (_height     == DEFAULT_DOUBLE)  return false;
        if (_length     == DEFAULT_DOUBLE)  return false;
        if (_width      == DEFAULT_DOUBLE)  return false;
        if (_trunkVolume== DEFAULT_INT)     return false;
        if (_weight     == DEFAULT_INT)     return false;
        if (_seatCount  == DEFAULT_INT)     return false;


        // Transmission
        if (_gearboxType == GearboxType::NA) return false;
        if (_gearCount   == DEFAULT_INT)     return false;


        // Power
        if (_fuelType      == FuelType::NA) return false;
        if (_horsePower    == DEFAULT_INT)  return false;
        if (_taxHorsePower == DEFAULT_INT)  return false;


        // Consumption
        if (_tankCapacity    == DEFAULT_INT)    return false;
        if (_fuelConsumption == DEFAULT_DOUBLE) return false;
        if (_co2Emissions    == DEFAULT_INT)    return false;
        if (_co2Class        == Co2Class::NA)   return false;


        // Commercialisation
        if (!_commercialisationStart.has_value()) return false;


        // Car complete
        return this->isValid();

    }


    /**
     * @brief Checks if the car data has been properly filled (no error sentinel values)
     * @return true if the car data is valid, false otherwise
     */
    bool Car::isValid() const {

        // General
        if (_brand      == ERROR_STR)       return false;
        if (_model      == ERROR_STR)       return false;
        if (_generation == ERROR_STR)       return false;
        if (_phase      == ERROR_STR)       return false;
        if (_engine     == ERROR_STR)       return false;
        if (_trim       == ERROR_STR)       return false;
        if (_price      == ERROR_INT)       return false;


        // Dimensions
        if (_height     == ERROR_DOUBLE)    return false;
        if (_length     == ERROR_DOUBLE)    return false;
        if (_width      == ERROR_DOUBLE)    return false;
        if (_trunkVolume== ERROR_INT)       return false;
        if (_weight     == ERROR_INT)       return false;
        if (_seatCount  == ERROR_INT)       return false;


        // Transmission
        if (_gearCount  == ERROR_INT)       return false;


        // Power
        if (_horsePower    == ERROR_INT)    return false;
        if (_taxHorsePower == ERROR_INT)    return false;


        // Consumption
        if (_tankCapacity    == ERROR_INT)      return false;
        if (_fuelConsumption == ERROR_DOUBLE)   return false;
        if (_co2Emissions    == ERROR_INT)      return false;


        // Car valid
        return true;

    }


    /**
     * @brief Returns a string representation of the car (for debugging purposes)
     * @return the string representation of the car
     */
    std::string Car::toString() const {

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        // Header
        oss << getFullId() << "\n";


        // -------------------------------------------------------------------------
        // General
        // -------------------------------------------------------------------------
        oss << "- General\n";
        oss << "    -> " << std::left << std::setw(17) << "Brand"          << ": \"" << _brand       << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Model"          << ": \"" << _model       << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Generation"     << ": \"" << _generation  << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Phase"          << ": \"" << _phase       << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Engine"         << ": \"" << _engine      << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Trim"           << ": \"" << _trim        << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Price"          << ": "   << _price       << " €\n";


        // -------------------------------------------------------------------------
        // Dimensions
        // -------------------------------------------------------------------------
        oss << "- Dimensions\n";
        oss << "    -> " << std::left << std::setw(17) << "Height"         << ": " << _height      << " m\n";
        oss << "    -> " << std::left << std::setw(17) << "Length"         << ": " << _length      << " m\n";
        oss << "    -> " << std::left << std::setw(17) << "Width"          << ": " << _width       << " m\n";
        oss << "    -> " << std::left << std::setw(17) << "TrunkVolume"    << ": " << _trunkVolume << " L\n";
        oss << "    -> " << std::left << std::setw(17) << "Weight"         << ": " << _weight      << " kg\n";
        oss << "    -> " << std::left << std::setw(17) << "SeatCount"      << ": " << _seatCount   << "\n";


        // -------------------------------------------------------------------------
        // Transmission
        // -------------------------------------------------------------------------
        oss << "- Transmission\n";
        oss << "    -> " << std::left << std::setw(17) << "GearboxType"    << ": \"" << gearBoxTypeToString(_gearboxType) << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "GearCount"      << ": "   << _gearCount  << "\n";


        // -------------------------------------------------------------------------
        // Power
        // -------------------------------------------------------------------------
        oss << "- Power\n";
        oss << "    -> " << std::left << std::setw(17) << "FuelType"       << ": \"" << fuelTypeToString(_fuelType)       << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "HorsePower"     << ": "   << _horsePower    << " DIN hp\n";
        oss << "    -> " << std::left << std::setw(17) << "TaxHorsePower"  << ": "   << _taxHorsePower << " fiscal hp\n";


        // -------------------------------------------------------------------------
        // Consumption
        // -------------------------------------------------------------------------
        oss << "- Consumption\n";
        oss << "    -> " << std::left << std::setw(17) << "TankCapacity"    << ": " << _tankCapacity    << " L\n";
        oss << "    -> " << std::left << std::setw(17) << "FuelConsumption" << ": " << _fuelConsumption << " L/100km\n";
        oss << "    -> " << std::left << std::setw(17) << "Co2Emissions"    << ": " << _co2Emissions    << " g/km\n";
        oss << "    -> " << std::left << std::setw(17) << "Co2Class"        << ": \"" << co2ClassToString(_co2Class)      << "\"\n";


        // -------------------------------------------------------------------------
        // Commercialisation
        // -------------------------------------------------------------------------
        oss << "- Commercialisation\n";
        oss << "    -> " << std::left << std::setw(17) << "Start"
            << ": \""
            << (_commercialisationStart.has_value()
                    ? Validation::formatDate(_commercialisationStart.value())
                    : "N/A")
            << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "End"
            << ": \""
            << (_commercialisationEnd.has_value()
                    ? Validation::formatDate(_commercialisationEnd.value())
                    : "still in sale")
            << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Still in Sale"  << ": " << _stillInSale << "\n";


        return oss.str();
    }


    /**
     * @brief Saves the car data to a file in JSON format
     * @param filename The file path (without the filename). If empty, the file will saved at data/car
     * @return The path of the saved file
     * @note The file will be named with the car full ID (ex: "CAR-123e45...json")
     * @todo Implement the save() method
     */
    std::string Car::save(const std::string& filepath) const {
        Logger::info("[{}].save()", this->getFullId());
        return filepath;
    }




    // =========================================================================
    // Operator overload
    // =========================================================================

    /**
     * @brief Operator << overload for the stream compatibility (uses the Car.toString() method)
     * @param os the targeted stream
     * @param car the targeted car
     * @return the stream with the car string representation inserted
     */
    std::ostream& operator<<(std::ostream& os, const Car& car) {
        os << car.toString();
        return os;
    }

} // namespace CarScraper