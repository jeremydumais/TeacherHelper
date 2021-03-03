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
                              
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
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
