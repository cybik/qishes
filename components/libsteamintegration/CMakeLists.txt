cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "steamintegration")
project(${MY_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VER "Qt6")

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

include_directories(
    AFTER
        "${PROJECT_3RDPARTY_DIR}/steamworks/public"
)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

find_package(
    ${QT_VER}
    REQUIRED
    COMPONENTS
        Core
)

include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

include_directories()

PreconfigureLibraryRpath()

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/steam_proton.cpp"
        "${PROJECT_SOURCE_DIR}/steam_integration.cpp"
        "${PROJECT_SOURCE_DIR}/steam_environment.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/steam_proton.h"
        "${PROJECT_SOURCE_DIR}/include/steam_integration.h"
        "${PROJECT_SOURCE_DIR}/include/steam_environment.h"
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
        vlvproton
        ${QT_VER}::Core
)

add_dependencies(
    ${MY_PROJECT_NAME}
        vlvproton
)

list(
    APPEND
    DEPENDENCIES_LIST
        "vlvproton"
)

AutoconfigureLibraryRpath( "${MY_PROJECT_NAME}" "${DEPENDENCIES_LIST}" )

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# shim