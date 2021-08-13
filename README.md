# Super Mario Galaxy 1 Decompilation

This repo contains a WIP decompilation of Super Mario Galaxy 1 (US).

It builds the following DOL:

main.dol: `sha1: 9A71008AE1EE9010E267FA67D1F0B0D4F0E895DD`

## Build Instructions

1. Obtain a clean DOL of Super Mario Galaxy 1 and place it in the include folder and name it `baserom.dol`.
2. Obtain a copy of mwcceppc and mwldeppc from CodeWarrior 1.2.5 for Gamecube (I can supply this if you DM me shibboleet#1407). You can run the installer and grab the two files out of the `PowerPC_EABI_Tools/Command_Line_Tools` folder where you installed it. Also obtain `powerpc-eabi-as` and `powerpc-eabi-objcopy`, which can be installed with [devkitPro](https://devkitpro.org/wiki/Getting_Started). These will go into the `tools` folder.
3. Build tools and `make`.

## Contributions

Contributions and PRs are welcome.