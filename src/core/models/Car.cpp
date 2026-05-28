// Imports
#include "Car.hpp"
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
    // Default values
    // =========================================================================
    static constexpr int         DEFAULT_INT    = 9999;
    static constexpr double      DEFAULT_DOUBLE = 9999.0;
    static const     std::string DEFAULT_STR    = "#";
    static constexpr int         ERROR_INT      = -1;
    static constexpr double      ERROR_DOUBLE   = -1.0;
    static const     std::string ERROR_STR      = "error";


    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * Default constructor: initializes all the car data with default values
     */
    Car::Car() {

        // UUID generation (random v4)
        std::random_device  rd;
        std::mt19937        engine{rd()};
        uuids::uuid_random_generator gen{engine};
        _uuid               = uuids::to_string(gen());

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
        _gearboxType        = DEFAULT_STR;
        _gearCount          = DEFAULT_INT;

        // Power
        _fuelType           = DEFAULT_STR;
        _horsePower         = DEFAULT_INT;
        _taxHorsePower      = DEFAULT_INT;

        // Consumption
        _tankCapacity       = DEFAULT_INT;
        _fuelConsumption    = DEFAULT_DOUBLE;
        _co2Emissions       = DEFAULT_INT;
        _co2Class           = DEFAULT_STR;

        // Years
        _yearStart          = DEFAULT_STR;
        _yearEnd            = DEFAULT_STR;

        // Debug
        Logger::info("CAR — NEW: \"{}\"", _uuid);

    }


    // Destructor
    Car::~Car() {

        // Debug
        Logger::info("CAR — DELETE: \"{}\"", _uuid);

    }





    // =========================================================================
    // Setters
    // =========================================================================

    // Brand
    void Car::setBrand(const std::string& brand) {

        // Verification
        if (brand.empty()) {
            this->_brand = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) then apply upper case
        this->_brand = una::cases::to_uppercase_utf8(una::norm::to_nfc_utf8(brand));

    }


    // Model
    void Car::setModel(const std::string& model) {

        // Verification
        if (model.empty()) {
            this->_model = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_model = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(model));

    }


    // Generation
    void Car::setGeneration(const std::string& generation) {

        // Verification
        if (generation.empty()) {
            this->_generation = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) only — generation is a technical identifier (ex: "4", "IV")
        this->_generation = una::norm::to_nfc_utf8(generation);

    }


    // Engine
    void Car::setEngine(const std::string& engine) {

        // Verification
        if (engine.empty()) {
            this->_engine = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) only — engine is a technical identifier (ex: "1.5 dCi 90hp")
        this->_engine = una::norm::to_nfc_utf8(engine);

    }


    // Trim
    void Car::setTrim(const std::string& trim) {

        // Verification
        if (trim.empty()) {
            this->_trim = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_trim = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(trim));
    
    }


    // Price
    void Car::setPrice(int price) {

        // Verification
        if (price < 0) {
            this->_price = ERROR_INT;
            return;
        }
        this->_price = price;

    }


    // Height
    void Car::setHeight(double height) {

        // Verification
        if (height < 0) {
            this->_height = ERROR_DOUBLE;
            return;
        }
        this->_height = height;

    }


    // Length
    void Car::setLength(double length) {

        // Verification
        if (length < 0) {
            this->_length = ERROR_DOUBLE;
            return;
        }
        this->_length = length;

    }


    // Width
    void Car::setWidth(double width) {

        // Verification
        if (width < 0) {
            this->_width = ERROR_DOUBLE;
            return;
        }
        this->_width = width;

    }


    // Trunk volume
    void Car::setTrunkVolume(int trunkVolume) {

        // Verification
        if (trunkVolume < 0) {
            this->_trunkVolume = ERROR_INT;
            return;
        }
        this->_trunkVolume = trunkVolume;

    }


    // Weight
    void Car::setWeight(int weight) {

        // Verification
        if (weight < 0) {
            this->_weight = ERROR_INT;
            return;
        }
        this->_weight = weight;

    }


    // Seat count
    void Car::setSeatCount(int seatCount) {

        // Verification
        if (seatCount < 0) {
            this->_seatCount = ERROR_INT;
            return;
        }
        this->_seatCount = seatCount;

    }


    // Gearbox type
    void Car::setGearboxType(const std::string& gearboxType) {

        // Verification
        if (gearboxType.empty()) {
            this->_gearboxType = ERROR_STR;
            return;
        }
        // Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_gearboxType = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(gearboxType));

    }


    // Gear count
    void Car::setGearCount(int gearCount) {

        // Verification
        if (gearCount < 0) {
            this->_gearCount = ERROR_INT;
            return;
        }
        this->_gearCount = gearCount;

    }


    // Fuel type
    void Car::setFuelType(const std::string& fuelType) {

        // Verification
        if (fuelType.empty()) {
            this->_fuelType = ERROR_STR;
            return;
        }
        // Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_fuelType = una::cases::to_titlecase_utf8(una::norm::to_nfc_utf8(fuelType));

    }


    // Horse power
    void Car::setHorsePower(int horsePower) {

        // Verification
        if (horsePower < 0) {
            this->_horsePower = ERROR_INT;
            return;
        }
        this->_horsePower = horsePower;

    }


    // Tax horse power
    void Car::setTaxHorsePower(int taxHorsePower) {

        // Verification
        if (taxHorsePower < 0) {
            this->_taxHorsePower = ERROR_INT;
            return;
        }
        this->_taxHorsePower = taxHorsePower;

    }


    // Tank capacity
    void Car::setTankCapacity(int tankCapacity) {

        // Verification
        if (tankCapacity < 0) {
            this->_tankCapacity = ERROR_INT;
            return;
        }
        this->_tankCapacity = tankCapacity;

    }


    // Fuel consumption
    void Car::setFuelConsumption(double fuelConsumption) {

        // Verification
        if (fuelConsumption < 0) {
            this->_fuelConsumption = ERROR_DOUBLE;
            return;
        }
        this->_fuelConsumption = fuelConsumption;

    }


    // Co2 emissions
    void Car::setCo2Emissions(int co2Emissions) {

        // Verification
        if (co2Emissions < 0) {
            this->_co2Emissions = ERROR_INT;
            return;
        }
        this->_co2Emissions = co2Emissions;

    }


    // Co2 class
    void Car::setCo2Class(const std::string& co2Class) {

        // Verification
        if (co2Class.empty()) {
            this->_co2Class = ERROR_STR;
            return;
        }
        // Normalize UTF-8 (NFC) then apply upper case (short code like "A", "B+")
        this->_co2Class = una::cases::to_uppercase_utf8(una::norm::to_nfc_utf8(co2Class));
    
    }


    // Year start
    void Car::setYearStart(const std::string& yearStart) {

        // Verification
        if (yearStart.empty()) {
            this->_yearStart = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) only — year is a numeric string (ex: "2012")
        this->_yearStart = una::norm::to_nfc_utf8(yearStart);

    }


    // Year end
    void Car::setYearEnd(const std::string& yearEnd) {

        // Verification
        if (yearEnd.empty()) {
            this->_yearEnd = ERROR_STR;
            return;
        }

        // Normalize UTF-8 (NFC) only — year is a numeric string (ex: "2019")
        this->_yearEnd = una::norm::to_nfc_utf8(yearEnd);

    }





    // =========================================================================
    // Utilities
    // =========================================================================


    /**
     * Returns true if the car data has been properly filled (no default sentinel values)
     */
    bool Car::isComplete() const {
        return _brand           != DEFAULT_STR
            && _model           != DEFAULT_STR
            && _generation      != DEFAULT_STR
            && _engine          != DEFAULT_STR
            && _trim            != DEFAULT_STR
            && _price           != DEFAULT_INT
            && _height          != DEFAULT_DOUBLE
            && _length          != DEFAULT_DOUBLE
            && _width           != DEFAULT_DOUBLE
            && _trunkVolume     != DEFAULT_INT
            && _weight          != DEFAULT_INT
            && _seatCount       != DEFAULT_INT
            && _gearboxType     != DEFAULT_STR
            && _gearCount       != DEFAULT_INT
            && _fuelType        != DEFAULT_STR
            && _horsePower      != DEFAULT_INT
            && _taxHorsePower   != DEFAULT_INT
            && _tankCapacity    != DEFAULT_INT
            && _fuelConsumption != DEFAULT_DOUBLE
            && _co2Emissions    != DEFAULT_INT
            && _co2Class        != DEFAULT_STR
            && _yearStart       != DEFAULT_STR
            && _yearEnd         != DEFAULT_STR;
    }


    /**
     * Returns true if the car data has been properly filled (no error sentinel values)
     */
    bool Car::isError() const {
        return _brand           != ERROR_STR
            && _model           != ERROR_STR
            && _generation      != ERROR_STR
            && _engine          != ERROR_STR
            && _trim            != ERROR_STR
            && _price           != ERROR_INT
            && _height          != ERROR_DOUBLE
            && _length          != ERROR_DOUBLE
            && _width           != ERROR_DOUBLE
            && _trunkVolume     != ERROR_INT
            && _weight          != ERROR_INT
            && _seatCount       != ERROR_INT
            && _gearboxType     != ERROR_STR
            && _gearCount       != ERROR_INT
            && _fuelType        != ERROR_STR
            && _horsePower      != ERROR_INT
            && _taxHorsePower   != ERROR_INT
            && _tankCapacity    != ERROR_INT
            && _fuelConsumption != ERROR_DOUBLE
            && _co2Emissions    != ERROR_INT
            && _co2Class        != ERROR_STR
            && _yearStart       != ERROR_STR
            && _yearEnd         != ERROR_STR;
    }


    /**
     * Returns a string representation of the car (for debugging purposes)
     */
    std::string Car::toString() const {
        std::ostringstream oss;
        oss << "[Car] "     << _brand        << " " << _model   << " (gen. " << _generation << ")"
            << " | "        << _engine      << " | " << _trim
            << " | "        << _horsePower  << " hp | " << _fuelType
            << " | "        << _yearStart   << "-" << _yearEnd;
        return oss.str();
    }




    // =========================================================================
    // Stream operator overload
    // =========================================================================
    std::ostream& operator<<(std::ostream& os, const Car& car) {

        os << std::fixed << std::setprecision(2);

        os << "Car :\n";

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

        // Years
        os << "- Years\n";
        os << "    -> " << std::left << std::setw(16) << "YearStart"      << ": \""     << car.getYearStart()       << "\"\n";
        os << "    -> " << std::left << std::setw(16) << "YearEnd"        << ": \""     << car.getYearEnd()         << "\"\n";

        return os;
    }

} // namespace CarScraper