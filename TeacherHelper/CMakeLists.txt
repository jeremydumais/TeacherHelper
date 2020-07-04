# Set project name
set(PROJECT_NAME    "TeacherHelperApp")

#set(CMAKE_BUILD_TYPE Debug)
include_directories("/usr/include/x86_64-linux-gnu/qt5"
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Models/include
    ${CMAKE_SOURCE_DIR}/DataServices/include)
    
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")


#Generate QT Form headers
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS AUTOGEN_BUILD_DIR/include)

set(PROJECT_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/mainForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/managementFormBase.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/studentManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/schoolManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/res/global.qrc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Models DataServices Qt5::Widgets sqlite3)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") #gcc
  # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  target_compile_options(${PROJECT_NAME}
    PRIVATE "-Wfatal-errors" # stops on first error
    "-Wall" # enables all the warnings about constructions
    "-Wextra" # Print extra warning messages"
    "-Werror=misleading-indentation" # Make all warnings into errors.
    "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
    "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
    "-ftrack-macro-expansion=0"
    "-fno-diagnostics-show-caret"
    "-Wno-misleading-indentation"
    )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(${PROJECT_NAME}
    PRIVATE "-Wfatal-errors" # stops on first error
    "-Wall" # enables all the warnings about constructions
    "-Wextra" # Print extra warning messages"
    "-Werror" # Make all warnings into errors.
    "-Wno-unused-lambda-capture" # FIX: It is necessary when building on MSVC
    "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
    "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
    )
endif()