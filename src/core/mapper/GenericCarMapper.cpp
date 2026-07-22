/**
 * @file GenericCarMapper.cpp
 * 
 * @brief This file implements the GenericCarMapper class, which is a generic mapper able to generate a Car Entity from a technical sheet.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// Imports
#include "GenericCarMapper.hpp"
#include "core/utils/Constant.hpp"
#include "core/logger/Logger.hpp"
#include <uni_algo/case.h>
#include <uni_algo/norm.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    // =========================================================================
    // Constructor / Destructor
    // =========================================================================

    /**
     * @brief Default constructor
     */
    GenericCarMapper::GenericCarMapper() : GenericCarMapper("MAPPER") {}


    /**
     * @brief Constructor which define the mapper PREFIX
     * @param prefix The prefix for the full id
     */
    GenericCarMapper::GenericCarMapper(const std::string& prefix) : Entity(prefix) {

        // Default value
        _inputFile = DEFAULT_STR;

    }





    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Sets the input file path for the technical sheet
     * @param inputFile The input file path for the technical sheet
     * @note Checks if the file exists
     */
    bool GenericCarMapper::setInputFile(const std::string& inputFile) {

        // Getting namespace
        namespace fs = std::filesystem;


        // Checking if the file exists
        if (fs::exists(inputFile)) {
            _inputFile = inputFile;
            Logger::trace("[{}].setInputFile : {}", getFullId(), _inputFile);
            return true;
        } else {
            _inputFile = DEFAULT_STR;
            Logger::debug("[{}].setInputFile : File {} does not exists", getFullId(), inputFile);
            return false;
        }

    }





    // =========================================================================
    // Internal helpers
    // =========================================================================


    /**
     * @brief Trims a string into a cleaner version to be cast into Int or Double
     * @param value The string to trim
     * @return The trimmed string
     */
    std::string& GenericCarMapper::cleanNumber(std::string &value) {

        // Coma replacing
        std::replace(value.begin(), value.end(), ',', '.');


        // Triming
        value.erase(
            std::remove(value.begin(), value.end(), ' '),
            value.end()
        );
        return value;

    }


    /**
     * @brief extracts an int from a optional string from a HTML
     * @param str the extracted string
     * @return the int value extracted or ERROR_INT
     */
    int GenericCarMapper::toInt(std::optional<std::string>& str) {

        // Checking if the string is empty
        if (!str) {
            return ERROR_INT;
        }
        

        // Trying the conversion
        try {
            return std::stoi(cleanNumber(str.value()));
        } catch (...) {
            return ERROR_INT;
        }
        
    }


    /**
     * @brief Extracts a double from an optional string extracted from HTML.
     * @param str the extracted string
     * @return the double value extracted or ERROR_DOUBLE.
     */
    double GenericCarMapper::toDouble(std::optional<std::string>& str) {

        // Checking if the string is empty
        if (!str) {
            return ERROR_DOUBLE;
        }


        // Trying the conversion
        try {
            return std::stod(cleanNumber(str.value()));
        } catch (...) {
            return ERROR_DOUBLE;
        }

    }

    
    /**
     * @brief Extract the content of a file
     * @param path the file path
     * @return The content of the file
     */
    std::string GenericCarMapper::readFile(const std::string& path) {

        // File opening
        std::ifstream file(path);
        if (!file.is_open()) {
            return ERROR_STR;
        }

        
        // Content extraction
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();

    }


    /**
     * @brief Converts a string to lowercase
     * @param str The string to convert
     * @return The lowercase string
     */
    std::string GenericCarMapper::toLowerCase(const std::string& str) {
        std::string lowerStr = str;
        std::transform(
            lowerStr.begin(), 
            lowerStr.end(), 
            lowerStr.begin(), 
            ::tolower);
        return lowerStr;
    }





    // =========================================================================
    // Main logic
    // =========================================================================

    /**
     * @brief Maps the technical sheet to a Car entity
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note Generic method to implement
     */
    int GenericCarMapper::mapToCar() {

        Logger::error("[{}].mapToCar : to implement", getFullId());
        return IGNORED_ACTION_CODE;

    }

} // namespace CarScraper