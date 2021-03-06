set(PROJECT_NAME   "DataServices")

project(${PROJECT_NAME} LANGUAGES CXX)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/Models/include)

if(APPLE)
    set(CMAKE_MACOSX_RPATH OFF)
endif()

add_library(${PROJECT_NAME} SHARED src/databaseConnection.cpp
                                   src/sqliteInsertOperation.cpp
                                   src/sqliteUpdateOperation.cpp
                                   src/sqliteSelectOperation.cpp
                                   src/sqliteDeleteOperation.cpp
                                   src/sqliteDDLOperation.cpp
                                   src/studentStorage.cpp
                                   src/sqliteOperationFactory.cpp
                                   src/schoolStorage.cpp
                                   src/classStorage.cpp
                                   src/cityStorage.cpp
                                   src/testTypeStorage.cpp
                                   src/subjectStorage.cpp
                                   src/sqliteDateTime.cpp
                                   src/sqliteDateTimeFactory.cpp
                                   src/assessmentStorage.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE DATASERVICES_EXPORTS)
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)

target_link_libraries(${PROJECT_NAME} Models ${CONAN_LIBS})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

if (NOT DEFINED BUILD_TESTING OR BUILD_TESTING)
  add_subdirectory("test/unit tests")
endif()

if (BUILD_INTEGRATION_TESTING)
  add_subdirectory("test/integration tests")
  message("-- Integration tests will be built")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
