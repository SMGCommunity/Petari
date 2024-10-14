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
config.shift_jis = False
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
    f"-DVERSION={version_num}"
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
    "-inline auto,level=2",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",
    "-ipa file",
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
        "AudioLib",
        [
            Object(NonMatching, "Game/AudioLib/AudSystem.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSystemVolumeController.cpp"),
            Object(NonMatching, "Game/AudioLib/AudAudience.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundInfo.cpp"),
            Object(NonMatching, "Game/AudioLib/AudWrap.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundObject.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundObject_Kawamura.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundObject_Takezawa.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundObject_Gohara.cpp"),
            Object(NonMatching, "Game/AudioLib/AudAnmSoundObject.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgm.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgmKeeper.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgmMgr.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgmRhythmStrategy.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgmSetting.cpp"),
            Object(NonMatching, "Game/AudioLib/AudBgmVolumeController.cpp"),
            Object(NonMatching, "Game/AudioLib/AudEffector.cpp"),
            Object(NonMatching, "Game/AudioLib/AudFader.cpp"),
            Object(NonMatching, "Game/AudioLib/AudLimitedSound.cpp"),
            Object(NonMatching, "Game/AudioLib/AudMeNameConverter.cpp"),
            Object(NonMatching, "Game/AudioLib/AudMicWrap.cpp"),
            Object(NonMatching, "Game/AudioLib/AudRemixMgr.cpp"),
            Object(NonMatching, "Game/AudioLib/AudRemixSequencer.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSceneMgr.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSeStrategy.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundNameConverter.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSoundObjHolder.cpp"),
            Object(NonMatching, "Game/AudioLib/AudSpeakerWrap.cpp"),
            Object(NonMatching, "Game/AudioLib/AudTrackController.cpp"),
            Object(NonMatching, "Game/AudioLib/AudUtil.cpp"),
            Object(NonMatching, "Game/AudioLib/OverwriteJAudio.cpp"),
            Object(NonMatching, "Game/AudioLib/CSSoundNameConverter.cpp")
        ],
    ),

    GameLib(
        "Boss",
        [
            Object(NonMatching, "Game/Boss/BossAccessor.cpp"),
            Object(NonMatching, "Game/Boss/BossBegoman.cpp"),
            Object(Matching, "Game/Boss/BossBegomanHead.cpp"),
            Object(NonMatching, "Game/Boss/BossKameck.cpp"),
            Object(Matching, "Game/Boss/BossKameckAction.cpp"),
            Object(Matching, "Game/Boss/BossKameckBarrier.cpp"),
            Object(Matching, "Game/Boss/BossKameckBattleDemo.cpp"),
            Object(Matching, "Game/Boss/BossKameckBattlePattarn.cpp"),
            Object(Matching, "Game/Boss/BossKameckSequencer.cpp"),
            Object(Matching, "Game/Boss/BossKameckMoveRail.cpp"),
            Object(Matching, "Game/Boss/BossKameckVs1.cpp"),
            Object(Matching, "Game/Boss/BossKameckVs2.cpp"),
            Object(Matching, "Game/Boss/BossKameckStateBattle.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBug.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugActionBase.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugActionFlyHigh.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugActionFlyLow.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugActionGround.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugActionSequencer.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugAngryDemo.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugBomb.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugBombHolder.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugFinishDemo.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugFlyDemo.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugFunction.cpp"),
            Object(NonMatching, "Game/Boss/BossStinkBugOpeningDemo.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackun.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunAction.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBall.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBattleEgg.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBattleEggVs2.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBattleVs1Lv1.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBattleVs1Lv2.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunBattleVs2Lv1.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunDemo.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunDemoPosition.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunEggShell.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunFire.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunSequencer.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunStateAwake.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunStateDamage.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunStateFire.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunTail.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunTailNode.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunTailPart.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunTailRoot.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunTrackFire.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunVs1.cpp"),
            Object(NonMatching, "Game/Boss/DinoPackunVs2.cpp"),
            Object(NonMatching, "Game/Boss/Dodoryu.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuDemo.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuHill.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuMove.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuStateBase.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuStateLv1.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuStateLv2.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuStateWait.cpp"),
            Object(NonMatching, "Game/Boss/DodoryuUtil.cpp"),
            Object(NonMatching, "Game/Boss/Koopa.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleBase.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleMain.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleStairsBase.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleStairsVs1.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleStairsVs3.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleVs3Lv1.cpp"),
            Object(NonMatching, "Game/Boss/KoopaBattleVs3Lv2.cpp"),
            Object(NonMatching, "Game/Boss/KoopaDemoPowerUp.cpp"),
            Object(NonMatching, "Game/Boss/KoopaDemoFallToPlanetVs3Lv2.cpp"),
            Object(NonMatching, "Game/Boss/KoopaDemoFallToPlanetVs3Lv3.cpp"),
            Object(NonMatching, "Game/Boss/KoopaDemoJumpToPlanet.cpp"),
            Object(NonMatching, "Game/Boss/KoopaFigureBall.cpp"),
            Object(NonMatching, "Game/Boss/KoopaFireShort.cpp"),
            Object(NonMatching, "Game/Boss/KoopaFireStairs.cpp"),
            Object(NonMatching, "Game/Boss/KoopaFunction.cpp"),
            Object(NonMatching, "Game/Boss/KoopaParts.cpp"),
            Object(NonMatching, "Game/Boss/KoopaPowerUpSwitch.cpp"),
            Object(NonMatching, "Game/Boss/KoopaPlanetShadow.cpp"),
            Object(NonMatching, "Game/Boss/KoopaRestarterVs3.cpp"),
            Object(NonMatching, "Game/Boss/KoopaRockBreak.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSensorCtrl.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSequencer.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSubSequenceBattle.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSequencerVs1.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSequencerVs2.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSequencerVs3.cpp"),
            Object(NonMatching, "Game/Boss/KoopaShockWave.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackFireShort.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackFireLong.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackHipDrop.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackRoll.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackShockWave.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateAttackSpin.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateChaseRoll.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateDamageEscape.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateGuard.cpp"),
            Object(NonMatching, "Game/Boss/KoopaStateJumpAway.cpp"),
            Object(NonMatching, "Game/Boss/KoopaSwitchKeeper.cpp"),
            Object(NonMatching, "Game/Boss/KoopaViewSwitchKeeper.cpp"),
            Object(NonMatching, "Game/Boss/OtaKing.cpp"),
            Object(NonMatching, "Game/Boss/OtaKingLongFoot.cpp"),
            Object(NonMatching, "Game/Boss/OtaKingMagma.cpp"),
            Object(NonMatching, "Game/Boss/Polta.cpp"),
            Object(NonMatching, "Game/Boss/PoltaActionBase.cpp"),
            Object(NonMatching, "Game/Boss/PoltaActionSequencer.cpp"),
            Object(NonMatching, "Game/Boss/PoltaArm.cpp"),
            Object(NonMatching, "Game/Boss/PoltaBattleLv1.cpp"),
            Object(NonMatching, "Game/Boss/PoltaBattleLv2.cpp"),
            Object(NonMatching, "Game/Boss/PoltaDemo.cpp"),
            Object(NonMatching, "Game/Boss/PoltaFunction.cpp"),
            Object(NonMatching, "Game/Boss/PoltaGroundRock.cpp"),
            Object(NonMatching, "Game/Boss/PoltaGroundRockHolder.cpp"),
            Object(NonMatching, "Game/Boss/PoltaRock.cpp"),
            Object(NonMatching, "Game/Boss/PoltaRockHolder.cpp"),
            Object(NonMatching, "Game/Boss/PoltaSensorCtrl.cpp"),
            Object(NonMatching, "Game/Boss/PoltaStateAttackGround.cpp"),
            Object(NonMatching, "Game/Boss/PoltaStateGenerateRock.cpp"),
            Object(NonMatching, "Game/Boss/PoltaStateGroundRockAttack.cpp"),
            Object(NonMatching, "Game/Boss/PoltaStatePunch.cpp"),
            Object(NonMatching, "Game/Boss/PoltaStateStagger.cpp"),
            Object(NonMatching, "Game/Boss/PoltaWaitStart.cpp"),
            Object(NonMatching, "Game/Boss/SkeletalFishBaby.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBabyRail.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBabyRailHolder.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBoss.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBossBattleDirector.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBossFunc.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBossInfo.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBossRail.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishBossRailHolder.cpp"),
            Object(NonMatching, "Game/Boss/SkeletalFishGuard.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishGuardHolder.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishJointCalc.cpp"),
            Object(Matching, "Game/Boss/SkeletalFishRailControl.cpp"),
            Object(NonMatching, "Game/Boss/TombSpider.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderAcid.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderAction1st.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderAction2nd.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderActionBase.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderActionCocoon.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderDemo.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderEnvironment.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderFunction.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderGland.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderParts.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderSensorCtrl.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderStateSwoon.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderThreadAttacher.cpp"),
            Object(NonMatching, "Game/Boss/TombSpiderVitalSpot.cpp"),
            Object(NonMatching, "Game/Boss/TripodBoss.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossAccesser.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossBaseJointPosition.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossBreakMovement.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossCoin.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossCore.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossFixParts.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossFixPartsBase.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossGuardWall.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossGuardWallPart.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossKillerGenerater.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossKillerGeneraterCircle.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossKinokoOneUp.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossLeg.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossMovableArea.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossRailMoveParts.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossRotateParts.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossShell.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossStepPoint.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossStepSequence.cpp"),
            Object(NonMatching, "Game/Boss/TripodBossStepStartArea.cpp")
        ],
    ),


    # Camera

    GameLib(
        "Demo",
        [
            Object(NonMatching, "Game/Demo/AstroDomeDemoStarter.cpp"),
            Object(NonMatching, "Game/Demo/AstroDemoFunction.cpp"),
            Object(NonMatching, "Game/Demo/DemoActionKeeper.cpp"),
            Object(NonMatching, "Game/Demo/DemoCameraFunction.cpp"),
            Object(NonMatching, "Game/Demo/DemoCameraKeeper.cpp"),
            Object(NonMatching, "Game/Demo/DemoCastGroup.cpp"),
            Object(Matching, "Game/Demo/DemoCastGroupHolder.cpp"),
            Object(NonMatching, "Game/Demo/DemoCastSubGroup.cpp"),
            Object(NonMatching, "Game/Demo/DemoCtrlBase.cpp"),
            Object(NonMatching, "Game/Demo/DemoDirector.cpp"),
            Object(NonMatching, "Game/Demo/DemoExecutor.cpp"),
            Object(Matching, "Game/Demo/DemoExecutorFunction.cpp"),
            Object(Matching, "Game/Demo/DemoFunction.cpp"),
            Object(NonMatching, "Game/Demo/DemoKoopaJrShip.cpp"),
            Object(Matching, "Game/Demo/DemoPadRumbler.cpp"),
            Object(Matching, "Game/Demo/DemoParamCommonDataTable.cpp"),
            Object(Matching, "Game/Demo/DemoPlayerKeeper.cpp"),
            Object(Matching, "Game/Demo/DemoPositionController.cpp"),
            Object(Matching, "Game/Demo/DemoSimpleCastHolder.cpp"),
            Object(NonMatching, "Game/Demo/DemoSoundKeeper.cpp"),
            Object(NonMatching, "Game/Demo/DemoStartRequestHolder.cpp"),
            Object(NonMatching, "Game/Demo/DemoStartRequestUtil.cpp"),
            Object(Matching, "Game/Demo/DemoSubPartKeeper.cpp"),
            Object(Matching, "Game/Demo/DemoTalkAnimCtrl.cpp"),
            Object(NonMatching, "Game/Demo/DemoTimeKeeper.cpp"),
            Object(NonMatching, "Game/Demo/DemoWipeKeeper.cpp"),
            Object(NonMatching, "Game/Demo/GrandStarReturnDemoStarter.cpp"),
            Object(NonMatching, "Game/Demo/PrologueDirector.cpp"),
            Object(NonMatching, "Game/Demo/ReturnDemoRailMove.cpp"),
            Object(NonMatching, "Game/Demo/ScenarioStarter.cpp"),
            Object(NonMatching, "Game/Demo/StarReturnDemoStarter.cpp")
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
