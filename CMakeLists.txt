cmake_minimum_required(VERSION 2.8.11)
project(pacif CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
cmake_policy(VERSION 2.8.11)
if (${CMAKE_VERSION} VERSION_GREATER 3.0.0)
    # OSX RPATH handling
    cmake_policy(SET CMP0042 OLD)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

include(CompilerFlags)

enable_testing()
add_subdirectory(tests)
