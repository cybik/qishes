cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "gachasteam")
project(${MY_PROJECT_NAME})

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

### LIBRARY VARIATION. Maybe adapt this into a .cmake include.

# Bringing in past experience / knowledge
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(OUTDIR_FINAL_ASSEMBLAGE_BASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(OUTDIR_FINAL_ASSEMBLAGE "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/")

# hi https://dev.my-gate.net/2021/08/04/understanding-rpath-with-cmake/
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

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

set_target_properties(
    ${MY_PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "\$ORIGIN"
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
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        mkdir -p "${OUTDIR_FINAL_ASSEMBLAGE_BASE}"
)

foreach(PROJECT_TO_INSTALL IN ITEMS vlvproton)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND
            echo "- Subproject Lib :: Processing $<TARGET_FILE:${PROJECT_TO_INSTALL}>" &&
            bash -c '[[ $<TARGET_FILE:${PROJECT_TO_INSTALL}> == *.a ]]'
                && echo "Static lib, skipping"
                || cp -Lf $<TARGET_FILE:${PROJECT_TO_INSTALL}> ${OUTDIR_FINAL_ASSEMBLAGE}
    )
endforeach()

# From past life still
add_custom_command(
    TARGET
        ${MY_PROJECT_NAME}
    POST_BUILD
    COMMAND
        echo "- Restoring SOnames and autocreating symlinks where needed"
            && bash ${CMAKE_CURRENT_SOURCE_DIR}/tools/restore_soname.sh ${OUTDIR_FINAL_ASSEMBLAGE}
)

target_include_directories(
    ${MY_PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# shim