set(PROJECT_NAME   "Controllers")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/DataServices/include
                           ${CMAKE_SOURCE_DIR}/Models/include)

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/cityController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/schoolController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/studentController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/classController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/testTypeController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/subjectController.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/src/studentSelectionController.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE CONTROLLERS_EXPORTS)
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
target_link_libraries(${PROJECT_NAME} DataServices ${CONAN_LIBS})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

add_subdirectory("test")
