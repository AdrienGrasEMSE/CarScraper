cmake_minimum_required(VERSION 3.20)
project(CarScraper VERSION 0.1.0 LANGUAGES CXX)


# ==============================================================================
# Global settings
# ==============================================================================

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ==============================================================================
# Dependencies (FetchContent) — compiled libs only
# ==============================================================================

# --- libzip / libcurl / libxml2 (system libraries) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP  REQUIRED libzip)
pkg_check_modules(CURL    REQUIRED libcurl)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)


# ==============================================================================
# Dependencies (header-only) — libs/
# ==============================================================================

# --- spdlog (header-only) : libs/spdlog/spdlog.h ---
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_SOURCE_DIR}/libs)

# --- uni-algo (header-only) : libs/uni-algo/... ---
add_library(uni-algo INTERFACE)
target_include_directories(uni-algo INTERFACE ${CMAKE_SOURCE_DIR}/libs)
target_compile_definitions(uni-algo INTERFACE UNI_ALGO_STATIC_DATA)

# --- stduuid (header-only) : libs/stduuid/uuid.h ---
add_library(stduuid INTERFACE)
target_include_directories(stduuid INTERFACE ${CMAKE_SOURCE_DIR}/libs)

# --- Catch2 v2 (header-only) : libs/catch2/catch.hpp ---
add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE ${CMAKE_SOURCE_DIR}/libs)

# --- Catch2 v2 (header-only) : libs/nlohmann/json.hpp ---
add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE ${CMAKE_SOURCE_DIR}/libs)


# ==============================================================================
# Helper — apply common settings to any target
# ==============================================================================

function(carscraper_configure_target TARGET)
    target_include_directories(${TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CURL_INCLUDE_DIRS}
        ${LIBZIP_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIRS}
    )
    target_link_libraries(${TARGET} PRIVATE
        ${CURL_LIBRARIES}
        ${LIBZIP_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        spdlog
        uni-algo
        stduuid
        catch2
        nlohmann
    )
    target_compile_options(${TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:      /W4>
    )
endfunction()


# ==============================================================================
# Core sources (shared between lib and test lib)
# ==============================================================================

set(CORE_SOURCES
    # Core — Logger
    src/core/logger/Logger.cpp

    # Core — Utils
    src/core/utils/Validation.cpp

    # Core - Enum
    src/core/enum/FuelType.cpp
    src/core/enum/GearboxType.cpp
    src/core/enum/Co2Class.cpp

    # Core — Models
    src/core/models/Entity.cpp
    src/core/models/Car.cpp

    # Core — Scraper
    src/core/scraper/GenericCarScraper.cpp
    src/core/scraper/ArgusScraper.cpp

    # IO — HttpClient
    src/io/httpClient/HttpClient.cpp

    # IO — HtmlSaver
    src/io/htmlSaver/HtmlSaver.cpp

    # IO — ExcelReader
    src/io/excelReader/ExcelReader.cpp

    # IO — HtmlParser
    src/io/htmlParser/HtmlParser.cpp

)


# ==============================================================================
# Main executable
# ==============================================================================

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${CORE_SOURCES}
)
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenXLSX::OpenXLSX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBZIP_LIBRARIES})
carscraper_configure_target(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY      "${CMAKE_SOURCE_DIR}/build"
)


# ==============================================================================
# CarScraperLib — production lib (no TESTING flag)
# ==============================================================================

add_library(CarScraperLib STATIC ${CORE_SOURCES})
carscraper_configure_target(CarScraperLib)


# ==============================================================================
# CarScraperLibTest — test lib (compiled with TESTING flag)
# ==============================================================================

add_library(CarScraperLibTest STATIC ${CORE_SOURCES})
carscraper_configure_target(CarScraperLibTest)
target_compile_definitions(CarScraperLibTest PRIVATE TESTING)


# ==============================================================================
# Tests — mono
# ==============================================================================

# Helper — apply working directory to any test target
function(carscraper_configure_test TARGET)
    set_target_properties(${TARGET} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY      "${CMAKE_SOURCE_DIR}/build/tests"
    )
endfunction()

# test Logger
add_executable(TestLogger               tests/mono/testLogger.cpp)
target_link_libraries(TestLogger        PRIVATE CarScraperLibTest)
carscraper_configure_target(TestLogger)
target_compile_definitions(TestLogger   PRIVATE TESTING)
carscraper_configure_test(TestLogger)

# test Validation
add_executable(TestValidation           tests/mono/testValidation.cpp)
target_link_libraries(TestValidation    PRIVATE CarScraperLibTest)
carscraper_configure_target(TestValidation)
target_compile_definitions(TestValidation PRIVATE TESTING)
carscraper_configure_test(TestValidation)

# test Entity
add_executable(TestEntity               tests/mono/testEntity.cpp)
target_link_libraries(TestEntity        PRIVATE CarScraperLibTest)
carscraper_configure_target(TestEntity)
target_compile_definitions(TestEntity   PRIVATE TESTING)
carscraper_configure_test(TestEntity)

# test Car
add_executable(TestCar                  tests/mono/testCar.cpp)
target_link_libraries(TestCar           PRIVATE CarScraperLibTest)
carscraper_configure_target(TestCar)
target_compile_definitions(TestCar      PRIVATE TESTING)
carscraper_configure_test(TestCar)

# test HttpClient
add_executable(TestHttpClient             tests/mono/testHttpClient.cpp)
target_link_libraries(TestHttpClient      PRIVATE CarScraperLibTest)
carscraper_configure_target(TestHttpClient)
target_compile_definitions(TestHttpClient PRIVATE TESTING)
carscraper_configure_test(TestHttpClient)
# ./testHttpClient "[httpclient]" "~[network]"      -> to disable network tests (e.g., in CI)

# test HtmlSaver
add_executable(TestHtmlSaver              tests/mono/testHtmlSaver.cpp)
target_link_libraries(TestHtmlSaver       PRIVATE CarScraperLibTest)
carscraper_configure_target(TestHtmlSaver)
target_compile_definitions(TestHtmlSaver  PRIVATE TESTING)
carscraper_configure_test(TestHtmlSaver)
# ./TestHtmlSaver "[HtmlSaver]" "~[network]"        -> to disable network tests (e.g., in CI)

# test ExcelReader
add_executable(TestExcelReader            tests/mono/testExcelReader.cpp)
target_link_libraries(TestExcelReader     PRIVATE CarScraperLibTest)
carscraper_configure_target(TestExcelReader)
target_compile_definitions(TestExcelReader  PRIVATE TESTING)
carscraper_configure_test(TestExcelReader)

# Test HtmlParser
add_executable(TestHtmlParser             tests/mono/testHtmlParser.cpp)
target_link_libraries(TestHtmlParser      PRIVATE CarScraperLibTest)
carscraper_configure_target(TestHtmlParser)
target_compile_definitions(TestHtmlParser PRIVATE TESTING)
carscraper_configure_test(TestHtmlParser)


# ==============================================================================
# Tests — all
# ==============================================================================

add_executable(TestAll                  tests/testAll.cpp)
target_link_libraries(TestAll           PRIVATE CarScraperLibTest)
carscraper_configure_target(TestAll)
target_compile_definitions(TestAll      PRIVATE TESTING)
carscraper_configure_test(TestAll)


# ==============================================================================
# Documentation — Doxygen (optionnel)
# ==============================================================================

find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN  ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/docs)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "DOxygen documentation generation..."
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found — target 'doc' unavailable")
endif()