cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "qgachawishes")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Cybik's Base")
set(CPACK_PACKAGE_NAME "${MY_PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT "root+gacha@cybik.moe")

project(
    ${MY_PROJECT_NAME}
    VERSION
        ${CPACK_PACKAGE_VERSION}
    LANGUAGES
        CXX
)
set(USE_QT6 ON)
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_3RDPARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdParty")

#add_subdirectory("${PROJECT_3RDPARTY_DIR}/xdg")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/yaml-cpp")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/termcolor")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/discord-game-sdk")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/qthttpclient")
#add_subdirectory("${PROJECT_3RDPARTY_DIR}/spinnerspp")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/components")

add_subdirectory("${PROJECT_3RDPARTY_DIR}/QAdwaitaDeco")

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Gui
        Xml
        Widgets
        WebEngineCore
        WebEngineWidgets
)
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

include_directories(
    BEFORE
        "${PROJECT_SOURCE_DIR}/include"
)

include_directories(
    AFTER
        #"${PROJECT_3RDPARTY_DIR}/xdg/include"
        "${PROJECT_3RDPARTY_DIR}/yaml-cpp/include"
        "${PROJECT_3RDPARTY_DIR}/termcolor/include"
        #"${PROJECT_3RDPARTY_DIR}/spinnerspp/include"
        "${PROJECT_3RDPARTY_DIR}/spinnersplusplus"
        "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/include"
)

# Bringing in past experience / knowledge
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(OUTDIR_FINAL_ASSEMBLAGE_BASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(OUTDIR_FINAL_ASSEMBLAGE "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/")

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

# Fun options
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

if(ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

set(
    SOURCES_CPP
        "${PROJECT_SOURCE_DIR}/qmain.cpp"

        "${PROJECT_SOURCE_DIR}/data/wishlog.cpp"

        "${PROJECT_SOURCE_DIR}/util/log.cpp"

        "${PROJECT_SOURCE_DIR}/commands/DataCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/DaemonCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/HistoryCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/LauncherCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/AbstractCommand.cpp"
)

# For AutoMOC
set(
    SOURCES_H
        "${PROJECT_SOURCE_DIR}/include/common.h"

        "${PROJECT_SOURCE_DIR}/include/data/wishlog.h"

        "${PROJECT_SOURCE_DIR}/include/util/log.h"

        "${PROJECT_SOURCE_DIR}/include/commands/DataCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/AbstractCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/HistoryCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/LauncherCommand.h"
        "${PROJECT_SOURCE_DIR}/include/commands/DaemonCommand.h"
)

add_executable(
    ${MY_PROJECT_NAME}
        ${SOURCES_CPP}
        ${SOURCES_H}
)

set_target_properties(
    ${MY_PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "\$ORIGIN/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
)

target_link_libraries(
    ${MY_PROJECT_NAME}
    PUBLIC
        # Qt
        Qt6::Gui
        Qt6::Xml
        Qt6::Core
        Qt6::Widgets
        Qt6::WebEngineCore
        Qt6::WebEngineWidgets
        # Not Qt
        yaml-cpp
        gachafs
        gachasteam
        vlvproton
        httpclient
        discord-game-sdk-interface
        #Ribbon
        #XdgUtils::BaseDir
        #QEasyDownloader
        #QArchive
        #qadwaitadecorations

        #"${PROJECT_SOURCE_DIR}/3rdParty/libsass/lib/libsass.a"
)

add_dependencies(
    ${MY_PROJECT_NAME}
        qadwaitadecorations
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        mkdir -p "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/"
)

foreach(PROJECT_TO_INSTALL IN ITEMS gachafs gachasteam vlvproton discord-game-sdk-interface qadwaitadecorations)
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

# Discord SDK library
set(DISCORD_FORCED_LIB_FILE "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/lib/linux/x86_64/libdiscord_game_sdk.so")
target_link_libraries(
    ${MY_PROJECT_NAME}
    PRIVATE
        "${DISCORD_FORCED_LIB_FILE}"
)

foreach(PROJECT_TO_INSTALL IN ITEMS ${DISCORD_FORCED_LIB_FILE})
    add_custom_command(
        TARGET
            ${MY_PROJECT_NAME}
        POST_BUILD
        COMMAND
            echo "- Forced Lib :: Processing ${PROJECT_TO_INSTALL}" &&
            bash -c '[[ ${PROJECT_TO_INSTALL} == *.a ]]'
                && echo "Static lib, skipping"
                || cp -Lf ${PROJECT_TO_INSTALL} ${OUTDIR_FINAL_ASSEMBLAGE}
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

install(
    TARGETS
        ${MY_PROJECT_NAME}
    RUNTIME
        DESTINATION bin
)

set(CPACK_GENERATOR ${CPACK_GENERATOR} "DEB")

include(CPack)