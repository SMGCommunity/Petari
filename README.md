# Super Mario Galaxy 1 Decompilation

This repo contains a WIP decompilation of Super Mario Galaxy 1 (USA).

## Build Instructions

1. Obtain a copy of mwcceppc and mwldeppc from CodeWarrior 1.2.5 for Gamecube, as well as any version of RVL_SDK, and NW4R. (I can supply this if you DM me shibboleet#1010). You can run the installer and grab the two files out of the `PowerPC_EABI_Tools/Command_Line_Tools` folder where you installed it. Also obtain `powerpc-eabi-as` and `powerpc-eabi-objcopy`, which can be installed with [devkitPro](https://devkitpro.org/wiki/Getting_Started). These will go into the `tools` folder
2. Add `MWFOLDER`, the path to the CodeWarrior 1.2.5 installation, to the system enviornment variables. Do the same for NW4R with the name `NW4RFOLDER`, and once more for RVL_SDK with the name `RVLFOLDER`. Add `CWFOLDER` and have it point to a folder that contains all of the GC/Wii compilers (I can supply this if you need it.)
3. Run `build.py`.

## Contributions

Contributions and PRs are welcome.