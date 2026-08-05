# Usage :
#
# - From root directory, go to the folder build
# - Reset : rm -rf CMakeFiles/ && rm CMakeCache.txt
# - Setup : cmake .. -DCMAKE_BUILD_TYPE=Release     For full realease
# - Setup : cmake .. -DCMAKE_BUILD_TYPE=Debug       For lighter process
# - Build : cmake --build . -j$(nproc)
# The last command compile everything with 2 parrallel process (fatser)


cmake_minimum_required(VERSION 3.20)
project(CarScraper VERSION 0.1.0 LANGUAGES CXX)


# ==============================================================================
# Build type — default to Release if not specified
# ==============================================================================

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()
message(STATUS "CarScraper build type: ${CMAKE_BUILD_TYPE}")


# ==============================================================================
# Compiler cache — speeds up repeated builds if ccache/sccache is installed
# ==============================================================================

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "ccache found — enabling compiler launcher")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()


# ==============================================================================
# Global settings
# ==============================================================================

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Link-Time Optimization — Release builds only ---
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
if(IPO_SUPPORTED AND CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "LTO enabled for Release build")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    if(NOT IPO_SUPPORTED)
        message(STATUS "LTO not supported: ${IPO_ERROR}")
    endif()
endif()


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

# --- nlohmann (header-only) : libs/nlohmann/json.hpp ---
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
        $<$<CXX_COMPILER_ID:GNU,Clang>: $<$<CONFIG:Release>: -O3>>
        $<$<CXX_COMPILER_ID:MSVC>:      /W4>
    )
endfunction()


# ==============================================================================
# Core sources (compiled ONCE into CarScraperLib / CarScraperLibTest)
# ==============================================================================

set(CORE_SOURCES

    # Core - Enum
    src/core/enum/Co2Class.cpp
    src/core/enum/DataSource.cpp
    src/core/enum/FuelType.cpp
    src/core/enum/GearboxType.cpp
    src/core/enum/TextCase.cpp

    # Core — Logger
    src/core/logger/Logger.cpp

    # Core — Mapper
    src/core/mappers/ArgusMapper.cpp
    src/core/mappers/CaradisiacMapper.cpp
    src/core/mappers/CrossMapper.cpp
    src/core/mappers/GenericCarMapper.cpp

    # Core — Models
    src/core/models/Entity.cpp
    src/core/models/Car.cpp

    # Core — Scraper
    src/core/scrapers/GenericCarScraper.cpp
    src/core/scrapers/ArgusScraper.cpp
    src/core/scrapers/CaradisiacScraper.cpp

    # Core — Service
    src/core/services/CleanService.cpp
    src/core/services/MapService.cpp
    src/core/services/ScrapService.cpp

    # Core — Utils
    src/core/utils/Validation.cpp

    # IO — HttpClient
    src/io/httpClient/HttpClient.cpp

    # IO — HtmlHandlers
    src/io/htmlHandlers/HtmlParser.cpp
    src/io/htmlHandlers/HtmlSaver.cpp

    # IO — ExcelReader
    src/io/excelReader/ExcelReader.cpp

)


# ==============================================================================
# CarScraperLib — production lib (no TESTING flag)
# ==============================================================================
# NOTE: this is now the ONLY target that compiles CORE_SOURCES for production.
# The main executable links against it instead of recompiling the same .cpp
# files a second time.

add_library(CarScraperLib STATIC ${CORE_SOURCES})
carscraper_configure_target(CarScraperLib)


# ==============================================================================
# Main executable
# ==============================================================================

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE CarScraperLib ${LIBZIP_LIBRARIES})
carscraper_configure_target(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY      "${CMAKE_SOURCE_DIR}/build"
)


# ==============================================================================
# CarScraperLibTest — test lib (compiled with TESTING flag)
# ==============================================================================
# This is the one unavoidable duplicate compilation of CORE_SOURCES: it needs
# the TESTING preprocessor flag, so it can't reuse the CarScraperLib object
# files. Every test target below links against this single lib instead of
# rebuilding CORE_SOURCES itself.

add_library(CarScraperLibTest STATIC ${CORE_SOURCES})
carscraper_configure_target(CarScraperLibTest)
target_compile_definitions(CarScraperLibTest PRIVATE TESTING)


# ==============================================================================
# Tests — mono
# ==============================================================================

# Helper — apply working directory + link CarScraperLibTest to any test target
function(carscraper_configure_test TARGET)
    target_link_libraries(${TARGET} PRIVATE CarScraperLibTest)
    carscraper_configure_target(${TARGET})
    target_compile_definitions(${TARGET} PRIVATE TESTING)
    set_target_properties(${TARGET} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY      "${CMAKE_SOURCE_DIR}/build/tests"
    )
endfunction()

# test Logger
add_executable(TestLogger tests/mono/testLogger.cpp)
carscraper_configure_test(TestLogger)

# test Validation
add_executable(TestValidation tests/mono/testValidation.cpp)
carscraper_configure_test(TestValidation)

# test Enum
add_executable(TestEnum tests/mono/testEnum.cpp)
carscraper_configure_test(TestEnum)

# test Entity
add_executable(TestEntity tests/mono/testEntity.cpp)
carscraper_configure_test(TestEntity)

# test Car
add_executable(TestCar tests/mono/testCar.cpp)
carscraper_configure_test(TestCar)

# test HttpClient
add_executable(TestHttpClient tests/mono/testHttpClient.cpp)
carscraper_configure_test(TestHttpClient)
# ./TestHttpClient "[httpclient]" "~[network]"      -> to disable network tests (e.g., in CI)

# test HtmlSaver
add_executable(TestHtmlSaver tests/mono/testHtmlSaver.cpp)
carscraper_configure_test(TestHtmlSaver)
# ./TestHtmlSaver "[HtmlSaver]" "~[network]"        -> to disable network tests (e.g., in CI)

# test ExcelReader
add_executable(TestExcelReader tests/mono/testExcelReader.cpp)
carscraper_configure_test(TestExcelReader)

# test HtmlParser
add_executable(TestHtmlParser tests/mono/testHtmlParser.cpp)
carscraper_configure_test(TestHtmlParser)

# test GenericCarScraper
add_executable(TestGenericCarScraper tests/mono/testGenericCarScraper.cpp)
carscraper_configure_test(TestGenericCarScraper)

# test GenericCarMapper
add_executable(TestGenericCarMapper tests/mono/testGenericCarMapper.cpp)
carscraper_configure_test(TestGenericCarMapper)

# Test ArgusMapper
add_executable(TestArgusMapper tests/mono/testArgusMapper.cpp)
carscraper_configure_test(TestArgusMapper)
 
# Test CaradisiacMapper
add_executable(TestCaradisiacMapper tests/mono/testCaradisiacMapper.cpp)
carscraper_configure_test(TestCaradisiacMapper)



# ==============================================================================
# Tests — all
# ==============================================================================

add_executable(TestAll tests/testAll.cpp)
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