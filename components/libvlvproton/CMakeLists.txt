cmake_minimum_required(VERSION 3.21)

project(vlvproton)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)
set(CMAKE_BUILD_TYPE Release)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/vlvproton.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/vlvproton.h"
)

add_library(
    vlvproton
    SHARED
        ${SOURCES_CPP}
        ${SOURCES_H}
)

target_include_directories(
    vlvproton
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# shim