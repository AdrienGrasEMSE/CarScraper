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

include(FetchContent)


# --- OpenXLSX (compiled — FetchContent mandatory) ---
FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY  https://github.com/troldal/OpenXLSX.git
    GIT_TAG         master
)
FetchContent_MakeAvailable(OpenXLSX)


# --- libcurl (system library) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)


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


# ==============================================================================
# Helper — apply common settings to any target
# ==============================================================================

function(carscraper_configure_target TARGET)
    target_include_directories(${TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CURL_INCLUDE_DIRS}
    )
    target_link_libraries(${TARGET} PRIVATE
        ${CURL_LIBRARIES}
        spdlog
        uni-algo
        stduuid
        catch2
    )
    target_compile_options(${TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:      /W4>
    )
endfunction()


# ==============================================================================
# Sources — main executable
# ==============================================================================

set(SOURCES
    src/main.cpp

    # Core — Utils
    src/core/utils/Validation.cpp

    # Core — Logger
    src/core/Logger/Logger.cpp

    # Core — Models
    src/core/models/Car.cpp
    src/core/models/Entity.cpp

    # IO — HttpClient
    #src/io/HttpClient/HttpClient.cpp
)


# ==============================================================================
# Main executable
# ==============================================================================

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenXLSX::OpenXLSX)
carscraper_configure_target(${PROJECT_NAME})


# ==============================================================================
# Tests
# ==============================================================================

# Lib for tests — to avoid recompiling core files for each test
add_library(CarScraperLib STATIC
    src/core/Logger/Logger.cpp
    src/core/utils/Validation.cpp
    src/core/models/Entity.cpp
)
carscraper_configure_target(CarScraperLib)

# test Logger
set(TEST_LOGGER_SOURCES tests/mono/testLogger.cpp)
add_executable(TestLogger ${TEST_LOGGER_SOURCES})
target_link_libraries(TestLogger PRIVATE CarScraperLib)
carscraper_configure_target(TestLogger)
target_compile_definitions(TestLogger PRIVATE TESTING)

# test Validation
set(TEST_VALIDATION_SOURCES tests/mono/testValidation.cpp)
add_executable(TestValidation ${TEST_VALIDATION_SOURCES})
target_link_libraries(TestValidation PRIVATE CarScraperLib)
carscraper_configure_target(TestValidation)
target_compile_definitions(TestValidation PRIVATE TESTING)

# test Entity
set(TEST_ENTITY_SOURCES tests/mono/testEntity.cpp)
add_executable(TestEntity ${TEST_ENTITY_SOURCES})
target_link_libraries(TestEntity PRIVATE CarScraperLib)
carscraper_configure_target(TestEntity)
target_compile_definitions(TestEntity PRIVATE TESTING)

# test Car
set(TEST_CAR_SOURCES tests/mono/testCar.cpp)
add_executable(TestCar ${TEST_CAR_SOURCES})
target_link_libraries(TestCar PRIVATE CarScraperLib)
carscraper_configure_target(TestCar)
target_compile_definitions(TestCar PRIVATE TESTING)

# testAll (aggregator)
set(TEST_ALL_SOURCES tests/testAll.cpp)
add_executable(TestAll ${TEST_ALL_SOURCES})
target_link_libraries(TestAll PRIVATE CarScraperLib)
carscraper_configure_target(TestAll)
target_compile_definitions(TestAll PRIVATE TESTING)
