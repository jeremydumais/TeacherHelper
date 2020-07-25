cmake_minimum_required(VERSION 3.10)

project(TeacherHelper C CXX)

if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

add_subdirectory("Models")
add_subdirectory("DataServices")
add_subdirectory("Controllers")
add_subdirectory("TeacherHelper")