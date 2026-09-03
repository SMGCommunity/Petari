#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudEffector.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include <JSystem/JAudio2/JAUSectionHeap.hpp>

namespace {
    static const AudSceneSoundInfo cSceneInfo[] = {
        {"dummy", "dummy", 0, -1},
        {"Game", "HellProminenceGalaxy", 1, -1},
        {"Game", "OceanRingGalaxy", 2, 5},
        {"Game", "OceanFloaterLandGalaxy", 3, -1},
        {"Game", "OceanPhantomCaveGalaxy", 4, 2},
        //
        {"Game", "EggStarGalaxy", 6, 0},
        {"Game", "StarDustGalaxy", 7, 1},
        {"Game", "BattleShipGalaxy", 8, -1},
        {"Game", "CannonFleetGalaxy", 9, -1},
        {"Game", "PhantomGalaxy", 10, -1},
        {"Game", "CosmosGardenGalaxy", 11, -1},
        {"Game", "AstroDome", 12, -1},
        //
        {"Game", "IceVolcanoGalaxy", 14, -1},
        {"Game", "HeavenlyBeachGalaxy", 15, -1},
        {"Game", "KoopaBattleVs1Galaxy", 16, -1},
        {"Game", "AstroGalaxy", 17, -1},
        {"Game", "HoneyBeeKingdomGalaxy", 18, 3},
        {"Game", "ReverseKingdomGalaxy", 19, 4},
        {"Game", "FileSelect", 20, -1},
        {"Game", "SurfingLv1Galaxy", 21, -1},
        {"Game", "SandClockGalaxy", 22, -1},
        {"Game", "TriLegLv1Galaxy", 23, -1},
        {"Game", "TriLegLv2Galaxy", 24, -1},
        {"Game", "KoopaJrShipLv1Galaxy", 25, -1},
        //
        {"Game", "KoopaJrDriverGalaxy", 27, -1},
        {"Game", "FactoryGalaxy", 28, -1},
        {"Game", "HoneyBeeExGalaxy", 29, -1},
        {"Game", "TamakoroExLv1Galaxy", 30, -1},
        {"Game", "TamakoroExLv2Galaxy", 31, -1},
        {"Game", "HeavensDoorGalaxy", 32, -1},
        {"Game", "FloaterOtaKingGalaxy", 33, -1},
        {"Game", "TransformationExGalaxy", 34, -1},
        {"Game", "LibraryRoom", 35, -1},
        {"Game", "PeachCastleGardenGalaxy", 36, -1},
        {"Game", "FishTunnelGalaxy", 37, -1},
        {"Game", "TearDropGalaxy", 38, -1},
        {"Game", "EpilogueDemoStage", 39, -1},
        {"Game", "KoopaBattleVs3Galaxy", 40, -1},
        {"Game", "DarkRoomGalaxy", 41, -1},
        {"Game", "PeachCastleFinalGalaxy", 42, -1},
        {"Game", "SkullSharkGalaxy", 43, -1},
        //
        {"Game", "KoopaBattleVs2Galaxy", 16, -1},
        {"Game", "SurfingLv2Galaxy", 21, -1},
        {"Game", "BeltConveyerExGalaxy", 30, -1},
        {"Game", "CocoonExGalaxy", 0, -1},
        {"Game", "TeresaMario2DGalaxy", 0, -1},
        {"Game", "BreakDownPlanetGalaxy", 0, -1},
        {"Game", "CubeBubbleExLv1Galaxy", 0, -1},
        {"Game", "CubeBubbleExLv2Galaxy", 0, -1},
        {"Game", "FlipPanelExGalaxy", 0, -1},
        {"Game", "SnowCapsuleGalaxy", 0, -1},
        {"Game", "E3HeavenlyBeachGalaxy", 15, -1},

    };

    static const s8 cScenarioNoToWaveSet[][6] = {
        {0, -1, 1, 0, -1, -1},    // EggStarGalaxy
        {-1, 2, 3, -1, -1, -1},   // StarDustGalaxy
        {4, 10, 5, 4, -1, -1},    // OceanPhantomCaveGalaxy
        {-1, -1, 7, 6, -1, -1},   // HoneyBeeKingdomGalaxy
        {-1, -1, -1, 6, -1, -1},  // ReverseKingdomGalaxy
        {9, 9, 9, 8, 9, 9},       // OceanRingGalaxy
    };

