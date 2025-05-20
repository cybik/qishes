cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "gachagames")
project(${MY_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(ASAN "Check memory" OFF)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Core
        Xml
)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/AGame.cpp"
        "${PROJECT_SOURCE_DIR}/util/HoyoMetadata.cpp"
        "${PROJECT_SOURCE_DIR}/AHoyoGame.cpp"

        "${PROJECT_SOURCE_DIR}/GameInfo.cpp"
        "${PROJECT_SOURCE_DIR}/Launcher.cpp"
        "${PROJECT_SOURCE_DIR}/GenshinImpact.cpp"
        "${PROJECT_SOURCE_DIR}/HonkaiStarRail.cpp"
        "${PROJECT_SOURCE_DIR}/HonkaiImpact3rd.cpp"
        "${PROJECT_SOURCE_DIR}/WutheringWaves.cpp"
        "${PROJECT_SOURCE_DIR}/ZenlessZoneZero.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/AGame.h"
        "${PROJECT_SOURCE_DIR}/include/AHoyoGame.h"
        "${PROJECT_SOURCE_DIR}/include/util/HoyoMetadata.h"
        "${PROJECT_SOURCE_DIR}/include/util/HoyoConstants.h"

        "${PROJECT_SOURCE_DIR}/include/GameInfo.h"
        "${PROJECT_SOURCE_DIR}/include/Launcher.h"
        "${PROJECT_SOURCE_DIR}/include/GenshinImpact.h"
        "${PROJECT_SOURCE_DIR}/include/HonkaiStarRail.h"
        "${PROJECT_SOURCE_DIR}/include/HonkaiImpact3rd.h"
        "${PROJECT_SOURCE_DIR}/include/WutheringWaves.h"
        "${PROJECT_SOURCE_DIR}/include/ZenlessZoneZero.h"
        #"${PROJECT_SOURCE_DIR}/include/util/effectors/IEffector.h"
        #"${PROJECT_SOURCE_DIR}/include/util/effectors/EnvEffector.h"
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
        Qt6::Xml
        Qt6::Core
        common
        httpclient
        cppcodec
        workarounds
        steamintegration
)

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/3rdParty/termcolor/include"
        "${CMAKE_SOURCE_DIR}/3rdParty/steamworks/public"
)

# shim
