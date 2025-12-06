#pragma once

#include <revolution/types.h>

class JMapIdInfo;

namespace MR {
    void requestChangeScene(const char*);
    void requestChangeSceneTitle();
    void requestChangeStageInGameAfterLoadingGameData();
    void requestChangeStageAfterStageClear();
    void requestChangeStageAfterMiss();
    void requestChangeStageInGameMoving(const char*, s32, const JMapIdInfo&);
    void requestChangeStageInGameMoving(const char*, s32);
    void requestChangeSceneAfterGameOver();
    void requestChangeSceneAfterBoot();
    void requestChangeStageGoBackAstroDome();
    void requestStartScenarioSelect(const char*);
    void requestStartScenarioSelectForComet(const char*, s32);
    bool hasRetryGalaxySequence();
    bool isExecScenarioStarter();
    void requestPowerStarGetDemo();
    void requestGrandStarGetDemo();
    void requestStartGameOverDemo();
    void requestEndGameOverDemo();
    void requestEndMissDemo();
    void requestShowGalaxyMap();
    void executeOnWelcomeAndRetry();
    void requestGoToAstroGalaxy(s32);
    void requestGoToAstroDomeFromAstroGalaxy(s32, s32);
};  // namespace MR
