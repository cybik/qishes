cmake_minimum_required(VERSION 3.21)

project(gachasteam)

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)
set(CMAKE_BUILD_TYPE Release)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(
    gachasteam
    SHARED
        "${CMAKE_CURRENT_LIST_DIR}/src/gachasteam.cpp"
)

target_include_directories(
    gachasteam
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

# shim