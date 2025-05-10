cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "common")
project(${MY_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(USE_QT6 ON)
#set(LOC_QT_VER "6.7.2")
#set(LOC_QT_VER "6.9.0")
find_package(Qt6
    REQUIRED
    COMPONENTS
        Core
)
set(QT_VER "Qt6")
find_package(
    ${QT_VER}
    REQUIRED
)

option(ASAN "Check memory" OFF)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/util/log.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/util/log.h"
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
        ${QT_VER}::Core
)

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/3rdParty/termcolor/include"
)

# shim
