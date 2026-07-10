cmake_minimum_required(VERSION 3.26)
set(PROJECT_NAME csound-plugins)

# bump project version here 
project(csound-plugins LANGUAGES C VERSION 0.3.0)

# extend semver prerelease tag .. Ex. -alpha.0 -rc.1 etc. 
set(CSND_PLUGIN_VERSION "${PROJECT_VERSION}")

# generate and add to build tree
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/version.h"
    @ONLY
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSTITION_INDEPENDENT_CODE ON)

if (APPLE)
  message("Building on Apple.")
  add_compile_definitions(-DMACOS=1)
endif()

add_subdirectory(dsp)
add_subdirectory(src)
