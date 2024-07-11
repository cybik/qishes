cmake_minimum_required(VERSION 3.21)

set(MY_PROJECT_NAME "qgachawishes")

project(
    ${MY_PROJECT_NAME}
    VERSION
        0.0.1
    LANGUAGES
        CXX
)
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
#add_subdirectory("${PROJECT_3RDPARTY_DIR}/spinnerspp")

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

include_directories(
    BEFORE
        "${CMAKE_CURRENT_LIST_DIR}/src/include"
)

include_directories(
    AFTER
        "${PROJECT_3RDPARTY_DIR}/xdg/include"
        "${PROJECT_3RDPARTY_DIR}/yaml-cpp/include"
        "${PROJECT_3RDPARTY_DIR}/termcolor/include"
        "${PROJECT_3RDPARTY_DIR}/spinnerspp/include"
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

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
add_executable(
    ${MY_PROJECT_NAME}
        "${CMAKE_CURRENT_LIST_DIR}/src/qmain.cpp"

        "${CMAKE_CURRENT_LIST_DIR}/src/data/wishlog.cpp"

        "${CMAKE_CURRENT_LIST_DIR}/src/commands/DataCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/commands/DaemonCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/commands/HistoryCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/commands/LauncherCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/commands/AbstractCommand.cpp"
)

target_link_libraries(
    ${MY_PROJECT_NAME}
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
        #XdgUtils::BaseDir
        #QEasyDownloader
        #QArchive

        #"${PROJECT_SOURCE_DIR}/3rdParty/libsass/lib/libsass.a"
)

