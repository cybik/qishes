cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "vlvproton")
project(${MY_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(QT_VER "Qt6")

option(ASAN "Check memory" OFF)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

#find_package(Qt6 REQUIRED COMPONENTS Core)
#qt_standard_project_setup()

#find_package(
#    ${QT_VER}
#    REQUIRED
#    COMPONENTS
#        Core
#)

#include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/proton.cpp"
        "${PROJECT_SOURCE_DIR}/vlvproton.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/proton.h"
        "${PROJECT_SOURCE_DIR}/include/vlvproton.h"
)

add_library(
    ${MY_PROJECT_NAME}
    SHARED
        ${SOURCES_CPP}
        ${SOURCES_H}
)

#target_link_libraries(
#    ${MY_PROJECT_NAME}
#    PUBLIC
#        ${QT_VER}::Core
#)

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/3rdParty/termcolor/include"
)

# shim