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
#include <nlohmann/json.hpp>
#include <stduuid/uuid.h>
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <fstream>
#include <sstream>
#include <filesystem>


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
        _weight             = DEFAULT_INT;

        // Liveability
        _trunkVolume        = DEFAULT_INT;
        _doorCount          = DEFAULT_INT;
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

        // Technical Data
        _dataSource         = CarScraper::DataSource::NA;
        _sourceFile         = NONE_STR;
        _duplicatedSource   = false;

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
        if (_brand != CarScraper::DEFAULT_STR &&
            _brand != CarScraper::NONE_STR &&
            _brand != CarScraper::ERROR_STR) {
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
        if (_model != CarScraper::DEFAULT_STR &&
            _model != CarScraper::NONE_STR &&
            _model != CarScraper::ERROR_STR) {
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
        if (_generation != CarScraper::DEFAULT_STR &&
            _generation != CarScraper::NONE_STR &&
            _generation != CarScraper::ERROR_STR) {
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
        if (_phase != CarScraper::DEFAULT_STR &&
            _phase != CarScraper::NONE_STR &&
            _phase != CarScraper::ERROR_STR) {
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
        if (_engine != CarScraper::DEFAULT_STR &&
            _engine != CarScraper::NONE_STR &&
            _engine != CarScraper::ERROR_STR) {
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
        if (_trim != CarScraper::DEFAULT_STR &&
            _trim != CarScraper::NONE_STR &&
            _trim != CarScraper::ERROR_STR) {
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
     * @brief Sets the weight value
     * @param weight The weight value
     * @note The weight value must be between 0 and 5000 kg (inclusive)
     */
    void Car::setWeight(int weight) {

        // Verification
        this->_weight = Validation::intValidation(weight, this->getFullId(), "Weight", 0, 5000);

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
     * @brief Sets the door count value
     * @param doorCount The door count value
     * @note The door count value must be between 0 and 10 (inclusive)
     */
    void Car::setDoorCount(int doorCount) {

        // Verification
        this->_doorCount = Validation::intValidation(doorCount, this->getFullId(), "DoorCount", 0, 10);

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
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "GearboxType", gearboxType);
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
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "FuelType", fuelType);
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
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "Co2Class", co2Class);
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "Co2Class", CarScraper::co2ClassToString(_co2Class));
        }
    
    }


    /**
     * @brief Sets the commercialisation start date
     * @param commercialisationStart The commercialisation start date
     */
    void Car::setCommercialisationStart(const std::optional<std::chrono::year_month_day>& commercialisationStart) {

        // Set value
        _commercialisationStart = commercialisationStart;


        // Log message
        if (_commercialisationStart.has_value()) {
            Logger::trace("[{}].set{} value: \"{}\"", this->getFullId(),
                "CommercialisationStart", Validation::formatDate(_commercialisationStart.value()));
        } else {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CommercialisationStart");
        }

    }


    /**
     * @brief Sets the commercialisation start date
     * @param commercialisationStart The commercialisation start date in string format
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
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "CommercialisationStart", commercialisationStart);
        }

    }


    /**
     * @brief Sets the commercialisation end date
     * @param commercialisationEnd The commercialisation end date
     */
    void Car::setCommercialisationEnd(const std::optional<std::chrono::year_month_day>& commercialisationEnd) {

        // Set value
        _commercialisationEnd = commercialisationEnd;


        // Log message
        if (_commercialisationEnd.has_value()) {
            Logger::trace("[{}].set{} value: \"{}\"", this->getFullId(),
                "CommercialisationEnd", Validation::formatDate(_commercialisationEnd.value()));
        } else {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "CommercialisationEnd");
        }

    }


    /**
     * @brief Sets the commercialisation end date
     * @param commercialisationEnd The commercialisation end date in string format
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
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "CommercialisationEnd", commercialisationEnd );
        }

    }


    /**
     * @brief Sets the still in sale value
     * @param stillInSale The still in sale value
     */
    void Car::setStillInSale(bool stillInSale) {
        this->_stillInSale = stillInSale;
    }


    /**
     * @brief Sets the data source using an enum value
     * @param dataSource The data source
     */
    void Car::setDataSource(const CarScraper::DataSource dataSource) {

       // Set value
        _dataSource = dataSource;


        // Log message
        if (_dataSource == CarScraper::DataSource::NA) {
            Logger::error("[{}].set{} got an invalid value", this->getFullId(), "DataSource");
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "DataSource", CarScraper::dataSourceToString(_dataSource));
        }

    }


    /**
     * @brief Sets the data source using a string value
     * @details The string value will be converted to the corresponding enum value
     * using the CarScraper::dataSourceFromString() method. If the string value is invalid,
     * the data source will be set to NA and an error message will be logged.
     * @param dataSource The data source
     */
    void Car::setDataSource(const std::string& dataSource) {

       // Set value
        _dataSource = CarScraper::dataSourceFromString(dataSource);


        // Log message
        if (_dataSource == CarScraper::DataSource::NA) {
            Logger::error("[{}].set{} got an invalid value \"{}\"", this->getFullId(), "DataSource", dataSource);
        } else {
            Logger::trace("[{}].set{} value: {}", this->getFullId(),
                "DataSource", CarScraper::dataSourceToString(_dataSource));
        }

    }


    /**
     * @brief Sets the source file of the car
     * @param sourceFile The source file of the car
     * @note The source file path is checked before setting the value. If the file does not
     * exist, an error message is logged and the value is not set.
     */
    void Car::setSourceFile(const std::string& sourceFile) {

        // By pass check in case of sentiennel values
        if (sourceFile == CarScraper::DEFAULT_STR ||
            sourceFile == CarScraper::NONE_STR ||
            sourceFile == CarScraper::ERROR_STR) {
            _sourceFile = sourceFile;
            Logger::trace("[{}].set{} value: {}", this->getFullId(), "SourceFile", _sourceFile);
            return;
        }


        // Checking if the file exists
        namespace fs = std::filesystem;
        if (!fs::exists(sourceFile)) {
            Logger::error("[{}].setSourceFile() : file \"{}\" does not exist", this->getFullId(), sourceFile);
            _sourceFile = ERROR_STR;
        } else {
            _sourceFile = sourceFile;
            Logger::trace("[{}].set{} value: {}", this->getFullId(), "SourceFile", _sourceFile);
        }

    }


    /**
     * @brief Sets the from duplicated source value
     * @param duplicatedSource The from duplicated source value
     */
    void Car::setFromDuplicatedSource(bool duplicatedSource) {
        this->_duplicatedSource = duplicatedSource;
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
        if (_weight     == DEFAULT_INT)     return false;


        // Liveability
        if (_trunkVolume== DEFAULT_INT)     return false;
        if (_doorCount  == DEFAULT_INT)     return false;
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


        // Technical Data
        if (_dataSource     == DataSource::NA)  return false;


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
        if (_weight     == ERROR_INT)       return false;


        // Liveability
        if (_trunkVolume== ERROR_INT)       return false;
        if (_doorCount  == ERROR_INT)       return false;
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
        oss << "    -> " << std::left << std::setw(17) << "Weight"         << ": " << _weight      << " kg\n";


        // -------------------------------------------------------------------------
        // Liveability
        // -------------------------------------------------------------------------
        oss << "- Liveability\n";
        oss << "    -> " << std::left << std::setw(17) << "TrunkVolume"    << ": " << _trunkVolume << " L\n";
        oss << "    -> " << std::left << std::setw(17) << "DoorCount"      << ": " << _doorCount   << "\n";
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
        if (!_stillInSale) {
            oss << "    -> " << std::left << std::setw(17) << "End"
                << ": \""
                << (_commercialisationEnd.has_value()
                        ? Validation::formatDate(_commercialisationEnd.value())
                        : "N/A")
                << "\"\n";
        }
        oss << "    -> " << std::left << std::setw(17) << "Still in Sale"  << ": " << (_stillInSale ? "Yes" : "No") << "\n";


        // -------------------------------------------------------------------------
        // Technical Data
        // -------------------------------------------------------------------------
        oss << "- Technical Data\n";
        oss << "    -> " << std::left << std::setw(17) << "Data source"     << ": \"" << dataSourceToString(_dataSource)  << "\"\n";
        oss << "    -> " << std::left << std::setw(17) << "Source file"     << ": \"" << _sourceFile  << "\"\n";


        return oss.str();
    }


    /**
     * @brief Saves the car data to a file in JSON format
     * @return The path of the saved file
     * @note This method will create a folder BRAND in the CAR_DIR directory. Then a model folder, a generation folder (named MODEL-GENERATION) and finnaly a year folder.
     * The file will be named with the car full name : BRAND-MODEL-GENERATION-PHASE-ENGINE-TRIM (+ a number in case)
     */
    int Car::save() const {

        // Debug
        Logger::debug("[{}].save()", this->getFullId());


        // ----- Ensuring the directory exists ----------------------------------------------------
        // Checking the commercialisation start date
        if (!_commercialisationStart) {
            Logger::error("[{}].save() : no commercialisation start date", this->getFullId());
            return ERROR_CODE;
        }


        // Getting the year
        std::string yearStr = _commercialisationStart ? std::to_string(static_cast<int>(_commercialisationStart->year())) : "";
        if (yearStr.empty()) {
            Logger::error("[{}].save() : unable to convert commercialisation start date into string", this->getFullId());
            return ERROR_CODE;
        }


        // Creating the directory path
        namespace fs = std::filesystem;
        fs::path directory;
        if (_generation == NONE_STR || _generation == DEFAULT_STR || _generation == ERROR_STR) {
            directory = CAR_DIR + _brand + "/" + _model + "/" + yearStr;
        } else {
            directory = CAR_DIR + _brand + "/" + _model + "/" + _model + "-" + _generation + "/" + yearStr;
        }


        // Checking if the directory exist or create it
        if (!fs::exists(directory)) {
            if (fs::create_directories(directory)) {
                Logger::info("[{}].save() : {} directory created", this->getFullId(), directory.string());
            } else {
                Logger::error("[{}].save() : unable to creat {} directory", this->getFullId(), directory.string());
            }
        }



        // ----- Creating the JSON Object ---------------------------------------------------------
        using json = nlohmann::json;
        json carJSON;

        // General
        carJSON["brand"]                = _brand;
        carJSON["model"]                = _model;
        carJSON["generation"]           = _generation;
        carJSON["phase"]                = _phase;
        carJSON["engine"]               = _engine;
        carJSON["trim"]                 = _trim;
        carJSON["price"]                = _price;

        // Dimensions
        carJSON["height"]               = _height;
        carJSON["length"]               = _length;
        carJSON["width"]                = _width;
        carJSON["weight"]               = _weight;

        // Liveability
        carJSON["trunkVolume"]          = _trunkVolume;
        carJSON["doorCount"]            = _doorCount;
        carJSON["seatCount"]            = _seatCount;

        // Transmission
        carJSON["gearboxType"]          = CarScraper::gearBoxTypeToString(_gearboxType);
        carJSON["gearCount"]            = _gearCount;

        // Power
        carJSON["fuelType"]             = CarScraper::fuelTypeToString(_fuelType);
        carJSON["horsePower"]           = _horsePower;
        carJSON["taxHorsePower"]        = _taxHorsePower;

        // Consumption
        carJSON["tankCapacity"]         = _tankCapacity;
        carJSON["fuelConsumption"]      = _fuelConsumption;
        carJSON["co2Emissions"]         = _co2Emissions;
        carJSON["co2Class"]             = CarScraper::co2ClassToString(_co2Class);

        // Commercialisation
        carJSON["commercialisationStart"]   = _commercialisationStart.has_value()   ? CarScraper::Validation::formatDate(*_commercialisationStart)  : nullptr;
        carJSON["commercialisationEnd"]     = _commercialisationEnd.has_value()     ? CarScraper::Validation::formatDate(*_commercialisationEnd)    : nullptr;
        carJSON["stillInSale"]              = _stillInSale;

        // Technical Data
        carJSON["dataSource"]           = CarScraper::dataSourceToString(_dataSource);



        // ----- Saving into a JSON File ----------------------------------------------------------
        fs::path filePath = directory.string() + "/" + _brand + "-" + _model;
        if (_generation != NONE_STR && _generation != DEFAULT_STR && _generation != ERROR_STR) {
            filePath += "-" + _generation;
        }
        if (_phase != NONE_STR && _phase != DEFAULT_STR && _phase != ERROR_STR) {
            filePath += "-" + _phase;
        }
        filePath += "-" + _engine + "-" + _trim;


        // Checking if the file already exists and adding a number to the file name if it does
        if (fs::exists(filePath.string() + ".json")) {
            Logger::warn("[{}].save() : file already exists, adding a number to the file name", this->getFullId());
            int fileIndex = 1;
            while (fs::exists(filePath.string() + "-" + std::to_string(fileIndex) + ".json")) {
                fileIndex++;
            }
            filePath += "-" + std::to_string(fileIndex);
        }
        filePath += ".json";


        // Writing the JSON object to the file
        std::ofstream file(filePath);
        file << carJSON.dump(4);


        // Debug
        Logger::debug("[{}].save() : file written {}", this->getFullId(), filePath.string());
        return SUCCESS_CODE;

    }


    /**
     * @brief Loads the car data from a file in JSON format
     * @param filePath The path of the file to load
     * @return SUCCESS_CODE or ERROR_CODE wheter the load is successful or not
     */
    int Car::load(const std::string& filePath) {

        // Debug
        Logger::debug("[{}].load({})", this->getFullId(), filePath);


        // File opening and error handling
        std::ifstream file(filePath);
        if (!file.is_open()) {
            Logger::error("[{}].load({}) unable to open file", this->getFullId(), filePath);
            return ERROR_CODE;
        }


        // Reading the JSON object from the file
        nlohmann::json carJSON;
        file >> carJSON;
        file.close();



        // --- Parsing the JSON object and setting the car data -----------------------------------

        // General
        this->setBrand(         carJSON.value("brand",      NONE_STR));
        this->setModel(         carJSON.value("model",      NONE_STR));
        this->setGeneration(    carJSON.value("generation", NONE_STR));
        this->setPhase(         carJSON.value("phase",      NONE_STR));
        this->setEngine(        carJSON.value("engine",     NONE_STR));
        this->setTrim(          carJSON.value("trim",       NONE_STR));
        this->setPrice(         carJSON.value("price",      0));

        // Dimensions
        this->setHeight(        carJSON.value("height",     DEFAULT_DOUBLE));
        this->setLength(        carJSON.value("length",     DEFAULT_DOUBLE));
        this->setWidth(         carJSON.value("width",      DEFAULT_DOUBLE));
        this->setWeight(        carJSON.value("weight",     DEFAULT_INT));

        // Liveability
        this->setTrunkVolume(   carJSON.value("trunkVolume",    DEFAULT_INT));
        this->setDoorCount(     carJSON.value("doorCount",      DEFAULT_INT));
        this->setSeatCount(     carJSON.value("seatCount",      DEFAULT_INT));

        // Transmission
        this->setGearboxType(   carJSON.value("gearboxType",    NONE_STR));
        this->setGearCount(     carJSON.value("gearCount",      DEFAULT_INT));

        // Power
        this->setFuelType(      carJSON.value("fuelType",       NONE_STR));
        this->setHorsePower(    carJSON.value("horsePower",     DEFAULT_INT));
        this->setTaxHorsePower( carJSON.value("taxHorsePower",  DEFAULT_INT));

        // Consumption
        this->setTankCapacity(      carJSON.value("tankCapacity",       DEFAULT_INT));
        this->setFuelConsumption(   carJSON.value("fuelConsumption",    DEFAULT_DOUBLE));
        this->setCo2Emissions(      carJSON.value("co2Emissions",       DEFAULT_INT));
        this->setCo2Class(          carJSON.value("co2Class",           NONE_STR));

        // Commercialisation
        this->setStillInSale(           carJSON.value("stillInSale", false));
        this->setCommercialisationStart(carJSON.value("commercialisationStart", NONE_STR));
        this->setCommercialisationEnd(  carJSON.value("commercialisationEnd", NONE_STR));

        // Technical Data
        _dataSource = CarScraper::DataSource::FILE;
        _sourceFile = filePath;
        

        // Debug
        Logger::debug("[{}].load({}) successful {}", this->getFullId(), filePath, carJSON.value("fuelType", NONE_STR));
        return SUCCESS_CODE;

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