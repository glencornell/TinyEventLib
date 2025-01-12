cmake_minimum_required(VERSION 3.25)
project(QtLite)

# Define custom build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profiling;Coverage;MemCheck" CACHE STRING "Build types" FORCE)

# Ensure CMake respects custom build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE)
endif()

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the custom target for all unit and integration tests
enable_testing()
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --timeout 20s
    COMMENT "Running all tests with output on failure and a timeout of 20 seconds"
  )

# Create global compiler and linker options
set(GLOBAL_COVERAGE_COMPILE_OPTIONS -O0 --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g)
include(cmake/coverage.cmake)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