    static const s8 cWaveSetInfo[][2] = {
        {0, 0},   // dummy
        {0, 1},   // HellProminenceGalaxy
        {3, 2},   // OceanRingGalaxy
        {0, 3},   // OceanFloaterLandGalaxy
        {0, 4},   // OceanPhantomCaveGalaxy
        {0, 5},   //
        {0, 6},   // EggStarGalaxy
        {0, 7},   // StarDustGalaxy
        {0, 8},   // BattleShipGalaxy
        {0, 9},   // CannonFleetGalaxy
        {0, 10},  // PhantomGalaxy
        {0, 11},  // CosmosGardenGalaxy
        {1, 12},  // AstroDome
        {0, 13},  //
        {0, 14},  // IceVolcanoGalaxy
        {3, 15},  // HeavenlyBeachGalaxy E3HeavenlyBeachGalaxy
        {0, 16},  // KoopaBattleVs1Galaxy KoopaBattleVs2Galaxy
        {0, 17},  // AstroGalaxy
        {0, 18},  // HoneyBeeKingdomGalaxy
        {0, 19},  // ReverseKingdomGalaxy
        {1, 20},  // FileSelect
        {3, 21},  // SurfingLv1Galaxy SurfingLv2Galaxy
        {0, 22},  // SandClockGalaxy
        {0, 23},  // TriLegLv1Galaxy
        {0, 24},  // TriLegLv2Galaxy
        {0, 25},  // KoopaJrShipLv1Galaxy
        {0, 26},  //
        {0, 27},  // KoopaJrDriverGalaxy
        {0, 28},  // FactoryGalaxy
        {0, 29},  // HoneyBeeExGalaxy
        {0, 30},  // TamakoroExLv1Galaxy BeltConveyerExGalaxy
        {0, 31},  // TamakoroExLv2Galaxy
        {0, 32},  // HeavensDoorGalaxy
        {0, 33},  // FloaterOtaKingGalaxy
        {0, 34},  // TransformationExGalaxy
        {2, 35},  // LibraryRoom
        {0, 36},  // PeachCastleGardenGalaxy
        {0, 37},  // FishTunnelGalaxy
        {0, 38},  // TearDropGalaxy
        {0, 39},  // EpilogueDemoStage
        {0, 40},  // KoopaBattleVs3Galaxy
        {0, 41},  // DarkRoomGalaxy
        {0, 42},  // PeachCastleFinalGalaxy
        {0, 43},  // SkullSharkGalaxy
    };

    static const s8 cBgmWaveSet[][2] = {
        {-1, -1},  //
        {23, -1},  // AstroDome FileSelect
        {23, -1},  // LibraryRoom
        {10, -1},  // OceanRingGalaxy HeavenlyBeachGalaxy E3HeavenlyBeachGalaxy SurfingLv1Galaxy SurfingLv2Galaxy
    };

