cmake_minimum_required(VERSION 3.21)

project(gachasteam)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)
set(CMAKE_BUILD_TYPE Release)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/gachasteam.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/gachasteam.h"
)

add_library(
    gachasteam
    SHARED
        ${SOURCES_CPP}
        ${SOURCES_H}
)

target_include_directories(
    gachasteam
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# shim