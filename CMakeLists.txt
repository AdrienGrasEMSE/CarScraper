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


# --- OpenXLSX (compilée — FetchContent obligatoire) ---
FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY  https://github.com/troldal/OpenXLSX.git
    GIT_TAG         master
)
FetchContent_MakeAvailable(OpenXLSX)


# --- libcurl (bibliothèque système) ---
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

    # Core — Models
    src/core/models/Car.cpp
    src/core/models/Entity.cpp

    # Core — Logger
    src/core/Logger/Logger.cpp

    # IO — HttpClient
    src/io/HttpClient/HttpClient.cpp
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

# Remplacer le bloc Tests existant par :
set(TEST_ENTITY_SOURCES
    tests/test_Entity.cpp
    src/core/models/Entity.cpp
    src/core/Logger/Logger.cpp
)
add_executable(TestEntity ${TEST_ENTITY_SOURCES})
carscraper_configure_target(TestEntity)

# Garder le test HttpClient existant
set(TEST_SOURCES
    tests/test_HttpClient.cpp
    src/io/HttpClient/HttpClient.cpp
)
add_executable(CarScraperTests ${TEST_SOURCES})
carscraper_configure_target(CarScraperTests)