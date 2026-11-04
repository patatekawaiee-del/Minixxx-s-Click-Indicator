
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ClickIndicator VERSION 1.0.0)

# Include all C++ source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# Create the mod
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Find Geode SDK
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK!")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# Load Geode SDK
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Configure the mod
setup_geode_mod(${PROJECT_NAME})

# Fix missing FMOD declarations in iOS bindings
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    if (TARGET GeodeBindings)
        target_compile_options(GeodeBindings PRIVATE
            "-include"
            "Geode/fmod/fmod.hpp"
        )
    endif()
endif()
