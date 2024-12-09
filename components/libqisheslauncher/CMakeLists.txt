cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "qisheslauncher")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Cybik's Base")
set(CPACK_PACKAGE_NAME "${MY_PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT "root+gacha@cybik.moe")

project(${MY_PROJECT_NAME})
project(
    ${MY_PROJECT_NAME}
    VERSION
        ${CPACK_PACKAGE_VERSION}
    LANGUAGES
        CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VER "Qt6")

option(ASAN "Check memory" OFF)

include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

PreconfigureLibraryRpath()

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

include_directories(
    BEFORE
        "${CMAKE_SOURCE_DIR}/3rdParty/libsass/include"
        "${PROJECT_INCLUDE_DIR}"
)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

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

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/data/SettingsData.cpp"
        "${PROJECT_SOURCE_DIR}/resources/SASSProcess.cpp"

        "${PROJECT_SOURCE_DIR}/ui/Landing.cpp"
        "${PROJECT_SOURCE_DIR}/ui/Splash.cpp"

        "${PROJECT_SOURCE_DIR}/process/process.cpp"
        "${PROJECT_SOURCE_DIR}/process/launch.cpp"
        "${PROJECT_SOURCE_DIR}/data/remote/runners/Proton.cpp"
        "${PROJECT_SOURCE_DIR}/data/remote/runners/Wine.cpp"
        "${PROJECT_SOURCE_DIR}/data/remote/runners/RunnerCommon.cpp"
        "${PROJECT_SOURCE_DIR}/data/remote/RemoteCommon.cpp"
        "${PROJECT_SOURCE_DIR}/data/remote/game/GameDL.cpp"
        "${PROJECT_SOURCE_DIR}/data/Settings.cpp"
)

# For AutoMOC, mostly
set(
    SOURCES_H
        "${PROJECT_INCLUDE_DIR}/about.h"
        "${PROJECT_INCLUDE_DIR}/data/SettingsData.h"

        "${PROJECT_INCLUDE_DIR}/resources/gear_image.h"
        "${PROJECT_INCLUDE_DIR}/resources/stylesheets.h"
        "${PROJECT_INCLUDE_DIR}/resources/SASSProcess.h"

        "${PROJECT_INCLUDE_DIR}/util/Locales.h"
        "${PROJECT_INCLUDE_DIR}/util/Constants.h"
        "${PROJECT_INCLUDE_DIR}/ui/Landing.h"
        "${PROJECT_INCLUDE_DIR}/ui/Splash.h"

        "${PROJECT_INCLUDE_DIR}/process/process.h"
        "${PROJECT_INCLUDE_DIR}/process/launch.h"
        "${PROJECT_INCLUDE_DIR}/data/remote/runners/Proton.h"
        "${PROJECT_INCLUDE_DIR}/data/remote/runners/Wine.h"
        "${PROJECT_INCLUDE_DIR}/data/remote/runners/RunnerCommon.h"
        "${PROJECT_INCLUDE_DIR}/data/remote/RemoteCommon.h"
        "${PROJECT_INCLUDE_DIR}/data/remote/game/GameDL.h"
        "${PROJECT_INCLUDE_DIR}/data/Settings.h"
)
set(
    SOURCES_SETTINGS_CPP
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabs.h
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsWindow.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTab.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabButtonBack.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabGeneral.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabEnhancements.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabRunner.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabMore.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabGame.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsGroup.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsButton.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsLabel.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsCombo.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsControl.cpp
        #${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsCheckbox.cpp
)
set(
    SOURCES_SETTINGS_h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsWindow.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTab.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabButtonBack.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabGeneral.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabEnhancements.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabRunner.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabMore.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/SettingsTabGame.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsGroup.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsButton.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsLabel.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsCombo.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsControl.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsCheckbox.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsControl.h
        #    ${PROJECT_SOURCE_DIR}/ui/settings/util/SettingsControls.h

)

add_library(
    ${MY_PROJECT_NAME}
    SHARED
        ${SOURCES_CPP}
        ${SOURCES_H}
)

target_link_libraries(
    ${MY_PROJECT_NAME}
    PUBLIC
        ${QT_VER}::Gui
        ${QT_VER}::Xml
        ${QT_VER}::Core
        ${QT_VER}::Widgets
        ${QT_VER}::WebEngineCore
        ${QT_VER}::WebEngineWidgets
        # Should be at the root of this whole thing.
        vlvproton
        steamintegration
        yaml-cpp
)

AutoconfigureLibraryRpath( "${MY_PROJECT_NAME}" "${DEPENDENCIES_LIST}" )

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_INCLUDE_DIR}"
)

# shim