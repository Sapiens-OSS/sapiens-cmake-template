cmake_minimum_required(VERSION 3.23)

set(MOD_ID "SapiensModTemplate" CACHE STRING "Internal mod id, used as name for mod files.")
set(PREVIEW_FILE "preview.png" CACHE FILEPATH "Preview image for the mod")

file(READ "modInfo.lua" MODINFO)
if (MODINFO MATCHES "name[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"")
    string(REGEX REPLACE "[ \t\r\n]" "-" MOD_ID_NO_WHITESPACE "${CMAKE_MATCH_1}")
    string(REGEX REPLACE "[^a-zA-Z0-9+_\-]" "" MOD_ID_SANITIZED "${MOD_ID_NO_WHITESPACE}")
    string(TOLOWER "${MOD_ID_SANITIZED}" MOD_ID)
else ()
    message(FATAL_ERROR "Could not extract the mod ID from modInfo.lua. Ensure that you have set the `name` property in modInfo.lua to a valid string.")
endif ()

message(STATUS "Using ${MOD_ID} as ID")

# Copy the mod to the game's mods folder
set(AUTO_COPY_MOD OFF CACHE BOOL "When ON attempt to copy the mod to the sapiens mod directory. THIS DELETES ALL FILES IN THE TARGET DIRECTORY.")
set(SAPIENS_MOD_DIRECTORY "" CACHE PATH "The directory sapiens loads mods from. THIS DELETES ALL FILES IN THE TARGET DIRECTORY.")

mark_as_advanced(MOD_TYPE) # Should not be changed for c mods as c mods are used for world specific things only.

project(${MOD_ID} C)

set(CMAKE_C_STANDARD 99)

# Add your source files here
set(_SOURCES src/height.c src/biome.c)
add_library(${MOD_ID} SHARED ${_SOURCES})

# ==================== DO NOT CHANGE ANYTHING BELOW THIS LINE ====================
# Mod devs should not need to change anything below this line. Only change if you know what you are doing.
# ==================== DO NOT CHANGE ANYTHING BELOW THIS LINE ====================

# Include sapiens headers and libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/splugins/include)

find_library(SP_LIBRARY SPCommon PATHS ${CMAKE_CURRENT_SOURCE_DIR}/libs/splugins/lib REQUIRED NO_CMAKE_FIND_ROOT_PATH)
mark_as_advanced(SP_LIBRARY) # Change SP_LIBRARY only if you want to use a custom compiled version of the sapiens plugin library, eg when the public repo is outdated.
target_link_libraries(${MOD_ID} ${SP_LIBRARY})

set(_mod_dir "${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}")

# Apply the settings from cmake to the modInfo.lua
configure_file(modInfo.lua "${_mod_dir}/modInfo.lua" @ONLY)

# Copy the preview file to the mod folder
cmake_path(GET PREVIEW_FILE FILENAME _preview_filename)
cmake_path(ABSOLUTE_PATH PREVIEW_FILE)
add_custom_command(TARGET ${MOD_ID} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PREVIEW_FILE}
        ${_mod_dir}/${_preview_filename}
        DEPENDS ${PREVIEW_FILE})

add_custom_target(sync_mod_files ALL
        DEPENDS
        always_rebuild
)

add_custom_command(
        OUTPUT always_rebuild
        COMMAND cmake -E echo "Syncing files from lua/ models/ and localizations/"
)
add_dependencies(${MOD_ID} sync_mod_files)

function(sync_directory_on_build directory)
    cmake_path(ABSOLUTE_PATH directory OUTPUT_VARIABLE _absolute_directory)
    set(_target "${_mod_dir}/${directory}")
    message(STATUS "Syncing ${_absolute_directory} to ${_target}")
    add_custom_command(
            TARGET sync_mod_files POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Syncing ${directory}"
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${_target}
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${_absolute_directory}
            ${_target}
            COMMENT "Syncing ${directory}")
endfunction()

# Copy the localizations, models and scripts to the mod folder
sync_directory_on_build("localizations")
sync_directory_on_build("models")
sync_directory_on_build("scripts")

# Copy the build mod library to the mod folder
add_custom_command(TARGET ${MOD_ID} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${MOD_ID}>"
        "${_mod_dir}/lib/$<TARGET_FILE_NAME:${MOD_ID}>")


if (AUTO_COPY_MOD)
    if ("${SAPIENS_MOD_DIRECTORY}" STREQUAL "")
        message(STATUS "No Sapiens directory is set, looking for one at default locations.")
        if (WIN32)
            set(_copy_dest "$ENV{APPDATA}\\majicjungle\\sapiens\\mods\\")
            # check if _copy_dest exists
            if (EXISTS ${_copy_dest})
                message(STATUS "Found Sapiens directory at ${_copy_dest}")
                set(SAPIENS_MOD_DIRECTORY "${_copy_dest}" CACHE PATH "The directory Sapiens loads mods from." FORCE)
            else ()
                message(STATUS "Could not find Sapiens directory at ${_copy_dest}. Please verify where you installed Sapiens, and manually specify the path")
            endif ()
        elseif (UNIX)
            set(_copy_dest "$ENV{HOME}/.steam/steam/steamapps/compatdata/1060230/pfx/drive_c/users/steamuser/AppData/Roaming/majicjungle/sapiens/mods/")
            if (EXISTS ${_copy_dest})
                message(STATUS "Found Sapiens directory at ${_copy_dest}")
                set(SAPIENS_MOD_DIRECTORY "${_copy_dest}" CACHE PATH "The directory Sapiens loads mods from." FORCE)
            else ()
                message(STATUS "Could not find Sapiens directory at ${_copy_dest}. Please verify where you installed Sapiens, and manually specify the path")
            endif ()
        else ()
            message(STATUS "Auto copy currently only supported on Windows, macOS and Linux.")
        endif ()
    endif ()
    if (EXISTS ${SAPIENS_MOD_DIRECTORY})
        set(_mod_target_dir "${SAPIENS_MOD_DIRECTORY}/${MOD_ID}")
        message(STATUS "Mod will be copied from '${_mod_dir}' to '${_mod_target_dir}'")
        message(WARNING "${_mod_target_dir} will be recursively deleted and recreated if it already exists.")

        add_custom_target(copy_mod
                COMMAND ${CMAKE_COMMAND} -E remove_directory ${_mod_target_dir}

                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${_mod_dir}
                ${_mod_target_dir}

                DEPENDS ${MOD_ID}
                COMMENT "Copying mod to sapiens mod directory.")


        add_custom_target(run_game
                COMMAND steam "steam://rungameid/1060230")
                
        add_dependencies(run_game copy_mod)
    else ()
        message(FATAL_ERROR "Sapiens mod directory not found.")
    endif ()
endif ()
