project(qgachafs_libs)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libgachafs")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libsteamintegration")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libvlvproton")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/libqisheslauncher")

# shim