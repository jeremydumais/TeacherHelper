cmake_minimum_required(VERSION 3.10)

project(TeacherHelper C CXX)

if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# Download and unpack fmt at configure time
include(ExternalProject)
ExternalProject_Add(fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  #CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CPPFLAGS=-fPIC ${CMAKE_SOURCE_DIR}/../fmtlib-build --prefix=${LIB_OUTPUT}
  EXCLUDE_FROM_ALL TRUE
  BUILD_COMMAND $(MAKE) fmt
  STEP_TARGETS build
  CMAKE_ARGS
  "-DCMAKE_POSITION_INDEPENDENT_CODE=ON")
set(fmtlib_BINARY_DIR "${CMAKE_BINARY_DIR}/fmtlib-prefix/src/fmtlib-build")
set(fmtlib_SOURCE_DIR "${CMAKE_BINARY_DIR}/fmtlib-prefix/src/fmtlib/include")

enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif() 

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

add_subdirectory("Models")
add_subdirectory("DataServices")
add_subdirectory("Controllers")
add_subdirectory("TeacherHelper")