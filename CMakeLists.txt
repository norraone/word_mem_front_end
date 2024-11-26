cmake_minimum_required(VERSION 3.25)
project(impl_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

# Set Qt path
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.14.2/5.14.2/mingw73_64")
set(Qt5_DIR "C:/Qt/Qt5.14.2/5.14.2/mingw73_64/lib/cmake/Qt5")

# Find Qt packages
find_package(Qt5 COMPONENTS Core Gui Widgets Sql REQUIRED)

add_executable(impl_1 main.cpp
        mainwindow.cpp
        mainwindow.h
        customwidget.cpp
        customwidget.h
        userdatabase.cpp
        userdatabase.h)

target_link_libraries(impl_1 PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
)

# Deploy Qt dependencies
if(WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    
    # Add SQL plugins deployment
    if (EXISTS "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
    endif()

    foreach(QT_LIB Core Gui Widgets Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()

# Deploy Qt DLLs
include(${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Core/Qt5CoreConfigExtras.cmake)
get_target_property(Qt5_Core_Location Qt5::Core LOCATION)
get_filename_component(Qt5_Core_Path ${Qt5_Core_Location} PATH)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Qt5_Core_Path}/Qt5Core.dll ${Qt5_Core_Path}/Qt5Gui.dll ${Qt5_Core_Path}/Qt5Widgets.dll ${Qt5_Core_Path}/Qt5Sql.dll)
include(InstallRequiredSystemLibraries)
