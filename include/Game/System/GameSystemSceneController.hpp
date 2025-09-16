#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include <revolution.h>

class GameScene;
class IntermissionScene;
class JMapIdInfo;
class NameObjHolder;
class NameObjListExecutor;
class Nerve;
class PlayTimerScene;
class ScenarioDataParser;
class ScenarioSelectScene;
class Scene;
class SceneObjHolder;
class Spine;

class SceneControlInfo {
public:
    SceneControlInfo();

    void setScene(const char *);
    void setStage(const char *);
    void setStartIdInfo(const JMapIdInfo &);

    char mScene[32];                        // 0x0
    char mStage[32];                        // 0x20
    u32 mCurrentScenarioNo;                 // 0x40
    u32 mCurrentSelectedScenarioNo;         // 0x44
    JMapIdInfo* _48;
};

enum SceneInitializeState {
    State_NotInit = 0,
    State_Init = 1,
    State_PlacementPlayer = 2,
    State_PlacementHighPrio = 3,
    State_Placement = 4,
    State_AfterPlacement = 5,
    State_End = 6
};

class GameSystemSceneController {
public:
    GameSystemSceneController();

    void initAfterStationedResourceLoaded();
    void requestChangeScene();
    void checkRequestAndChangeScene();
    void initializeScene();
    void destroyScene();
    bool isStopSound() const;
    bool isReadyToStartScene() const;
    bool isFirstUpdateSceneNerveNormal() const;
    void startScene();
    void updateScene();
    void updateSceneDuringResetProcessing();
    void calcAnimScene();
    void drawScene();
    NameObjListExecutor* getNameObjListExecutor() const;
    SceneObjHolder* getSceneObjHolder() const;
    bool isExistSceneObjHolder() const;
    s32 getCurrentScenarioNo() const;
    s32 getCurrentSelectedScenarioNo() const;
    void setSceneInitializeState(SceneInitializeState);
    bool isSceneInitializeState(SceneInitializeState) const;
    void startScenarioSelectScene();
    void startScenarioSelectSceneBackground();
    void setCurrentScenarioNo(s32, s32);
    void resetCurrentScenarioNo();
    bool isScenarioDecided() const;
    void loadScenarioWaveData();
    bool isLoadDoneScenarioWaveData() const;
    void exeNotInitialized();
    void exeWaitDrawDoneScene();
    void exeDestroyScene();
    void exeChangeWaveBank();
    void exeInitializeScene();
    void exeInvalidateSystemWipe();
    void exeDestroyed();
    void exeReadyToStartScene();
    void prepareReset();
    bool isPreparedReset();
    void restartGameAfterResetting();
    bool isExistRequest() const;
    bool isSameAtNextSceneAndStage() const;
    void updateSceneControlInfo();
    Scene* getCurrentSceneForExecute() const;
    Nerve* getNextNerveOnResetProcessing() const;
    bool tryDestroyFileCacheHeap(bool);
    bool requestChangeNerve(const Nerve *);

    SceneControlInfo _0;
    SceneControlInfo _4C;
    Spine* _98;
    Nerve* _9C;
    bool _A0;
    ScenarioDataParser* mScenarioParser;        // 0xA4
    NameObjHolder* mObjHolder;                  // 0xA8
    GameScene* mGameScene;                      // 0xAC
    SceneInitializeState mInitState;            // 0xB0
    IntermissionScene* mIntermissionScene;      // 0xB4
    PlayTimerScene* mPlayTimerScene;            // 0xB8
    ScenarioSelectScene* mScenarioScene;        // 0xBC
};

namespace NrvGameSystemSceneController {
    NERVE(GameSystemSceneControllerNotInitialized)
    NERVE(GameSystemSceneControllerNormal)
    NERVE(GameSystemSceneControllerChangeWaveBank)
    NERVE(GameSystemSceneControllerInitializeScene)
    NERVE(GameSystemSceneControllerInvalidateSystemWipe)
    NERVE(GameSystemSceneControllerWaitDrawDoneScene)
    NERVE(GameSystemSceneControllerDestroyScene)
    NERVE(GameSystemSceneControllerReadyToStartScene)
    NERVE(GameSystemSceneControllerWaitDrawDoneSceneForDestroy)
    NERVE(GameSystemSceneControllerDestroySceneForDestroy)
    NERVE(GameSystemSceneControllerDestroyed)
}
