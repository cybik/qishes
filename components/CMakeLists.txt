project(qgachafs_libs)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libgachafs")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libcommon")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libvlvproton")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libqisheslauncher")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libworkarounds")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libsteamintegration")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libgachagames")

# shim