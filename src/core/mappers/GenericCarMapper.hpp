/**
 * @file GenericCarMapper.hpp
 * 
 * @brief This file declares the GenericCarMapper class, which is a generic mapper able to generate a Car Entity from a technical sheet.
 *
 * @author Adrien GRAS
 * @date 2026-07-03
 */


// For only one include
#pragma once


// Imports
#include "core/models/Entity.hpp"
#include "core/models/Car.hpp"


/**
 * Namespace CarScraper
 */
namespace CarScraper {

    /**
     * @class GenericCarMapper
     * 
     * @brief Class representing a generic car mapper
     */
    class GenericCarMapper : public Entity {

        // =========================================================================
        // Protected data and methods
        // =========================================================================
        protected:

            // Input file
            std::string _inputFile; ///< Input file path for the technical sheet

            // Generated car entity
            Car _car;               ///< Generated car entity from the technical sheet





            // -------------------------------------------------------------------------
            // Internal Helpers
            // -------------------------------------------------------------------------

            /**
             * @brief Trims a string into a cleaner version to be cast into Int or Double
             * @param value The string to trim
             * @return The trimmed string
             */
            std::string& cleanNumber(std::string& value);


            /**
             * @brief extracts an int from a optional string from a HTML
             * @param str the extracted string
             * @return the int value extracted or ERROR_INT
             */
            int toInt(std::optional<std::string>& str);


            /**
             * @brief Extracts a double from an optional string from HTML.
             * @param str the extracted string
             * @return the double value extracted or ERROR_DOUBLE.
             */
            double toDouble(std::optional<std::string>& str);


            /**
             * @brief Extract the content of a file
             * @param path the file path
             * @return The content of the file
             */
            std::string readFile(const std::string& path);





        // =========================================================================
        // Public data and methods
        // =========================================================================
        public:

            // -------------------------------------------------------------------------
            // Constructor / Destructor
            // -------------------------------------------------------------------------

            /**
             * @brief Default constructor
             */
            GenericCarMapper();


            /**
             * @brief Constructor which define the mapper PREFIX
             * @param prefix The prefix for the full id
             */
            GenericCarMapper(const std::string& prefix);


            virtual ~GenericCarMapper() = default;




            
            // -------------------------------------------------------------------------
            // Getters
            // -------------------------------------------------------------------------

            /**
             * @brief Gets the input file path for the technical sheet
             * @return The input file path for the technical sheet
             */
            const std::string& getInputFile() const { return _inputFile; }


            /**
             * @brief Gets the generated car entity from the technical sheet
             * @return The generated car entity
             */
            const Car& getCar() const { return _car; }





            // -------------------------------------------------------------------------
            // Setters
            // -------------------------------------------------------------------------

            /**
             * @brief Sets the input file path for the technical sheet
             * @param inputFile The input file path for the technical sheet
             * @note Checks if the file exists
             */
            bool setInputFile(const std::string& inputFile);





            // -------------------------------------------------------------------------
            // Main logic
            // -------------------------------------------------------------------------

            /**
             * @brief Maps the technical sheet to a Car entity
             * @return ERROR_CODE / SUCCESS_CODE wheter the method is succesful or not
             * @note Generic method to implement
             */
            virtual int mapToCar() = 0;

    };

} // namespace CarScraper