    static const s8 cSeWaveSet[][4] = {
        {-1, -1, -1, -1},  // dummy
        {13, 16, 20, 33},  // HellProminenceGalaxy
        {14, 9, -1, -1},   // OceanRingGalaxy
        {14, 41, 9, 35},   // OceanFloaterLandGalaxy
        {14, 9, -1, -1},   // OceanPhantomCaveGalaxy
        {15, 17, 12, -1},  //
        {14, 46, -1, -1},  // EggStarGalaxy
        {13, -1, -1, -1},  // StarDustGalaxy
        {12, 21, 16, 46},  // BattleShipGalaxy
        {21, 16, 9, -1},   // CannonFleetGalaxy
        {39, 46, -1, -1},  // PhantomGalaxy
        {22, 29, 14, 13},  // CosmosGardenGalaxy
        {26, 37, 38, -1},  // AstroDome
        {24, 25, -1, -1},  //
        {13, 14, 35, 40},  // IceVolcanoGalaxy
        {14, 9, 35, -1},   // HeavenlyBeachGalaxy E3HeavenlyBeachGalaxy
        {27, -1, -1, -1},  // KoopaBattleVs1Galaxy KoopaBattleVs2Galaxy
        {38, 37, 46, -1},  // AstroGalaxy
        {24, 36, 46, -1},  // HoneyBeeKingdomGalaxy
        {24, 36, -1, -1},  // ReverseKingdomGalaxy
        {26, 47, -1, -1},  // FileSelect
        {9, 32, 35, -1},   // SurfingLv1Galaxy SurfingLv2Galaxy
        {30, 14, -1, -1},  // SandClockGalaxy
        {12, 21, -1, -1},  // TriLegLv1Galaxy
        {12, -1, -1, -1},  // TriLegLv2Galaxy
        {21, -1, -1, -1},  // KoopaJrShipLv1Galaxy
        {21, -1, -1, -1},  //
        {21, -1, -1, -1},  // KoopaJrDriverGalaxy
        {31, 13, -1, -1},  // FactoryGalaxy
        {24, 36, -1, -1},  // HoneyBeeExGalaxy
        {33, -1, -1, -1},  // TamakoroExLv1Galaxy BeltConveyerExGalaxy
        {33, -1, -1, -1},  // TamakoroExLv2Galaxy
        {44, 38, 37, 42},  // HeavensDoorGalaxy
        {17, 21, -1, -1},  // FloaterOtaKingGalaxy
        {30, -1, -1, -1},  // TransformationExGalaxy
        {26, 37, 38, -1},  // LibraryRoom
        {37, 43, -1, -1},  // PeachCastleGardenGalaxy
        {14, 9, 19, 35},   // FishTunnelGalaxy
        {14, 9, 35, -1},   // TearDropGalaxy
        {45, 37, -1, -1},  // EpilogueDemoStage
        {27, 21, -1, -1},  // KoopaBattleVs3Galaxy
        {22, -1, -1, -1},  // DarkRoomGalaxy
        {37, 43, 36, 35},  // PeachCastleFinalGalaxy
        {14, 19, 9, -1},   // SkullSharkGalaxy
    };

    static const s8 cSeScenarioWaveSet[][4] = {
        {20, -1, -1, -1},  // EggStarGalaxy[0,3]
        {17, -1, -1, -1},  // EggStarGalaxy[1]
        {18, -1, -1, -1},  // StarDustGalaxy[1]
        {15, -1, -1, -1},  // StarDustGalaxy[2]
        {18, -1, -1, -1},  // OceanPhantomCaveGalaxy[0,3]
        {35, -1, -1, -1},  // OceanPhantomCaveGalaxy[2]
        {40, -1, -1, -1},  // HoneyBeeKingdomGalaxy[3] ReverseKingdomGalaxy[3]
        {25, -1, -1, -1},  // HoneyBeeKingdomGalaxy[2]
        {40, -1, -1, -1},  // OceanRingGalaxy[3]
        {35, 36, -1, -1},  // OceanRingGalaxy[0,1,2,4,5]
        {41, -1, -1, -1},  // OceanPhantomCaveGalaxy[1]
    };
};  // namespace

AudSceneMgr::AudSceneMgr(JAUSectionHeap* pSectionHeap)
    : _4(), mSeWaveSetId(), mSeScenarioWaveSetId(-1), mBgmWaveSetId(), mPlayerMode(), mPrevPlayerMode(), mIsNewPlayerMode(), _1D() {
    mSectionHeap = pSectionHeap;
}

bool AudSceneMgr::isLoadDoneSystemInit() {
    return mSectionHeap->isWaveLoaded(7, 0);
}

void AudSceneMgr::loadStaticResource() {
    mSectionHeap->loadWaveArc(0);
    mSectionHeap->loadWaveArc(1);
    mSectionHeap->loadWaveArc(2);
    mSectionHeap->loadWaveArc(3);
    mSectionHeap->loadWaveArc(4);
    mSectionHeap->loadWaveArc(5);
    mSectionHeap->loadWaveArc(6);

    mSectionHeap->loadWaveArc(8);

    mSectionHeap->loadWaveArc(11);
}

