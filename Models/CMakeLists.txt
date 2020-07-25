set(PROJECT_NAME   "Models")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED src/city.cpp
                                   src/school.cpp
                                   src/class.cpp
                                   src/student.cpp
                                   src/testType.cpp
                                   src/subject.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

add_subdirectory("test")
