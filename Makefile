MOD_ID := MyMod
MOD_MAJOR_VERSION := 1
MOD_MINOR_VERSION := 2
MOD_PATCH_VERSION := 3

SAPIENS_MOD_DIRECTORY := "/mnt/LinuxHDD/SteamLibrary/steamapps/compatdata/1060230/pfx/drive_c/users/steamuser/AppData/Roaming/majicjungle/sapiens/mods"

UNAME := $(shell uname)
CMAKE_BUILD_BINARY := cmake

ifeq ($(UNAME),Linux)
	CMAKE_BUILD_BINARY := x86_64-w64-mingw32-cmake
endif

build:
	${CMAKE_BUILD_BINARY}\
		-DMOD_ID="${MOD_ID}"\
		-DMOD_MAJOR_VERSION=1\
		-DMOD_MINOR_VERSION=2\
		-DMOD_PATCH_VERSION=1\
		-DSAPIENS_MOD_DIRECTORY="${SAPIENS_MOD_DIRECTORY}"\
	 . -B build

run:
	cmake --build build/ --target run_game