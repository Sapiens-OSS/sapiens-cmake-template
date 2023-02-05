# Sapiens Mod Template
A CMake template to make C and Lua mods for the Sapiens game

## Getting Started (VSCode extension)

1. Install the Sapiens Modding extension for VSCode in the VSCode marketplace
2. Search using Ctrl+Shift+P for 'newProject'. An option to create a new Sapiens mod project will appear
3. Follow the guide that shows up
    1. Enter the directory in which a new directory will be created that contains the project
    2. Enter the Mod ID. This ID will become the name of the directory it will create
    3. Enter the path to your Sapiens installation. This depends on where your Steam library is located, but the default locations are as follows:

        | Platform       	| File Path                                                                                                           	|
        |----------------	|---------------------------------------------------------------------------------------------------------------------	|
        | Windows        	| `%AppData%\majicjungle\sapiens\mods`                                                                                	|
        | MacOS          	| ?                                                                                                                   	|
        | Linux (Proton) 	| `~/.steam/steam/steamapps/compatdata/1060230/pfx/drive_c/users/steamuser/AppData/Roaming/majicjungle/sapiens/mods/` 	|

        Note that the extension will attempt to automatically look for an installation folder, but this is a long process with a tendency to crash, and only works on Linux for now. So don't bother waiting for it
    4. Enter the name of your mod. This is a string in which you are free to type in anything
    5. Enter the description of your mod
    6. Enter the mod type. To know the difference, consult the wiki. But in short: If your mod is for localizations (translations), choose 'app'. Otherwise, choose 'world'
    7. Enter the name of the developer (you)
    8. Enter a website (optional)
4. Confirm, but double-check the location it will write files to, as well as the mod location where your mod will be copied to when building
5. In the output, you can follow the process. If everything works, it will open a new VSCode window in the project.

From thereon out, you can run
```
cmake --build build/ --target sync_mod_files --target run_game
```
To run and build the game.

If, for any reason, your build folder was removed, your steam library location changes, or you want to change the ID of your mod, run the following:
```
# Linux users, replace cmake with x86_64-w64-mingw32-cmake
cmake -DMOD_ID=MyCMod -DAUTO_COPY_MOD=ON -DSAPIENS_MOD_DIRECTORY="/path/to/sapiens/installation/mods/folder" . -B build
```


## Getting Started (Manual)

1. Create a repository based on this template (click the green 'Use this template' button on Github)
2. Clone the newly created repository 
```
git clone --recurse-submodules [YOUR GIT REPO]
```
3. In the newly created repository, change `modInfo.lua` to your liking. Do not forget to set `"type"` to either `"world"` or `"app"`, or your mod won't appear in-game
4. Configure cmake, using your favourite IDE, the cmake gui, or the command line as follows:
```
# Linux users, replace cmake with x86_64-w64-mingw32-cmake
cmake -DMOD_ID=MyCMod -DAUTO_COPY_MOD=ON -DSAPIENS_MOD_DIRECTORY="/path/to/sapiens/installation/mods/folder" . -B build
```
5. Build the mod, through your IDE or from command line:
```
cmake --build build/ --target sync_mod_files --target run_game
```
6. The final mod folder can be found in the build folder (`./build` in the above commands) and is named as the given mod ID (`MyCMod` in the above commands)

## Settings

- **MOD_ID** Internally used for name of the generated files like `dll`s and final folder.
- **SAPIENS_MOD_DIRECTORY** The mod directory in the sapiens folder. Defaults on windows to `%AppData%\majicjungle\sapiens\mods\`
- **AUTO_COPY_MOD** When ON will attempt to copy the mod to the SAPIENS_MOD_DIRECTORY
