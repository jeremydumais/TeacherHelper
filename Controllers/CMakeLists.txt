set(PROJECT_NAME   "Controllers")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/DataServices/include
                           ${CMAKE_SOURCE_DIR}/Models/include)

if(APPLE)
  set(CMAKE_MACOSX_RPATH OFF)
#elseif(UNIX AND NOT APPLE)
#  set(CMAKE_INSTALL_RPATH OFF)
endif()

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/databaseController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/cityController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/schoolController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/studentController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/classController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/testTypeController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/subjectController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/studentSelectionController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/assessmentController.cpp)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") #gcc
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    target_compile_options(${PROJECT_NAME}
      PRIVATE "-Wfatal-errors" # stops on first error
      "-Wall" # enables all the warnings about constructions
      "-Wextra" # Print extra warning messages"
      "-Werror=misleading-indentation" # Make all warnings into errors.
      "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
      "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
      "-ftrack-macro-expansion=0"
      "-fno-diagnostics-show-caret"
      "-Wno-misleading-indentation"
      )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME}
      PRIVATE "-Wfatal-errors" # stops on first error
      "-Wall" # enables all the warnings about constructions
      "-Wextra" # Print extra warning messages"
      "-Werror" # Make all warnings into errors.
      "-Wno-unused-lambda-capture" # FIX: It is necessary when building on MSVC
      "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
      "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
      )      

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275")
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE CONTROLLERS_EXPORTS
    INTERFACE NOMINMAX # avoid Win macro definition of min/max, use std one
    INTERFACE _SCL_SECURE_NO_WARNINGS) # disable security-paranoia warning
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE CONTROLLERS_EXPORTS)
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
target_link_libraries(${PROJECT_NAME} DataServices ${CONAN_LIBS})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

if (NOT DEFINED BUILD_TESTING OR BUILD_TESTING)
  add_subdirectory("test")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
