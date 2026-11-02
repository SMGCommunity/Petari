Petari
[![Build Status]][actions] [![Discord Badge]][discord]
======
![RMGJ01_Code]![RMGJ01_Link] ![RMGE01_Code]![RMGE01_Link] ![RMGP01_Code]![RMGP01_Link] ![RMGK01_Code]![RMGK01_Link]

[Build Status]: https://github.com/SMGCommunity/Petari/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/SMGCommunity/Petari/actions/workflows/build.yml

[RMGJ01_Code]: https://decomp.dev/SMGCommunity/Petari/RMGJ01.svg?mode=shield&measure=code&label=RMGJ01
[RMGJ01_Link]: https://decomp.dev/SMGCommunity/Petari/RMGJ01.svg?mode=shield&measure=complete_code_percent&label=
[RMGE01_Code]: https://decomp.dev/SMGCommunity/Petari/RMGE01.svg?mode=shield&measure=code&label=RMGE01
[RMGE01_Link]: https://decomp.dev/SMGCommunity/Petari/RMGE01.svg?mode=shield&measure=complete_code_percent&label=
[RMGP01_Code]: https://decomp.dev/SMGCommunity/Petari/RMGP01.svg?mode=shield&measure=code&label=RMGP01
[RMGP01_Link]: https://decomp.dev/SMGCommunity/Petari/RMGP01.svg?mode=shield&measure=complete_code_percent&label=
[RMGK01_Code]: https://decomp.dev/SMGCommunity/Petari/RMGK01.svg?mode=shield&measure=code&label=RMGK01
[RMGK01_Link]: https://decomp.dev/SMGCommunity/Petari/RMGK01.svg?mode=shield&measure=complete_code_percent&label=

[Discord Badge]: https://img.shields.io/discord/727908905392275526?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/ZxEqyYeZbf
[progress_link]: https://decomp.dev/SMGCommunity/Petari

<!-- markdownlint-disable MD033 -->
[<img src="https://decomp.dev/SMGCommunity/Petari.svg?w=512&h=256" width="512" height="256" alt="A visual">][progress_link]
<!-- markdownlint-enable MD033 -->

A work-in-progress decompilation of Super Mario Galaxy.

This repository does **not** contain any game assets or assembly whatsoever. An existing copy of the game is required.

This project is **not** meant to be an effort to create a PC port. Join the Discord server to learn more and ask about our work-in-progress PC port "AstroCore".

> [!NOTE]
> This is a community-focused project that would like to remain as human as possible. AI-generated or otherwise automated contributions will be rejected. Contributors should be able to explain and justify any work they submit.

Dependencies
============

Windows
-------

On Windows, it is **highly recommended** to use native tooling. WSL or msys2 are **not** required.  
When running under WSL, [objdiff](#diffing) is unable to get filesystem notifications for automatic rebuilds.

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

macOS
-----

- Install [ninja](https://github.com/ninja-build/ninja/releases):

  ```sh
  brew install ninja
  ```

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Linux
-----

- Install [ninja](https://github.com/ninja-build/ninja/releases).

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Building
========

- Clone the repository:

  ```sh
  git clone https://github.com/SMGCommunity/Petari.git
  ```

- Using [Dolphin Emulator](https://dolphin-emu.org/), extract your desired region of the game to `orig/RMGX01/`, where `X` is the region code.
![](assets/dolphin-extract.png)
  - To save space, the only necessary files are the following. Any others can be deleted.
    - `sys/main.dol`
- Configure:

  ```sh
  python configure.py
  ```

  To use a version other than `RMGK01` (Korea), specify it with `--version`.
- Build:

  ```sh
  ninja
  ```

Diffing
=======

Once the initial build succeeds, an `objdiff.json` should exist in the project root.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.

![](assets/objdiff.png)

Credits
=======
Big thanks to the [doldecomp team](https://github.com/doldecomp) for their efforts on bte, [tp](https://github.com/zeldaret/tp) for JSystem, and [ogws](https://github.com/doldecomp/ogws/tree/master), where this repository has sourced code and headers from.
