set(PROJECT_NAME   "Models")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(APPLE)
    set(CMAKE_MACOSX_RPATH OFF)
endif()
add_library(${PROJECT_NAME} SHARED src/city.cpp
                                   src/school.cpp
                                   src/class.cpp
                                   src/student.cpp
                                   src/testType.cpp
                                   src/subject.cpp
                                   src/assessmentResult.cpp
                                   src/assessment.cpp
                                   src/version.cpp)
     
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
    target_compile_definitions(${PROJECT_NAME}
      PRIVATE MODELS_EXPORTS
      INTERFACE NOMINMAX # avoid Win macro definition of min/max, use std one
      INTERFACE _SCL_SECURE_NO_WARNINGS) # disable security-paranoia warning
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

if (NOT DEFINED BUILD_TESTING OR BUILD_TESTING)
  add_subdirectory("test")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
