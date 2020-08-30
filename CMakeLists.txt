cmake_minimum_required(VERSION 3.10)

project(TeacherHelper C CXX)

if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" LOWERCASE_CMAKE_BUILD_TYPE)
if ("${LOWERCASE_CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    message("-- Debug mode selected")
elseif(LOWERCASE_CMAKE_BUILD_TYPE MATCHES "^(release)$")
    message("-- Release mode selected")
endif()
set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()
include(GoogleTest)

add_subdirectory("Models")
add_subdirectory("DataServices")
add_subdirectory("Controllers")
add_subdirectory("TeacherHelper")