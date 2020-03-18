cmake_minimum_required(VERSION 3.9)
project(bio-bin C CXX)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
add_subdirectory(thirdparty/benchmark)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake.modules/")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

set ( LIBRARY_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/lib)
set ( EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/bin)

find_package(Boost 1.53.0 REQUIRED)
message (${Boost_INCLUDE_DIR})
find_package(HDF5 REQUIRED COMPONENTS CXX)
find_library(STATS NAMES stats PATHS ./lib)
if (STATS)
	message( STATUS "Found Stats" )
endif (STATS)
find_package(YamlCpp REQUIRED)

add_subdirectory(wordsBuffer)    
add_subdirectory(libStats)
add_subdirectory(thirdparty/catch)
add_subdirectory(src)
#add_subdirectory(test)
