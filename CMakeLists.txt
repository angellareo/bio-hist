cmake_minimum_required(VERSION 2.8)
project(PezHist C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake.modules/")

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG -std=c++11")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

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
add_subdirectory(src)
