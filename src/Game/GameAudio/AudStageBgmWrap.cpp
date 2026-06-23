#include "Game/GameAudio/AudStageBgmWrap.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

struct StageBgmData {
    /* 0x00 */ const char* mSceneName;
    /* 0x04 */ const char* mStageName;
    /* 0x08 */ u32 mSoundID;
    /* 0x0C */ s32 mScenarioGroup;
};

const StageBgmData sMAStageBgmData[] = {
    {"Game", "HellProminenceGalaxy", MBGM_GALAXY_02, -1},
    {"Game", "OceanRingGalaxy", MBGM_GALAXY_03, -1},
    {"Game", "OceanFloaterLandGalaxy", MBGM_GALAXY_04, -1},
    {"Game", "EggStarGalaxy", MBGM_GALAXY_01, -1},
    {"Game", "PhantomGalaxy", MBGM_GALAXY_INTER, -1},
    {"Game", "AstroDome", MBGM_ASTRO_DOME, 4},
    {"Game", "HeavenlyBeachGalaxy", MBGM_GALAXY_03, -1},
    {"Game", "AstroGalaxy", STM_ASTRO_OUT, 1},
    {"Game", "CosmosGardenGalaxy", MBGM_GALAXY_14, -1},
    {"Game", "HoneyBeeKingdomGalaxy", MBGM_GALAXY_10, 0},
    {"Game", "BeltConveyerExGalaxy", MBGM_GALAXY_17, -1},
    {"Game", "KoopaBattleVs1Galaxy", MBGM_GALAXY_15, -1},
    {"Game", "SurfingLv1Galaxy", MBGM_GALAXY_03, -1},
    {"Game", "SurfingLv2Galaxy", MBGM_GALAXY_03, -1},
    {"Game", "CubeBubbleExLv1Galaxy", MBGM_GALAXY_12, -1},
    {"Game", "CubeBubbleExLv2Galaxy", MBGM_GALAXY_12, -1},
    {"Game", "FlipPanelExGalaxy", MBGM_GALAXY_13, -1},
    {"Game", "TamakoroExLv1Galaxy", MBGM_GALAXY_17, -1},
    {"Game", "TamakoroExLv2Galaxy", MBGM_GALAXY_17, -1},
    {"Game", "BreakDownPlanetGalaxy", MBGM_GALAXY_13, -1},
    {"Game", "SandClockGalaxy", MBGM_GALAXY_19, -1},
    {"Game", "KoopaBattleVs2Galaxy", MBGM_GALAXY_15, -1},
    {"Game", "KoopaJrShipLv1Galaxy", MBGM_GALAXY_09, -1},
    {"Game", "HoneyBeeExGalaxy", MBGM_GALAXY_17, -1},
    {"Game", "TriLegLv1Galaxy", MBGM_GALAXY_01_TOMB, -1},
    {"Game", "TriLegLv2Galaxy", MBGM_GALAXY_01_TOMB, -1},
    {"Game", "CocoonExGalaxy", MBGM_GALAXY_12, -1},
    {"Game", "ReverseKingdomGalaxy", MBGM_GALAXY_10, -1},
    {"Game", "DarkRoomGalaxy", MBGM_GALAXY_12, -1},
    {"Game", "FactoryGalaxy", MBGM_GALAXY_22, -1},
    {"Game", "SkullSharkGalaxy", MBGM_GALAXY_23, -1},
    {"Game", "KoopaJrDriverGalaxy", MBGM_GALAXY_01_TOMB, -1},
    {"Game", "FloaterOtaKingGalaxy", MBGM_GALAXY_01_TOMB, -1},
    {"Game", "TransformationExGalaxy", MBGM_GALAXY_13, -1},
    {"Game", "LibraryRoom", MBGM_LIBRARY, -1},
    {"Game", "TearDropGalaxy", MBGM_GALAXY_23, -1},
    {"Game", "SnowCapsuleGalaxy", MBGM_GALAXY_27, -1},
    {"Game", "FishTunnelGalaxy", MBGM_GALAXY_05, -1},
    {"Game", "HeavensDoorGalaxy", MBGM_GALAXY_24, 2},
    {"Game", "TeresaMario2DGalaxy", MBGM_GALAXY_12, -1},
    {"Game", "OceanPhantomCaveGalaxy", MBGM_GALAXY_INTER, -1},
    {"Game", "KoopaBattleVs3Galaxy", MBGM_GALAXY_28, -1},
    {"Game", "PeachCastleGardenGalaxy", STM_PROLOGUE_05, -1},
    {"Game", "BattleShipGalaxy", MBGM_GALAXY_11, 3},
    {"Game", "CannonFleetGalaxy", MBGM_GALAXY_11, 3},
    {"Game", "PeachCastleFinalGalaxy", STM_PROLOGUE_02, -1},
    {"Game", "E3HeavenlyBeachGalaxy", MBGM_GALAXY_13, -1},
    {nullptr, nullptr, -1, -1},
};

