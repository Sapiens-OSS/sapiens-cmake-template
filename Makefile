MOD_ID := MyMod
MOD_NAME := My Mod
DESCRIPTION := An amazing mod that does things in C and Lua
DEVELOPER := Me!
DEVELOPER_URL := https://example.com
PREVIEW_FILE := preview.png
MOD_MAJOR_VERSION := 1
MOD_MINOR_VERSION := 2
MOD_PATCH_VERSION := 3

AUTO_COPY_MOD := ON
SAPIENS_MOD_DIRECTORY := ""

UNAME := $(shell uname)
CMAKE_BUILD_BINARY := cmake

build:
	ifeq ($(UNAME), Linux)
		CMAKE_BUILD_BINARY = x86_64-w64-mingw32-cmake
	endif
	${CMAKE_BUILD_BINARY}\
		-DMOD_ID=${MOD_ID}\
		-DMOD_NAME="Terrain Variations"\
		-DDESCRIPTION="${MOD_DESCRIPTION}"\
		-DDEVELOPER="nmattela"\
		-DDEVELOPER_URL="https://github.com/nmattela/sapiens_terrain_variations"\
		-DPREVIEW_FILE="preview.png"\
		-DMOD_MAJOR_VERSION=1\
		-DMOD_MINOR_VERSION=2\
		-DMOD_PATCH_VERSION=1\
		-DAUTO_COPY_MOD=ON\
		-DSAPIENS_MOD_DIRECTORY="/mnt/LinuxHDD/SteamLibrary/steamapps/compatdata/1060230/pfx/drive_c/users/steamuser/AppData/Roaming/majicjungle/sapiens/mods"\
	. -B build

run:
	cmake --build build/ --target run_game