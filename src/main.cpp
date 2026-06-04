// Imports
#include <iostream>
#include "core/models/Car.hpp"
#include "core/models/Entity.hpp"
#include "io/HttpClient/HttpClient.hpp"
#include "io/HtmlSaver/HtmlSaver.hpp"
#include "core/logger/Logger.hpp"





/**
 * Main function
 */
int main() {

    // Debug
    CarScraper::Logger::init("logs/log.txt");
    CarScraper::Logger::info("MAIN : Application start");



    
    


    // Debug
    CarScraper::Logger::info("MAIN : Application finnish");

    return 0;
}