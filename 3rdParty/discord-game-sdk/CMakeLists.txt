cmake_minimum_required(VERSION 3.0)
set(MY_PROJECT_NAME "discord-game-sdk-interface")
project(
    ${MY_PROJECT_NAME}
    VERSION
        0.0.1
    LANGUAGES
        CXX
)
set(PROJECT_DESCRIPTION "Discord Game SDK")
set(PROJECT_HOMEPAGE_URL https://discord.com/developers/docs/game-sdk/)

## Global variables

# Request C++-11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB
    DGSDK_SRC
        "${CMAKE_CURRENT_LIST_DIR}/cpp/*.cpp"
)
include_directories(
    AFTER
        "${CMAKE_CURRENT_LIST_DIR}/include"
)

# Sources
add_library(
    ${MY_PROJECT_NAME}
    SHARED
        ${DGSDK_SRC}
)