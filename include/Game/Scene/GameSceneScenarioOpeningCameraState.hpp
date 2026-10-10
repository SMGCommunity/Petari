#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ScenarioTitle;

class GameSceneScenarioOpeningCameraState : public NerveExecutor {
public:
    GameSceneScenarioOpeningCameraState();

    void update();
    bool isDone() const;
    void start();
    void end();
    void exeWait();
    void exePlay();
    bool trySkipTrigger() const;

    /* 0x08 */ TPos3f mBaseMtx;
    /* 0x38 */ ScenarioTitle* mScenarioTitle;
};
