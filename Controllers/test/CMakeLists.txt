set(PROJECT_UNITTEST_NAME   "Controllers_unittests")

project(${PROJECT_UNITTEST_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(${PROJECT_UNITTEST_NAME} ${INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cityController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/schoolController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/classController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/studentController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/testTypeController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/subjectController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/studentSelectionController_unittest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/assessmentController_unittest.cpp)

target_link_libraries(${PROJECT_UNITTEST_NAME} PRIVATE Controllers ${CONAN_LIBS})
add_test(${PROJECT_UNITTEST_NAME} ${CMAKE_BINARY_DIR}/bin/${PROJECT_UNITTEST_NAME})