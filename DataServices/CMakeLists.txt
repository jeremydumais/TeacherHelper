set(PROJECT_NAME   "DataServices")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/Models/include)

add_library(${PROJECT_NAME} SHARED src/databaseConnection.cpp
                                   src/sqliteInsertOperation.cpp
                                   src/sqliteUpdateOperation.cpp
                                   src/sqliteSelectOperation.cpp
                                   src/sqliteDeleteOperation.cpp
                                   src/sqliteDDLOperation.cpp
                                   src/studentStorage.cpp
                                   src/schoolStorage.cpp
                                   src/classStorage.cpp
                                   src/cityStorage.cpp
                                   src/testTypeStorage.cpp
                                   src/subjectStorage.cpp)
add_dependencies(${PROJECT_NAME} fmtlib-build)
#set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME} PUBLIC ${fmtlib_SOURCE_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)
#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} Models sqlite3 ${Boost_LIBRARIES} ${fmtlib_BINARY_DIR}/libfmt.a)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

add_subdirectory("test")