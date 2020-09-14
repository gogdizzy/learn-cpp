cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(LEARN_CPP)

set(CMAKE_CXX_COMPILER "/usr/local/gcc-10.2.0/bin/g++-10.2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O2")
link_directories("/usr/local/gcc-10.2.0/lib")

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()

add_subdirectory(backtrace)
add_subdirectory(basic)
add_subdirectory(locale)
add_subdirectory(meta)
add_subdirectory(openmp)

