// Imports
#include <iostream>
#include "core/models/Car.hpp"
#include "core/models/Entity.hpp"
#include "io/HttpClient/HttpClient.hpp"
#include "core/logger/Logger.hpp"





/**
 * Main function
 */
int main() {

    // Debug
    CarScraper::Logger::init();
    CarScraper::Logger::info("MAIN : Application start");



    // New Enity
    //CarScraper::Entity entity1;
    //CarScraper::Entity entity2 = CarScraper::Entity("TEST");


    // New Car
    CarScraper::Car car;
    std::cout << car << std::endl;


    // Get the raw HTML of a car listing page (for testing purposes)
    //CarScraper::HttpClient httpClient;
    //CarScraper::HttpResponse response = httpClient.get("https://www.largus.fr/fiche-technique/Renault/Clio/IV+B98/2020/Berline+5+Portes/09+TCe+75ch+G%C3%A9n%C3%A9ration+-+19+5p-2149663.html");
    //std::cout << response.body << std::endl;
    


    // Debug
    CarScraper::Logger::info("MAIN : Application finnish");

    return 0;
}