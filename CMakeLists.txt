cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "qgachawishes")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Cybik's Base")
set(CPACK_PACKAGE_NAME "${MY_PROJECT_NAME}")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "x86_64")
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

add_subdirectory("${PROJECT_3RDPARTY_DIR}/xdg")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/yaml-cpp")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/termcolor")
add_subdirectory("${PROJECT_3RDPARTY_DIR}/discord-game-sdk")
#add_subdirectory("${PROJECT_3RDPARTY_DIR}/spinnerspp")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/components")

#add_subdirectory("${PROJECT_3RDPARTY_DIR}/QAdwaitaDeco")

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
        "${PROJECT_3RDPARTY_DIR}/xdg/include"
        "${PROJECT_3RDPARTY_DIR}/yaml-cpp/include"
        "${PROJECT_3RDPARTY_DIR}/termcolor/include"
        "${PROJECT_3RDPARTY_DIR}/spinnerspp/include"
        "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/include"
)

# hi https://dev.my-gate.net/2021/08/04/understanding-rpath-with-cmake/
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable(
    ${MY_PROJECT_NAME}
        "${PROJECT_SOURCE_DIR}/qmain.cpp"

        "${PROJECT_SOURCE_DIR}/data/wishlog.cpp"

        "${PROJECT_SOURCE_DIR}/util/log.cpp"

        "${PROJECT_SOURCE_DIR}/commands/DataCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/DaemonCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/HistoryCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/LauncherCommand.cpp"
        "${PROJECT_SOURCE_DIR}/commands/AbstractCommand.cpp"
)

target_link_libraries(
    ${MY_PROJECT_NAME}
    PUBLIC
        yaml-cpp
        Qt6::Gui
        Qt6::Xml
        Qt6::Core
        Qt6::Widgets
        Qt6::WebEngineCore
        Qt6::WebEngineWidgets
        gachafs
        gachasteam
        #Ribbon
        discord-game-sdk-interface
        #XdgUtils::BaseDir
        #QEasyDownloader
        #QArchive
        #QAdwaitaDecorations::qadwaitadecorations

        #"${PROJECT_SOURCE_DIR}/3rdParty/libsass/lib/libsass.a"
)

target_link_libraries(${MY_PROJECT_NAME} PRIVATE
    "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/lib/linux/x86_64/libdiscord_game_sdk.so"
)

install(
    TARGETS
        ${MY_PROJECT_NAME}
    RUNTIME
        DESTINATION bin
)

include(CPack)