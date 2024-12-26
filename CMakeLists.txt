cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "qgachawishes")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Cybik's Base")
set(CPACK_PACKAGE_NAME "${MY_PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT "root+gacha@cybik.moe")

project(
    ${MY_PROJECT_NAME}
    VERSION
        ${CPACK_PACKAGE_VERSION}
    LANGUAGES
        CXX
)
set(USE_QT6 ON)

option(ASAN "Check memory" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VER "Qt6")

include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

set(SARIBBON_BUILD_EXAMPLES OFF CACHE BOOL "Exported" FORCE)
set(SARIBBON_USE_FRAMELESS_LIB ON CACHE BOOL "Exported" FORCE)
#set(ASAN ON CACHE BOOL "Check memory" FORCE)
#set(QWINDOWKIT_INSTALL OFF CACHE BOOL "Exported" FORCE)
#set(QWindowKit_DIR "${PROJECT_3RDPARTY_DIR}/qwindowkit/src" CACHE STRING "Exported" FORCE)
#set(QWindowKit_DIR "${PROJECT_BIN_3RDPARTY_DIR}/qwindowkit/src" CACHE STRING "Exported" FORCE)

add_subdirectory("${PROJECT_3RDPARTY_DIR}")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/components")

find_package(
    ${QT_VER}
    REQUIRED
    COMPONENTS
        Gui
        Xml
        Widgets
        WebEngineCore
        WebEngineWidgets
)
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

include_directories(
    BEFORE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/3rdParty/libsass/include"
)

include_directories(
    AFTER
        "${PROJECT_3RDPARTY_DIR}/yaml-cpp/include"
        "${PROJECT_3RDPARTY_DIR}/termcolor/include"
        "${PROJECT_3RDPARTY_DIR}/spinnersplusplus"
        #"${PROJECT_3RDPARTY_DIR}/discord-rpc-classic/include"
        "${PROJECT_3RDPARTY_DIR}/discord-rpc-classic-bin/include"
        "${PROJECT_3RDPARTY_DIR}/qwindowkit/src"
        "${PROJECT_3RDPARTY_DIR}/saribbon/src"
        #"${PROJECT_3RDPARTY_DIR}/discord-game-sdk/include"
        "${PROJECT_3RDPARTY_DIR}/steamworks/public"
)

PreconfigureApplicationRpath()

if(ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/qmain.cpp"

        "${PROJECT_SOURCE_DIR}/data/wishlog.cpp"

        "${PROJECT_SOURCE_DIR}/util/log.cpp"
        "${PROJECT_SOURCE_DIR}/util/Discord.cpp"

        "${PROJECT_SOURCE_DIR}/commands/DataCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/DaemonCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/HistoryCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/LauncherCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/AbstractCommand.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/common.h"

        "${PROJECT_SOURCE_DIR}/include/data/wishlog.h"

        "${PROJECT_SOURCE_DIR}/include/util/log.h"
        "${PROJECT_SOURCE_DIR}/include/util/Discord.h"

        "${PROJECT_SOURCE_DIR}/include/commands/DataCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/AbstractCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/HistoryCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/LauncherCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/DaemonCommand.h"
)

add_executable(
    ${MY_PROJECT_NAME}
        ${SOURCES_CPP}
        ${SOURCES_H}
)

set_target_properties(
    ${MY_PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "\$ORIGIN/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
)

target_link_libraries(
    ${MY_PROJECT_NAME}
    PUBLIC
        # Qt
        ${QT_VER}::Gui
        ${QT_VER}::Xml
        ${QT_VER}::Core
        ${QT_VER}::Widgets
        ${QT_VER}::WebEngineCore
        ${QT_VER}::WebEngineWidgets
        #qlementine
        #QEasyDownloader
        #QArchive
        #qadwaitadecorations
        # Not Qt
        yaml-cpp
        gachafs
        steamintegration
        vlvproton
        httpclient
        qisheslauncher
        #discord-rpc
        #discord-game-sdk-interface
        QWKCore
        QWKWidgets

        SARibbonBar::SARibbonBar

        # todo: fix this bloody thing tbh
        "${PROJECT_3RDPARTY_DIR}/libsass/lib/libsass.a"
        "${PROJECT_3RDPARTY_DIR}/discord-rpc-classic-bin/lib/libdiscord-rpc.so"
        "${PROJECT_3RDPARTY_DIR}/steamworks/redistributable_bin/linux64/libsteam_api.so"
)

add_dependencies(
    ${MY_PROJECT_NAME}
        qadwaitadecorations
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        mkdir -p "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/"
)

list(
    APPEND
    APP_DEPENDENCIES
        "gachafs"
        "steamintegration"
        "qisheslauncher"
        "vlvproton"
        #"discord-rpc"
        #"discord-game-sdk-interface"
        "qadwaitadecorations"
        "QWKCore"
        "QWKWidgets"
        "SARibbonBar::SARibbonBar"
)

AutoconfigureApplicationRpath("${MY_PROJECT_NAME}" "${APP_DEPENDENCIES}")

install(
    TARGETS
        ${MY_PROJECT_NAME}
    RUNTIME
        DESTINATION bin
)

set(CPACK_GENERATOR ${CPACK_GENERATOR} "DEB")

include(CPack)

# Unit tests!
add_executable(
    "${MY_PROJECT_NAME}-test"
)

target_link_libraries(
    "${MY_PROJECT_NAME}-test"
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests("${MY_PROJECT_NAME}-test")
