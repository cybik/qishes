cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "gachasteam")
project(${MY_PROJECT_NAME})

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

PreconfigureLibraryRpath()

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/gachasteam.cpp"
        "${PROJECT_SOURCE_DIR}/steam_environment.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/gachasteam.h"
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