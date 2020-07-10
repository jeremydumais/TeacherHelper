set(PROJECT_NAME   "Controllers")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/DataServices/include
                           ${CMAKE_SOURCE_DIR}/Models/include)

add_library(${PROJECT_NAME} SHARED src/cityController.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
target_link_libraries(${PROJECT_NAME} DataServices ${Boost_LIBRARIES})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

add_subdirectory("test")
