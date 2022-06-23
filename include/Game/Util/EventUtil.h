#pragma once

#include <revolution.h>

namespace MR {
    bool isOnGameEventFlagRosettaTalkAboutTicoInTower();

    bool isPowerStarGetDemoWithLuigiCurrentGalaxyAndScenario(s32);

    bool isOnGameEventFlagChildsRoom();
    bool isOnGameEventFlagLibraryRoom();

    bool hasGrandStar(int);

    bool hasPowerStarInCurrentStage(s32);

    bool hasPowerStarAtResultSequence();

    bool hasPowerStarInCurrentStageWithDeclarer(const char *, s32);
    bool isPowerStarGreenInCurrentStageWithDeclarer(const char *, s32);
    bool isPowerStarRedInCurrentStageWithDeclarer(const char* , s32);

    bool isPowerStarGreenInCurrentStage(s32);
    bool isPowerStarRedInCurrentStage(s32);

    bool isPowerStarGreenAtResultSequence();
    bool isPowerStarRedAtResultSequence();

    bool isGrandStarInCurrentStageWithDeclarer(const char *, s32);

    bool isPowerStarGetDemoWithLuigiCurrentGalaxy();

    bool isGalaxyRedCometAppearInCurrentStage();
    bool isGalaxyDarkCometAppearInCurrentStage();
    bool isGalaxyRedCometAppearInCurrentStage();
    bool isGalaxyGhostCometAppearInCurrentStage();
    bool isGalaxyQuickCometAppearInCurrentStage();
    bool isGalaxyBlackCometAppearInCurrentStage();

    bool isPlayerLuigi();

    void explainOneUpIfAtFirst();

    void incPlayerLeft();
};
