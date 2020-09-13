set(PROJECT_INTEGRATIONTEST_NAME   "DataServices_integrationtests")

project(${PROJECT_INTEGRATIONTEST_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_INTEGRATIONTEST_NAME} ${INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cityCRUD_integrationtest.cpp)

target_link_libraries(${PROJECT_INTEGRATIONTEST_NAME} PRIVATE DataServices ${CONAN_LIBS})
gtest_discover_tests(${PROJECT_INTEGRATIONTEST_NAME})