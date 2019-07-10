cmake_minimum_required(VERSION 2.8.11)
project(cifxx CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

include(CompilerFlags)

add_library(cifxx INTERFACE)
target_include_directories(cifxx INTERFACE ${PROJECT_SOURCE_DIR})

if (${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
    enable_testing()
    add_subdirectory(tests)
endif()