const u32 sMASenarioBgmData[][6] = {
    {
        MBGM_GALAXY_10,
        MBGM_GALAXY_10,
        MBGM_GALAXY_10_HURRY,
        MBGM_GALAXY_10,
        MBGM_GALAXY_10,
        MBGM_GALAXY_10,
    },
    {
        STM_ASTRO_OUT,
        STM_ASTRO_OUT_2,
        STM_ASTRO_OUT_3,
        MBGM_GALAXY_24,
        STM_ASTRO_OUT_3,
        STM_ASTRO_OUT_3,
    },
    {
        -1,
        MBGM_GALAXY_24,
        MBGM_GALAXY_24,
        MBGM_GALAXY_24,
        MBGM_GALAXY_24,
        MBGM_GALAXY_24,

    },
    {
        -1,
        -1,
        -1,
        -1,
        MBGM_GALAXY_21,
        -1,
    },
    {
        MBGM_ASTRO_DOME,
        MBGM_ASTRO_DOME,
        MBGM_ASTRO_DOME,
        MBGM_ASTRO_DOME,
        MBGM_ASTRO_DOME,
        MBGM_ASTRO_DOME_LOFT,
    },
};

inline JAISoundID getScenarioBgmData(int index, int scenarioNo) {
    return sMASenarioBgmData[index][scenarioNo];
}

JAISoundID AudStageBgmWrap::changeStageNameToSoundID(const char* pSceneName, const char* pStageName, s32 scenarioNo) {
    JAISoundID soundID = -1;
    JAISoundID koopaAppearBgm = getKoopaFortressAppearBgm(pStageName);
    if (koopaAppearBgm != -1) {
        return (u32)koopaAppearBgm;  // TODO: ?
    }

    JAISoundID cometEventBgm = getCometEventBgm(pStageName);
    if (cometEventBgm != -1) {
        return (u32)cometEventBgm;  // TODO: ?
    }

    if (MR::isEqualString(pStageName, "AstroGalaxy") && scenarioNo == 1 && !MR::isOnGameEventFlagAstroGalaxyBgmBright()) {
        return MBGM_GALAXY_24;
    }

    u32 idx = 0;
    while (sMAStageBgmData[idx].mStageName != nullptr) {
        if (MR::strcasecmp(sMAStageBgmData[idx].mSceneName, pSceneName) == 0 && MR::strcasecmp(sMAStageBgmData[idx].mStageName, pStageName) == 0) {
            if (sMAStageBgmData[idx].mScenarioGroup >= 0) {
                soundID = getScenarioBgmData(sMAStageBgmData[idx].mScenarioGroup, scenarioNo - 1);
            } else {
                soundID = sMAStageBgmData[idx].mSoundID;
            }
            break;
        }
        idx++;
    }
    return soundID;
}

JAISoundID AudStageBgmWrap::getCometEventBgm(const char* pStageName) {
    if (MR::isGalaxyRedCometAppearInCurrentStage() || MR::isGalaxyDarkCometAppearInCurrentStage()) {
        return MBGM_GALAXY_02_HURRY;
    }

    if (MR::isGalaxyGhostCometAppearInCurrentStage() || MR::isGalaxyQuickCometAppearInCurrentStage()) {
        return MBGM_GALAXY_20;
    }

    if (MR::isGalaxyBlackCometAppearInCurrentStage()) {
        return MBGM_GALAXY_21;
    }

    return -1;
}

JAISoundID AudStageBgmWrap::getKoopaFortressAppearBgm(const char* pStageName) {
    if (MR::isEqualString(pStageName, "AstroDome") && MR::isKoopaFortressAppearInGalaxy()) {
        return MBGM_ASTRO_DOME_2;
    }

    return -1;
}
