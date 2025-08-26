cmake_minimum_required(VERSION 3.16)

project(NetDiskClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg)
set(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL3)

include_directories(${FFMPEG_DIR}/include)
include_directories(${SDL3_DIR}/include)

link_directories(${FFMPEG_DIR}/lib)
link_directories(${SDL3_DIR}/lib)


set(PROJECT_SOURCES
        main.cpp
        loginwidget.cpp
        loginwidget.h
        loginwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NetDiskClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mainwindow.h mainwindow.cpp mainwindow.ui
        resource.qrc
        json.hpp
        fileinfo.h fileinfo.cpp
        createdirdialog.h createdirdialog.cpp createdirdialog.ui
        transferwidget.h transferwidget.cpp transferwidget.ui
        progressbardelegate.h progressbardelegate.cpp
        filetrans.h filetrans.cpp
        threadtask.h threadtask.cpp
        listwidget.h listwidget.cpp listwidget.ui
        mytreeview.h mytreeview.cpp
        downloadtask.h downloadtask.cpp
        config.h
        cloudadddialog.h cloudadddialog.cpp cloudadddialog.ui
        generatesharelinkdlg.h generatesharelinkdlg.cpp generatesharelinkdlg.ui

        packetqueue.h packetqueue.cpp
        player.h player.cpp player.ui




    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NetDiskClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NetDiskClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NetDiskClient
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(NetDiskClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(NetDiskClient PRIVATE Qt6::Network)
target_link_libraries(NetDiskClient PRIVATE Qt6::Core)
target_link_libraries(NetDiskClient PRIVATE Qt6::Core)
target_link_libraries(NetDiskClient PRIVATE Qt6::Core)
target_link_libraries(NetDiskClient PRIVATE Qt6::Core)

target_include_directories(NetDiskClient PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NetDiskClient PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    avcodec
    avformat
    avutil
    avfilter
    swresample
    swscale
    SDL3
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NetDiskClient)
endif()
set_target_properties(NetDiskClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NetDiskClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NetDiskClient)
endif()
