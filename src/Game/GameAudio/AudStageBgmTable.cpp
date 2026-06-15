#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/GameAudio/AudStageBgmWrap.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

// TODO: bgm state enum

namespace {
    struct StageBgm {
        /* 0x00 */ const char* mStageName;
        /* 0x04 */ u32 mBgmIDs[6];
        /* 0x1C */ u32 mBgmStates[8];
    };

    StageBgm sStageBgmSet[] = {
        {
            "OceanRingGalaxy",
            {-1, MBGM_GALAXY_03, MBGM_GALAXY_18, MBGM_GALAXY_01_TOMB, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "OceanPhantomCaveGalaxy",
            {MBGM_GALAXY_12, MBGM_GALAXY_05, MBGM_GALAXY_INTER, MBGM_GALAXY_06, MBGM_GALAXY_18, -1},
            {0, 1, -1, -1, -1, -1, -1, -1},
        },
        {
            "EggStarGalaxy",
            {MBGM_GALAXY_12, MBGM_GALAXY_01, MBGM_GALAXY_INTER, MBGM_GALAXY_01_TOMB, MBGM_GALAXY_18, -1},
            {0, 1, -1, -1, -1, -1, -1, -1},
        },
        {
            "StarDustGalaxy",
            {MBGM_GALAXY_09, MBGM_GALAXY_08, MBGM_GALAXY_01_TOMB, MBGM_GALAXY_12, MBGM_GALAXY_13, MBGM_KINOPIO_TANKEN},
            {0, 1, -1, -1, -1, -1, -1, -1},
        },
        {
            "PhantomGalaxy",
            {MBGM_GALAXY_INTER, MBGM_GALAXY_06, MBGM_GALAXY_12, MBGM_GALAXY_18, -1, -1},
            {0, 1, -1, -1, -1, -1, -1, -1},
        },
        {
            "BattleShipGalaxy",
            {MBGM_GALAXY_11, MBGM_GALAXY_13, MBGM_GALAXY_01_TOMB, MBGM_GALAXY_18, -1, -1},
            {0, 1, 7, -1, -1, -1, -1, -1},
        },
        {
            "HeavenlyBeachGalaxy",
            {MBGM_GALAXY_03, MBGM_GALAXY_13, MBGM_GALAXY_18, MBGM_GALAXY_17, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "IceVolcanoGalaxy",
            {MBGM_GALAXY_INTER, MBGM_GALAXY_16, MBGM_GALAXY_13, MBGM_GALAXY_18, -1, -1},
            {0, 1, 2, -1, -1, -1, -1, -1},
        },
        {
            "CannonFleetGalaxy",
            {MBGM_GALAXY_11, MBGM_GALAXY_13, MBGM_GALAXY_01_TOMB, MBGM_GALAXY_18, -1, -1},
            {0, 1, 7, -1, -1, -1, -1, -1},
        },
        {
            "FactoryGalaxy",
            {MBGM_GALAXY_22, MBGM_GALAXY_18, MBGM_BOSS_MECHA_KOOPA, MBGM_GALAXY_27, MBGM_BOSS_05_A, -1},
            {0, 1, 2, -1, -1, -1, -1, -1},
        },
        {
            "SandClockGalaxy",
            {MBGM_GALAXY_19, MBGM_GALAXY_13, MBGM_GALAXY_18, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "HeavensDoorGalaxy",
            {MBGM_GALAXY_24, MBGM_GALAXY_25, MBGM_GALAXY_26, MBGM_GALAXY_18, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "HoneyBeeKingdomGalaxy",
            {MBGM_GALAXY_10, MBGM_GALAXY_18, MBGM_GALAXY_10_HURRY, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "ReverseKingdomGalaxy",
            {MBGM_GALAXY_10, MBGM_GALAXY_18, MBGM_GALAXY_INTER, MBGM_BOSS_05_A, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "CosmosGardenGalaxy",
            {MBGM_GALAXY_14, MBGM_GALAXY_13, MBGM_GALAXY_12, MBGM_GALAXY_18, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "HellProminenceGalaxy",
            {MBGM_GALAXY_02, MBGM_GALAXY_18, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "KoopaBattleVs3Galaxy",
            {MBGM_GALAXY_28, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "SurfingLv1Galaxy",
            {MBGM_GALAXY_03, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
        {
            "SurfingLv2Galaxy",
            {MBGM_GALAXY_03, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1},
        },
    };

    const StageBgm* findDataElement(const char* pStageName) {
        for (u32 idx = 0; idx < ARRAY_SIZE(sStageBgmSet); idx++) {
            const StageBgm* bgm = &sStageBgmSet[idx];
            if (MR::isEqualString(pStageName, bgm->mStageName)) {
                return bgm;
            }
        }
        return nullptr;
    }

}  // namespace

u32 AudStageBgmTable::getBgmId(const char* pStageName, s32 bgmNo) {
    JAISoundID cometBgm = AudStageBgmWrap::getCometEventBgm(pStageName);
    if (cometBgm != -1) {
        return cometBgm;
    }

    const StageBgm* bgm = ::findDataElement(pStageName);
    if (bgm != nullptr) {
        return bgm->mBgmIDs[bgmNo];
    }

    return 0;
}

u32 AudStageBgmTable::getBgmState(const char* pStageName, s32 stateNo) {
    const StageBgm* bgm = ::findDataElement(pStageName);
    if (bgm != nullptr) {
        return bgm->mBgmStates[stateNo];
    }

    return 0;
}
