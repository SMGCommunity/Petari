#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry.hpp>

class ScenarioTitle;

class GameSceneScenarioOpeningCameraState : public NerveExecutor {
public:
    GameSceneScenarioOpeningCameraState();

    virtual ~GameSceneScenarioOpeningCameraState();

    void update();
    bool isDone() const;
    void start();
    void end();
    void exePlay();
    bool trySkipTrigger() const;
    inline void exeWait();

    TPos3f mBaseMtx;                        // 0x08
    ScenarioTitle* mScenarioTitle;          // 0x38
};
