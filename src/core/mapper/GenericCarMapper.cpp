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
    // Main logic
    // =========================================================================

    /**
     * @brief Maps the technical sheet to a Car entity
     * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
     * @note Generic method to implement
     */
    int GenericCarMapper::mapToCar() const {

        Logger::error("[{}].mapToCar : to implement", getFullId());
        return IGNORED_ACTION_CODE;

    }

} // namespace CarScraper