cmake_minimum_required(VERSION 3.21)

project(gachafs)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
        "${CMAKE_CURRENT_LIST_DIR}/../../3rdParty/termcolor/include"
)
set(CMAKE_BUILD_TYPE Release)

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Gui
        Xml
)


set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(
    gachafs
    SHARED
        "${CMAKE_CURRENT_LIST_DIR}/src/gachafs.cpp"
)

target_link_libraries(
    gachafs
        Qt6::Gui
        Qt6::Xml
        Qt6::Core
)

target_include_directories(
    gachafs
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

# shim