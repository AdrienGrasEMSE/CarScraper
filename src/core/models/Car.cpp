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

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * Sets the brand value
     * 
     * @param brand The brand value
     */
    void Car::setBrand(const std::string& brand) {

        // Verification + Normalize UTF-8 (NFC) then apply upper case
        this->_brand = una::cases::to_uppercase_utf8(
            una::norm::to_nfc_utf8(stringValidation(brand, "Brand", 100)));

    }


    /**
     * Sets the model value
     * 
     * @param model The model value
     */
    void Car::setModel(const std::string& model) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_model = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(stringValidation(model, "Model", 200)));

    }


    /**
     * Sets the generation value
     *
     * @param generation The generation value
     */
    void Car::setGeneration(const std::string& generation) {

        // Verification + Normalize UTF-8 (NFC) only — generation is a technical identifier (ex: "4", "IV")
        this->_generation = una::norm::to_nfc_utf8(stringValidation(generation, "Generation", 50));

    }


    /**
     * Sets the engine value
     *
     * @param engine The engine value
     */
    void Car::setEngine(const std::string& engine) {

        // Verification + Normalize UTF-8 (NFC) only — engine is a technical identifier (ex: "1.5 dCi 90hp")
        this->_engine = una::norm::to_nfc_utf8(stringValidation(engine, "Engine", 200));

    }


    /**
     * Sets the trim value
     * 
     * @param trim The trim value
     */
    void Car::setTrim(const std::string& trim) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_trim = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(stringValidation(trim, "Trim", 200)));
    
    }


    /**
     * Sets the price value
     * 
     * @param price The price value
     */
    void Car::setPrice(int price) {

        // Verification
        this->_price = intValidation(price, "Price", 0, 10000000);

    }


    /**
     * Sets the height value
     * 
     * @param height The height value
     */
    void Car::setHeight(double height) {

        // Verification
        this->_height = doubleValidation(height, "Height", 0.0, 10.0);

    }


    /**
     * Sets the length value
     * 
     * @param length The length value
     */
    void Car::setLength(double length) {

        // Verification
        this->_length = doubleValidation(length, "Length", 0.0, 10.0);

    }


    /**
     * Sets the width value
     * 
     * @param width The width value
     */
    void Car::setWidth(double width) {

        // Verification
        this->_width = doubleValidation(width, "Width", 0.0, 10.0);

    }


    /**
     * Sets the trunk volume value
     * 
     * @param trunkVolume The trunk volume value
     */
    void Car::setTrunkVolume(int trunkVolume) {

        // Verification
        this->_trunkVolume = intValidation(trunkVolume, "TrunkVolume", 0, 1000);

    }


    /**
     * Sets the weight value
     * 
     * @param weight The weight value
     */
    void Car::setWeight(int weight) {

        // Verification
        this->_weight = intValidation(weight, "Weight", 0, 5000);

    }


    /**
     * Sets the seat count value
     * 
     * @param seatCount The seat count value
     */
    void Car::setSeatCount(int seatCount) {

        // Verification
        this->_seatCount = intValidation(seatCount, "SeatCount", 0, 10);

    }


    // Gearbox type
    void Car::setGearboxType(const std::string& gearboxType) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_gearboxType = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(stringValidation(gearboxType, "GearboxType", 50)));

    }


    /**
     * Sets the gear count value
     * 
     * @param gearCount The gear count value
     */
    void Car::setGearCount(int gearCount) {

        // Verification
        this->_gearCount = intValidation(gearCount, "GearCount", 0, 20);

    }


    // Fuel type
    void Car::setFuelType(const std::string& fuelType) {

        // Verification + Normalize UTF-8 (NFC) then apply title case (handles accented chars)
        this->_fuelType = una::cases::to_titlecase_utf8(
            una::norm::to_nfc_utf8(stringValidation(fuelType, "FuelType", 50)));

    }


    /**
     * Sets the horse power value
     * 
     * @param horsePower The horse power value
     */
    void Car::setHorsePower(int horsePower) {

        // Verification
        this->_horsePower = intValidation(horsePower, "HorsePower", 0, 3000);

    }


    /**
     * Sets the tax horse power value
     * 
     * @param taxHorsePower The tax horse power value
     */
    void Car::setTaxHorsePower(int taxHorsePower) {

        // Verification
        this->_taxHorsePower = intValidation(taxHorsePower, "TaxHorsePower", 0, 200);

    }


    /**
     * Sets the tank capacity value
     * 
     * @param tankCapacity The tank capacity value
     */
    void Car::setTankCapacity(int tankCapacity) {

        // Verification
        this->_tankCapacity = intValidation(tankCapacity, "TankCapacity", 0, 500);

    }


    /**
     * Sets the fuel consumption value
     * 
     * @param fuelConsumption The fuel consumption value
     */
    void Car::setFuelConsumption(double fuelConsumption) {

        // Verification
        this->_fuelConsumption = doubleValidation(fuelConsumption, "FuelConsumption", 0.0, 50.0);

    }


    /**
     * Sets the CO2 emissions value
     * 
     * @param co2Emissions The CO2 emissions value
     */
    void Car::setCo2Emissions(int co2Emissions) {

        // Verification
        this->_co2Emissions = intValidation(co2Emissions, "Co2Emissions", 0, 200);

    }


    /**
     * Sets the CO2 class value
     * 
     * @param co2Class The CO2 class value
     */
    void Car::setCo2Class(const std::string& co2Class) {

        // Verification + Normalize UTF-8 (NFC) then apply upper case (short code like "A", "B+")
        this->_co2Class = una::cases::to_uppercase_utf8(
            una::norm::to_nfc_utf8(stringValidation(co2Class, "Co2Class", 10)));
    
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
     * Checks if the car data has been filled (no default sentinel values)
     *
     * @return true if the car data is complete, false otherwise
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
     * Checks if the car data has been properly filled (no error sentinel values)
     *
     * @return true if the car data is valid, false otherwise
     */
    bool Car::isValid() const {
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
     * Validates and normalizes a string value
     * 
     * @param strVal The string value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param maxLength The maximum allowed length for the string (default: 255)
     */
    std::string Car::stringValidation(  const std::string&      strVal,
                                        const std::string&      attributeName,
                                        const long unsigned int maxLength) {

        // Check if the string is empty
        if (strVal.empty()) {
            Logger::error("Car::set{} got an empty value", attributeName);
            return ERROR_STR;
        }


        // Check if the string exceeds the maximum length
        if (strVal.length() > maxLength) {
            Logger::error(  "Car::set{} got a value that exceeds the maximum length of {} characters",
                            attributeName, std::to_string(maxLength));
            return ERROR_STR;
        }


        // String valid
        Logger::trace("Car::set{} value: \"{}\"", attributeName, strVal);
        return strVal;

    }


    /**
     * Validates and normalizes an integer value
     *
     * @param intVal The integer value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param minValue The minimum allowed value for the integer (default: 0)
     * @param maxValue The maximum allowed value for the integer (default: 9999)
     */
    int Car::intValidation( const int           intVal,
                            const std::string&  attributeName,
                            const int           minValue,
                            const int           maxValue) {

        // Check if the integer is below the minimum value
        if (intVal < minValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} < {}",
                            attributeName, std::to_string(intVal), std::to_string(minValue));
            return ERROR_INT;
        }


        // Check if the integer is above the maximum value
        if (intVal > maxValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} > {}",
                            attributeName, std::to_string(intVal), std::to_string(maxValue));
            return ERROR_INT;
        }


        // Integer valid
        Logger::trace("Car::set{} value: {}", attributeName, std::to_string(intVal));
        return intVal;

    }


    /**
     * Validates and normalizes a double value
     *
     * @param doubleVal The double value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param minValue The minimum allowed value for the double (default: 0.0)
     * @param maxValue The maximum allowed value for the double (default: 9999.0)
     */
    double Car::doubleValidation(   const double        doubleVal,
                                    const std::string&  attributeName,
                                    const double        minValue,
                                    const double        maxValue) {

        // Check if the double is below the minimum value
        if (doubleVal < minValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} < {}",
                            attributeName, std::to_string(doubleVal), std::to_string(minValue));
            return ERROR_DOUBLE;
        }


        // Check if the double is above the maximum value
        if (doubleVal > maxValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} > {}",
                            attributeName, std::to_string(doubleVal), std::to_string(maxValue));
            return ERROR_DOUBLE;
        }


        // Double valid
        Logger::trace("Car::set{} value: {}", attributeName, std::to_string(doubleVal));
        return doubleVal;

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

        os << car.getFullId() << "\"\n";

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