set(PROJECT_UNITTEST_NAME   "Models_unittests")

project(${PROJECT_UNITTEST_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_UNITTEST_NAME} ${INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/student_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/school_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/class_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/city_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/testType_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/subject_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/assessmentResult_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/assessment_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/version_unittest.cpp)

target_link_libraries(${PROJECT_UNITTEST_NAME} PRIVATE Models ${CONAN_LIBS})
gtest_discover_tests(${PROJECT_UNITTEST_NAME})