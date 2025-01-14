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

include("${CMAKE_SOURCE_DIR}/cmake/RpathDefinitions.cmake")

add_subdirectory("${PROJECT_3RDPARTY_DIR}/yaml-cpp")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/termcolor")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/discord-game-sdk")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/qthttpclient")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/QAdwaitaDeco") # todo: needed or not?

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/components")

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
        "${PROJECT_3RDPARTY_DIR}/yaml-cpp/include"
        "${PROJECT_3RDPARTY_DIR}/termcolor/include"
        "${PROJECT_3RDPARTY_DIR}/spinnersplusplus"
        "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/include"
)

PreconfigureApplicationRpath()

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
        #QEasyDownloader
        #QArchive
        #qadwaitadecorations
        # Not Qt
        yaml-cpp
        gachafs
        gachasteam
        vlvproton
        httpclient
        discord-game-sdk-interface
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

list(
    APPEND
    APP_DEPENDENCIES
        "gachafs"
        "gachasteam"
        "vlvproton"
        "discord-game-sdk-interface"
        "qadwaitadecorations"
)

AutoconfigureApplicationRpath("${MY_PROJECT_NAME}" "${APP_DEPENDENCIES}")

install(
    TARGETS
        ${MY_PROJECT_NAME}
    RUNTIME
        DESTINATION bin
)

set(CPACK_GENERATOR ${CPACK_GENERATOR} "DEB")

include(CPack)