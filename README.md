# SapiensCModTemplate
A CMake template to make c-mods for the sapiens game 

## Getting Started

1. Create a repository based on this template
2. Clone the newly create repository 
```
git clone --recurse-submodules [YOUR GIT REPO]
```
3. In the newly created repository configure cmake, using your favourite IDE, the cmake gui, or the command line as follows:
```
cmake -DMOD_ID=MyCMod -DMOD_NAME="My C Mod" -DDESCRIPTION="An amazing mod that does things in C" -DDEVELOPER="Me!" -DDEVELOPER_URL="https://example.com" -DPREVIEW_FILE="preview.png" -DMOD_MAJOR_VERSION=1 -DMOD_MINOR_VERSION=2 -DMOD_PATCH_VERSION=3 . -B build
```
4. Build the mod, through your IDE or from command line:
```
cd build
cmake --build .
```
5. The final mod folder can be found in the build folder (`./build` in the above commands) and is named as the given modid (`MyCMod` in the above commands)

## Settings

- **MOD_ID** Internally used for name of the generated files like `dll`s and final folder.
- **MOD_NAME** Name of the mod as shown in game
- **DESCRIPTION** Description of the mod as shown in game
- **DEVELOPER** Developer of the mod
- **DEVELOPER_URL** URL of the developer of the mod
- **PREVIEW_FILE** Image displayed in game as a preview
- **MOD_MAJOR_VERSION** Major version of the mod (Combined with the following variables as MAJOR.MINOR.PATCH)
- **MOD_MINOR_VERSION** Minor version of the mod
- **MOD_PATCH_VERSION** Patch version of the mod
- **AUTO_GENERATE_SAPIENS_MOD_SYMLINK** When ON will attempt to generate a symlink in the default mod folder to the generated build files for easy debugging
- **MOD_TYPE** ADVANCED setting to change the mod type, C mods should always be the default world type but can be set to app using this.