bool AudSceneMgr::isLoadDoneStaticResource() {
    bool loadDone = false;

    bool isLoaded0 = mSectionHeap->isWaveLoaded(0, 0);
    bool isLoaded1 = mSectionHeap->isWaveLoaded(1, 0);
    bool isLoaded2 = mSectionHeap->isWaveLoaded(2, 0);
    bool isLoaded3 = mSectionHeap->isWaveLoaded(3, 0);
    bool isLoaded4 = mSectionHeap->isWaveLoaded(4, 0);
    bool isLoaded5 = mSectionHeap->isWaveLoaded(5, 0);
    bool isLoaded6 = mSectionHeap->isWaveLoaded(6, 0);
    bool isLoaded8 = mSectionHeap->isWaveLoaded(8, 0);
    bool isLoaded11 = mSectionHeap->isWaveLoaded(11, 0);

    if (isLoaded0 && isLoaded1 && isLoaded2 && isLoaded3 && isLoaded4 && isLoaded5 && isLoaded6 && isLoaded8 && isLoaded11) {
        loadDone = true;
    }

    return loadDone;
}

void AudSceneMgr::loadStageResource(const char* pSceneName, const char* pStageName) {
    loadStageResource(findSceneSoundInfo(pSceneName, pStageName));
}

void AudSceneMgr::loadStageResource(const AudSceneSoundInfo* pSoundInfo) {
    mIsNewPlayerMode = false;
    if (pSoundInfo == nullptr) {
        return;
    }

    s32 bgmWaveSetId = ::cWaveSetInfo[pSoundInfo->mStageId][0];
    s32 seWaveSetId = ::cWaveSetInfo[pSoundInfo->mStageId][1];
    const s8* bgmWaveSet = ::cBgmWaveSet[bgmWaveSetId];
    const s8* seWaveSet = ::cSeWaveSet[seWaveSetId];

    if (mPlayerMode != mPrevPlayerMode) {
        mIsNewPlayerMode = true;
    }

    eraseLastSeScenarioWaveSet();
    if (seWaveSetId != mSeWaveSetId || mIsNewPlayerMode == true) {
        eraseLastBgmWaveSet();
        eraseLastSeWaveSet();
        if (mIsNewPlayerMode) {
            mSectionHeap->eraseWaveArc(34, 2);
            mSectionHeap->eraseWaveArc(34, 4);
            loadPlayerResource();
        }

        loadWaveSet(seWaveSet, 4);
        loadWaveSet(bgmWaveSet, 2);

    } else if (bgmWaveSetId != mBgmWaveSetId) {
        eraseLastBgmWaveSet();
        loadWaveSet(bgmWaveSet, 2);
    }

    mBgmWaveSetId = bgmWaveSetId;
    mPrevPlayerMode = mPlayerMode;
    mSeWaveSetId = seWaveSetId;
}

bool AudSceneMgr::isLoadDoneStageResource() {
    const s8* bgmWaveSet = ::cBgmWaveSet[mBgmWaveSetId];
    const s8* seWaveSet = ::cSeWaveSet[mSeWaveSetId];

    for (s32 i = 0; i < 2; i++) {
        if (bgmWaveSet[i] < 0) {
            continue;
        }

        if (!mSectionHeap->isWaveLoaded(bgmWaveSet[i], 0)) {
            return false;
        }
    }

    for (s32 i = 0; i < 4; i++) {
        if (seWaveSet[i] < 0) {
            continue;
        }

        if (!mSectionHeap->isWaveLoaded(seWaveSet[i], 0)) {
            return false;
        }
    }

    if (mIsNewPlayerMode && !isPlayerResourceLoaded()) {
        return false;
    }

    return true;
}

void AudSceneMgr::loadScenarioResource(const char* pSceneName, const char* pStageName, s32 scenarioNo) {
    loadScenarioResource(findSceneSoundInfo(pSceneName, pStageName), scenarioNo);
}

void AudSceneMgr::loadScenarioResource(const AudSceneSoundInfo* pSoundInfo, s32 scenarioNo) {
    // FIXME: reg load
    // https://decomp.me/scratch/yx2Kb

    if (scenarioNo <= 0) {
        mSeScenarioWaveSetId = -1;
        return;
    }

    if (pSoundInfo == nullptr) {
        return;
    }

    if (pSoundInfo->mScenarioId < 0) {
        mSeScenarioWaveSetId = -1;
        return;
    }

    const s8* scenarioSet = ::cScenarioNoToWaveSet[pSoundInfo->mScenarioId];
    s8 scenarioWaveSetId = scenarioSet[scenarioNo - 1];
    if (scenarioWaveSetId < 0) {
        mSeScenarioWaveSetId = -1;
        return;
    }

    loadWaveSet(::cSeScenarioWaveSet[scenarioWaveSetId], 4);
    mSeScenarioWaveSetId = scenarioWaveSetId;
}

