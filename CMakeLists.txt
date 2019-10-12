cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(LEARN_CPP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(memory_layout_test memory_layout.cpp)
add_executable(stl_test stl.cpp)
add_executable(syntax_test syntax.cpp)
add_executable(exception_test exception.cpp)
add_executable(io_test io.cpp)

