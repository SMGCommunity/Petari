#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "RMGK01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.shift_jis = True
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v0.9.5"
config.objdiff_tag = "v2.0.0"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,s",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

cflags_game = [
    "-nodefaults",
    "-lang c++",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,s",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",
    "-i include",
    "-i libs/JSystem/include",
    "-i libs/MSL_C++/include",
    "-i libs/MSL_C/include",
    "-i libs/MetroTRK/include",
    "-i libs/RVLFaceLib/include",
    "-i libs/RVL_SDK/include",
    "-i libs/Runtime/include",
    "-i libs/nw4r/include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/3.0a5"

def GameLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/3.0a3",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": objects,
    }
    

# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": objects,
    }

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching

config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(NonMatching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
        ],
    },

    GameLib(
        "Animation",
        [
            Object(NonMatching, "Game/Animation/AnmPlayer.cpp"),
            Object(NonMatching, "Game/Animation/BckCtrl.cpp"),
            Object(NonMatching, "Game/Animation/BpkPlayer.cpp"),
            Object(NonMatching, "Game/Animation/BrkPlayer.cpp"),
            Object(NonMatching, "Game/Animation/BtkPlayer.cpp"),
            Object(NonMatching, "Game/Animation/BtpPlayer.cpp"),
            Object(NonMatching, "Game/Animation/BvaPlayer.cpp"),
            Object(NonMatching, "Game/Animation/LayoutAnmPlayer.cpp"),
            Object(NonMatching, "Game/Animation/MaterialAnmBuffer.cpp"),
            Object(NonMatching, "Game/Animation/XanimeCore.cpp"),
            Object(NonMatching, "Game/Animation/XanimePlayer.cpp"),
            Object(NonMatching, "Game/Animation/XanimeResource.cpp"),
        ],
    ),

    GameLib(
        "AreaObj",
        [
            Object(NonMatching, "Game/AreaObj/AreaForm.cpp"),
            Object(NonMatching, "Game/AreaObj/AreaFormDrawer.cpp"),
            Object(NonMatching, "Game/AreaObj/AreaObj.cpp"),
            Object(Matching, "Game/AreaObj/AreaObjContainer.cpp"),
            Object(Matching, "Game/AreaObj/AreaObjFollower.cpp"),
            Object(Matching, "Game/AreaObj/AstroChangeStageCube.cpp"),
            Object(Matching, "Game/AreaObj/AudioEffectArea.cpp"),
            Object(Matching, "Game/AreaObj/BgmProhibitArea.cpp"),
            Object(Matching, "Game/AreaObj/BigBubbleCameraArea.cpp"),
            Object(NonMatching, "Game/AreaObj/BigBubbleGoalArea.cpp"),
            Object(NonMatching, "Game/AreaObj/BloomArea.cpp"),
            Object(NonMatching, "Game/AreaObj/CameraRepulsiveArea.cpp"),
            Object(NonMatching, "Game/AreaObj/ChangeBgmCube.cpp"),
            Object(NonMatching, "Game/AreaObj/CollisionArea.cpp"),
            Object(NonMatching, "Game/AreaObj/CubeCamera.cpp"),
            Object(Matching, "Game/AreaObj/DeathArea.cpp"),
            Object(Matching, "Game/AreaObj/DepthOfFieldArea.cpp"),
            Object(NonMatching, "Game/AreaObj/FollowCollisionArea.cpp"),
            Object(Matching, "Game/AreaObj/GlaringLightArea.cpp"),
            Object(Matching, "Game/AreaObj/HazeCube.cpp"),
            Object(NonMatching, "Game/AreaObj/ImageEffectArea.cpp"),
            Object(Matching, "Game/AreaObj/LightArea.cpp"),
            Object(NonMatching, "Game/AreaObj/LightAreaHolder.cpp"),
            Object(NonMatching, "Game/AreaObj/MercatorTransformCube.cpp"),
            Object(Matching, "Game/AreaObj/MessageArea.cpp"),
            Object(Matching, "Game/AreaObj/PlayerSeArea.cpp"),
            Object(Matching, "Game/AreaObj/QuakeEffectArea.cpp"),
            Object(Matching, "Game/AreaObj/RestartCube.cpp"),
            Object(Matching, "Game/AreaObj/ScreenBlurArea.cpp"),
            Object(Matching, "Game/AreaObj/SimpleBloomArea.cpp"),
            Object(NonMatching, "Game/AreaObj/SoundEmitterCube.cpp"),
            Object(NonMatching, "Game/AreaObj/SoundEmitterSphere.cpp"),
            Object(Matching, "Game/AreaObj/SpinGuidanceArea.cpp"),
            Object(Matching, "Game/AreaObj/SunLightArea.cpp"),
            Object(Matching, "Game/AreaObj/SwitchArea.cpp"),
            Object(NonMatching, "Game/AreaObj/WarpCube.cpp"),
            Object(Matching, "Game/AreaObj/WaterArea.cpp")
        ],
    ),

    GameLib(
        "Gravity",
        [
            Object(Matching, "Game/Gravity/ConeGravity.cpp"),
            Object(Matching, "Game/Gravity/CubeGravity.cpp"),
            Object(Matching, "Game/Gravity/DiskGravity.cpp"),
            Object(Matching, "Game/Gravity/DiskTorusGravity.cpp"),
            Object(Matching, "Game/Gravity/GlobalGravityObj.cpp"),
            Object(Matching, "Game/Gravity/GraviryFollower.cpp"),
            Object(Matching, "Game/Gravity/GravityCreator.cpp"),
            Object(Matching, "Game/Gravity/GravityInfo.cpp"),
            Object(Matching, "Game/Gravity/ParallelGravity.cpp"),
            Object(Matching, "Game/Gravity/PlanetGravity.cpp"),
            Object(Matching, "Game/Gravity/PlanetGravityManager.cpp"),
            Object(Matching, "Game/Gravity/PointGravity.cpp"),
            Object(Matching, "Game/Gravity/SegmentGravity.cpp"),
            Object(Matching, "Game/Gravity/WireGravity.cpp")
        ],
    ),

    GameLib(
        "NameObj",
        [
            Object(NonMatching, "Game/NameObj/ModelChangableObjFactory.cpp"),
            Object(Matching, "Game/NameObj/MovementOnOffGroupHolder.cpp"),
            Object(Matching, "Game/NameObj/NameObj.cpp"),
            Object(Matching, "Game/NameObj/NameObjAdaptor.cpp"),
            Object(Matching, "Game/NameObj/NameObjArchiveListCollector.cpp"),
            Object(NonMatching, "Game/NameObj/NameObjCategoryList.cpp"),
            Object(Matching, "Game/NameObj/NameObjExecuteHolder.cpp"),
            Object(NonMatching, "Game/NameObj/NameObjFactory.cpp"),
            Object(Matching, "Game/NameObj/NameObjFinder.cpp"),
            Object(Matching, "Game/NameObj/NameObjGroup.cpp"),
            Object(NonMatching, "Game/NameObj/NameObjHolder.cpp"),
            Object(NonMatching, "Game/NameObj/NameObjListExecutor.cpp"),
            Object(Matching, "Game/NameObj/NameObjRegister.cpp")
        ],
    )
]

# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game"),
    ProgressCategory("jsys", "JSystem"),
    ProgressCategory("sdk", "SDK"),
    ProgressCategory("nw4r", "NW4R"),
    ProgressCategory("rfl", "RVLFaceLib")
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