bool AudSceneMgr::isLoadDoneScenarioResource() {
    if (mSeScenarioWaveSetId < 0) {
        return true;
    }

    const s8* waveSet = ::cSeScenarioWaveSet[mSeScenarioWaveSetId];
    for (s32 i = 0; i < 4; i++) {
        if (waveSet[i] < 0) {
            continue;
        }

        if (!mSectionHeap->isWaveLoaded(waveSet[i], 0)) {
            return false;
        }
    }

    return true;
}

void AudSceneMgr::startScene() {
    _4 = 0;
    AudWrap::getSystem()->_82A = false;
    AudWrap::getSystem()->_82B = false;
    AudWrap::getSystem()->_82C = false;
    AudWrap::getSystem()->initSceneVolume();
    AudEffector* effector = AudWrap::getSystem()->mAudEffector;
    if (effector != nullptr) {
        effector->initParams();
    }
    _1D = false;
    SpkSystem::reconnect(-1);
}

const AudSceneSoundInfo* AudSceneMgr::findSceneSoundInfo(const char* pSceneName, const char* pStageName) {
    for (s32 i = 0; i < ARRAY_SIZE(::cSceneInfo); i++) {
        if (strcmp(pSceneName, ::cSceneInfo[i].mSceneName) == 0 && strcmp(pStageName, ::cSceneInfo[i].mStageName) == 0) {
            return &::cSceneInfo[i];
        }
    }

    if (strcmp(pSceneName, "Game") == 0) {
        return &::cSceneInfo[0];
    }

    return nullptr;
}

void AudSceneMgr::eraseLastBgmWaveSet() {
    const s8* bgmWaveSet = ::cBgmWaveSet[mBgmWaveSetId];

    for (s32 i = 2 - 1; i >= 0; i--) {
        if (bgmWaveSet[i] >= 0) {
            mSectionHeap->eraseWaveArc(bgmWaveSet[i]);
        }
    }
}

void AudSceneMgr::eraseLastSeWaveSet() {
    const s8* seWaveSet = ::cSeWaveSet[mSeWaveSetId];

    for (s32 i = 4 - 1; i >= 0; i--) {
        if (seWaveSet[i] >= 0) {
            mSectionHeap->eraseWaveArc(seWaveSet[i]);
        }
    }
}

void AudSceneMgr::eraseLastSeScenarioWaveSet() {
    if (mSeScenarioWaveSetId < 0) {
        return;
    }

    const s8* seWaveSet = ::cSeScenarioWaveSet[mSeScenarioWaveSetId];

    for (s32 i = 4 - 1; i >= 0; i--) {
        if (seWaveSet[i] >= 0) {
            mSectionHeap->eraseWaveArc(seWaveSet[i]);
        }
    }
}

void AudSceneMgr::loadWaveSet(const s8* pWaveSet, s32 numItems) {
    for (s32 i = 0; i < numItems; i++) {
        if (pWaveSet[i] >= 0) {
            mSectionHeap->loadWaveArc(pWaveSet[i]);
        }
    }
}

void AudSceneMgr::setPlayerModeMario() {
    mPlayerMode = PlayerMode_Mario;
}

void AudSceneMgr::setPlayerModeLuigi() {
    mPlayerMode = PlayerMode_Luigi;
}

bool AudSceneMgr::loadPlayerResource() {
    switch (mPlayerMode) {
    case PlayerMode_Mario:
        return mSectionHeap->loadWaveArc(34, 2);
    case PlayerMode_Luigi:
        return mSectionHeap->loadWaveArc(34, 4);
    }
    return false;
}

bool AudSceneMgr::isPlayerResourceLoaded() {
    switch (mPlayerMode) {
    case PlayerMode_Mario:
        return mSectionHeap->isWaveLoaded(34, 1);
    case PlayerMode_Luigi:
        return mSectionHeap->isWaveLoaded(34, 2);
    }
    return true;
}
