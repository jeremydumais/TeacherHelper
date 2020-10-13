set(PROJECT_UNITTEST_NAME   "DataServices_unittests")

project(${PROJECT_UNITTEST_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_UNITTEST_NAME} ${INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/databaseConnection_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sqliteDateTime_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sqliteDateTimeFactory_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cityStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/schoolStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/studentStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/classStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/subjectStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/testTypeStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/assessmentStorage_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/databaseVersionStorage_unittest.cpp)


target_link_libraries(${PROJECT_UNITTEST_NAME} PRIVATE DataServices ${CONAN_LIBS})
gtest_discover_tests(${PROJECT_UNITTEST_NAME})