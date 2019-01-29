cmake_minimum_required(VERSION 3.11)

project(cpp_examples)

set(cpp_examples_BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${cpp_examples_BIN_DIR})

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tests)
