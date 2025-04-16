#pragma once

#include "Game/Util/JMapIdInfo.hpp"

namespace MR {
    void requestChangeStageInGameMoving(const char *, s32);
    void requestChangeStageInGameMoving(const char *, s32, const JMapIdInfo &);

    void requestStartScenarioSelect(const char *);
    void requestGoToAstroDomeFromAstroGalaxy(s32, s32);
    void requestStartScenarioSelect(const char *);

    void requestGoToAstroGalaxy(s32);

    bool isExecScenarioStarter();

    void executeOnWelcomeAndRetry();

    void requestChangeStageAfterStageClear();
};
