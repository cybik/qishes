cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "gachafs")
project(${MY_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
        "${CMAKE_CURRENT_LIST_DIR}/../../3rdParty/termcolor/include"
)

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Gui
        Xml
)


set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/gachafs.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/gachafs.h"
        "${PROJECT_SOURCE_DIR}/include/egachafs.h"
)

add_library(
    ${MY_PROJECT_NAME}
    SHARED
        ${SOURCES_CPP}
        ${SOURCES_H}
)

target_link_libraries(
    ${MY_PROJECT_NAME}
        Qt6::Gui
        Qt6::Xml
        Qt6::Core
)

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# shim