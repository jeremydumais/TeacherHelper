set(PROJECT_UNITTEST_NAME   "DataServices_unittests")

project(${PROJECT_UNITTEST_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_UNITTEST_NAME} ${INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/databaseConnection_unittest.cpp)

target_link_libraries(${PROJECT_UNITTEST_NAME} PRIVATE DataServices ${CONAN_LIBS})
add_test(AllTestsInMain ${PROJECT_UNITTEST_NAME})