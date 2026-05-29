// Imports
#include "Validation.hpp"
#include "core/logger/Logger.hpp"


/**
 * Namespace CarScraper::Validation
 */
 namespace CarScraper::Validation {

    // =========================================================================
    // String and numeric validations
    // =========================================================================

    /**
     * Validates and normalizes a string value
     * 
     * Log ERROR if the string is empty or exceeds the maximum length
     * Log TRACE with the validated value if the string is valid
     * 
     * @param strVal The string value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param maxLength The maximum allowed length for the string (default: 255)
     * @return The validated string value
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
                            attributeName, maxLength);
            return ERROR_STR;
        }


        // String valid
        Logger::trace("Car::set{} value: \"{}\"", attributeName, strVal);
        return strVal;

    }


    /**
     * Validates and normalizes an integer value
     *
     * Log ERROR if the integer is out of bounds
     * Log TRACE with the validated value if the integer is valid
     *
     * @param intVal The integer value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param minValue The minimum allowed value for the integer (default: 0)
     * @param maxValue The maximum allowed value for the integer (default: 9999)
     * @return The validated integer value
     */
    int Car::intValidation( const int           intVal,
                            const std::string&  attributeName,
                            const int           minValue,
                            const int           maxValue) {

        // Check if the integer is below the minimum value
        if (intVal < minValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} < {}",
                            attributeName, intVal, minValue);
            return ERROR_INT;
        }


        // Check if the integer is above the maximum value
        if (intVal > maxValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} > {}",
                            attributeName, intVal, maxValue);
            return ERROR_INT;
        }


        // Integer valid
        Logger::trace("Car::set{} value: {}", attributeName, intVal);
        return intVal;

    }


    /**
     * Validates and normalizes a double value
     *
     * Log ERROR if the double is out of bounds
     * Log TRACE with the validated value if the double is valid
     *
     * @param doubleVal The double value to validate and normalize
     * @param attributeName The name of the attribute being validated (for logging purposes)
     * @param minValue The minimum allowed value for the double (default: 0.0)
     * @param maxValue The maximum allowed value for the double (default: 9999.0)
     * @return The validated double value
     */
    double Car::doubleValidation(   const double        doubleVal,
                                    const std::string&  attributeName,
                                    const double        minValue,
                                    const double        maxValue) {

        // Check if the double is below the minimum value
        if (doubleVal < minValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} < {}",
                            attributeName, doubleVal, minValue);
            return ERROR_DOUBLE;
        }


        // Check if the double is above the maximum value
        if (doubleVal > maxValue) {
            Logger::error(  "Car::set{} got a value out of bounds: {} > {}",
                            attributeName, doubleVal, maxValue);
            return ERROR_DOUBLE;
        }


        // Double valid
        Logger::trace("Car::set{} value: {}", attributeName, doubleVal);
        return doubleVal;

    }





    // =========================================================================
    // Date parsing and formatting
    // =========================================================================

    // -------------------------------------------------------------------------
    // Helper interne — non exposé dans le .hpp
    // -------------------------------------------------------------------------

    /**
     * Builds a date from day, month, and year.
     *
     * @param day The day of the date.
     * @param month The month of the date.
     * @param year The year of the date.
     * @param attributeName The name of the attribute being validated.
     * @return The built date or std::nullopt if the date is invalid.
     */
    static std::optional<std::chrono::year_month_day> buildDate(int day, int month, int year) {

        // Date construction with chrono
        std::chrono::year_month_day date{
            std::chrono::year{year},
            std::chrono::month{static_cast<unsigned>(month)},
            std::chrono::day{static_cast<unsigned>(day)}
        };


        // Validation
        if (!date.ok()) {
            Logger::error("Validation::buildDate got an invalid date: {}/{}/{}", day, month, year);
            return std::nullopt;
        }


        // Date valid
        Logger::trace("Validation::buildDate value: {}/{}/{}", day, month, year);
        return date;
    }


    /**
     * Splits a string into three parts based on a separator.
     *
     * @param v The string to split.
     * @param sep The separator character.
     * @param a The first part.
     * @param b The second part.
     * @param c The third part.
     * @return True if the string was successfully split, false otherwise.
     */
    static bool splitThreeParts(const std::string&  v,
                                char                sep,
                                int&                a,
                                int&                b,
                                int&                c) {

        // Try to split the string and convert the parts to integers
        try {
            size_t first  = v.find(sep);
            size_t second = v.find(sep, first + 1);

            if (first  == std::string::npos ||
                second == std::string::npos ||
                second == v.size() - 1) return false;

            a = std::stoi(v.substr(0, first));
            b = std::stoi(v.substr(first + 1, second - first - 1));
            c = std::stoi(v.substr(second + 1));
            return true;
        }
        catch (...) { return false; }

    }


    // -------------------------------------------------------------------------
    // Parsers publics
    // -------------------------------------------------------------------------

    /**
     * Parses a date string in the format DD/MM/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateDMY(const std::string& strVal) {

        // Final date parts
        int day, month, year;


        // String splitting and conversion to integers
        if (!splitThreeParts(strVal, '/', day, month, year)) {
            Logger::error("Validation::parseDateDMY got a malformed date (expected DD/MM/YYYY): {}", strVal);
            return std::nullopt;
        }


        // Valid date
        return buildDate(day, month, year);
    }


    /**
     * Parses a date string in the format YYYY/MM/DD.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateYMD(const std::string& strVal) {

        // Final date parts
        int year, month, day;


        // String splitting and conversion to integers
        if (!splitThreeParts(strVal, '/', year, month, day)) {
            Logger::error("Validation::parseDateYMD got a malformed date (expected YYYY/MM/DD): {}", strVal);
            return std::nullopt;
        }


        // Valid date
        return buildDate(day, month, year);
    }


    /**
     * Parses a date string in the format MM/DD/YYYY.
     *
     * @param strVal The date string to parse.
     * @return The parsed date or std::nullopt if the format is invalid.
     */
    std::optional<std::chrono::year_month_day> parseDateMDY(const std::string& strVal) {

        // Final date parts
        int month, day, year;


        // String splitting and conversion to integers
        if (!splitThreeParts(strVal, '/', month, day, year)) {
            Logger::error("Validation::parseDateMDY got a malformed date (expected MM/DD/YYYY): {}", strVal);
            return std::nullopt;
        }


        // Valid date
        return buildDate(day, month, year);
    }


    /**
     * Formats a date into a string in the format DD/MM/YYYY.
     *
     * @param date The date to format.
     * @return The formatted date string.
     */
    std::string formatDate(const std::chrono::year_month_day& date) {
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << static_cast<unsigned>(date.day())   << "/"
            << std::setw(2) << static_cast<unsigned>(date.month()) << "/"
            << std::setw(4) << static_cast<int>(date.year());
        return oss.str();
    }


} // namespace CarScraper::Validation