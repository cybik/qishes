
set(PROJECT_3RDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdParty")

macro(PreconfigureApplicationRpath)
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
endmacro()

macro(PreconfigureLibraryRpath)
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
endmacro()

macro(ProcessLibraryForPackaging targetproj targetlib)
    add_custom_command(
        TARGET
            ${targetproj}
        POST_BUILD
        COMMAND
            echo "- Subproject Lib :: Processing $<TARGET_FILE:${targetlib}>" &&
            bash -c '[[ $<TARGET_FILE:${targetlib}> == *.a ]]'
            && echo "Static lib, skipping"
            || cp -Lf $<TARGET_FILE:${targetlib}> ${OUTDIR_FINAL_ASSEMBLAGE}
    )
endmacro()

macro (RestoreSONames targetproj)
    # From past life still
    add_custom_command(
        TARGET
            ${targetproj}
        POST_BUILD
        COMMAND
            echo "- Restoring SOnames and autocreating symlinks where needed"
            && bash ${CMAKE_CURRENT_SOURCE_DIR}/tools/restore_soname.sh ${OUTDIR_FINAL_ASSEMBLAGE}
    )
endmacro()

function(AutoconfigureApplicationRpath targetproj dependencies)
    add_custom_command(
        TARGET
            ${targetproj}
        POST_BUILD
        COMMAND
            mkdir -p "${OUTDIR_FINAL_ASSEMBLAGE_BASE}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/"
    )

    foreach(PROJECT_TO_INSTALL IN ITEMS ${dependencies})
        ProcessLibraryForPackaging("${targetproj}" "${PROJECT_TO_INSTALL}")
    endforeach()

    # Discord SDK library
    set(DISCORD_FORCED_LIB_FILE "${PROJECT_3RDPARTY_DIR}/discord-game-sdk/lib/linux/x86_64/libdiscord_game_sdk.so")
    target_link_libraries(
        ${targetproj}
        PRIVATE
            "${DISCORD_FORCED_LIB_FILE}"
    )

    foreach(PROJECT_TO_INSTALL IN ITEMS ${DISCORD_FORCED_LIB_FILE})
        add_custom_command(
            TARGET
                ${targetproj}
            POST_BUILD
            COMMAND
                echo "- Forced Lib :: Processing ${PROJECT_TO_INSTALL}" &&
                bash -c '[[ ${PROJECT_TO_INSTALL} == *.a ]]'
                    && echo "Static lib, skipping"
                    || cp -Lf ${PROJECT_TO_INSTALL} ${OUTDIR_FINAL_ASSEMBLAGE}
        )
    endforeach()

    # From past life still
    RestoreSONames("${targetproj}")

endfunction()

function(AutoconfigureLibraryRpath targetproj dependencies)
    set_target_properties(
        ${targetproj}
        PROPERTIES
            INSTALL_RPATH "\$ORIGIN"
    )
    add_custom_command(
        TARGET ${targetproj}
        POST_BUILD
            COMMAND
            mkdir -p "${OUTDIR_FINAL_ASSEMBLAGE_BASE}"
    )

    foreach(PROJECT_TO_INSTALL IN ITEMS ${dependencies})
        ProcessLibraryForPackaging("${targetproj}" "${PROJECT_TO_INSTALL}")
    endforeach()

    # From past life still
    RestoreSONames("${targetproj}")

endfunction()