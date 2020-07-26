# Set project name
set(PROJECT_NAME    "TeacherHelperApp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Models/include
    ${CMAKE_SOURCE_DIR}/DataServices/include
    ${CMAKE_SOURCE_DIR}/Controllers/include
    ${QT_INCLUDE_PATH})

if ("${CMAKE_QT_PATH}" STREQUAL "")
  message("No QT path submitted")
  find_package(Qt5 COMPONENTS Widgets REQUIRED)
else()
  find_package(Qt5 COMPONENTS Widgets REQUIRED PATHS ${CMAKE_QT_PATH}/lib/cmake/Qt5)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

#Generate QT Form headers
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS AUTOGEN_BUILD_DIR/include)

set(PROJECT_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/configurationManager.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/specialFolders.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/mainForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/managementFormBase.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/studentManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/schoolManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/classManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/cityManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/testTypeManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/subjectManagementForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/studentSelectionForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/aboutBoxForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/src/imageAttributionsForm.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/res/darkstyle/darkstyle.qrc
                         ${CMAKE_CURRENT_SOURCE_DIR}/res/global.qrc
                         ${CMAKE_CURRENT_SOURCE_DIR}/res/global.rc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
if (WIN32)
  set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
endif()
target_link_libraries(${PROJECT_NAME} Models DataServices Controllers Qt5::Widgets sqlite3)


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
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(${PROJECT_NAME}
      PUBLIC /wdC4251
      INTERFACE NOMINMAX # avoid Win macro definition of min/max, use std one
      INTERFACE _SCL_SECURE_NO_WARNINGS) # disable security-paranoia warning
endif()

# Copy styles and resource images
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/darkstyle/darkstyle.qss ${CMAKE_BINARY_DIR}/bin/res/darkstyle/darkstyle.qss COPYONLY)
add_custom_target(copy-darkstyle-res ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res/darkstyle/rc
    ${CMAKE_BINARY_DIR}/bin/res/darkstyle/rc)

if (WIN32)
    # On Windows copy QT required dll to the binary dir
    if (LOWERCASE_CMAKE_BUILD_TYPE MATCHES "^(debug)$" OR LOWERCASE_CMAKE_BUILD_TYPE STREQUAL "")
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Cored.dll ${CMAKE_BINARY_DIR}/bin/Qt5Cored.dll COPYONLY)
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Guid.dll ${CMAKE_BINARY_DIR}/bin/Qt5Guid.dll COPYONLY)
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Widgetsd.dll ${CMAKE_BINARY_DIR}/bin/Qt5Widgetsd.dll COPYONLY)
    elseif(LOWERCASE_CMAKE_BUILD_TYPE MATCHES "^(release)$")
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Core.dll ${CMAKE_BINARY_DIR}/bin/Qt5Core.dll COPYONLY)
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Gui.dll ${CMAKE_BINARY_DIR}/bin/Qt5Gui.dll COPYONLY)
        configure_file(${CMAKE_QT_PATH}/bin/Qt5Widgets.dll ${CMAKE_BINARY_DIR}/bin/Qt5Widgets.dll COPYONLY)
    endif()
    add_custom_target(copy-qt-platform-package ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_QT_PATH}/plugins
        ${CMAKE_BINARY_DIR}/bin/plugins)
endif()
