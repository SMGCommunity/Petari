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

    TPos3f mBaseMtx;                // 0x08
    ScenarioTitle* mScenarioTitle;  // 0x38
};
