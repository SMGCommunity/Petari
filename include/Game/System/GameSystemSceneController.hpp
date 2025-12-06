#pragma once

#include <revolution/types.h>

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

    void setScene(const char*);
    void setStage(const char*);
    void setStartIdInfo(const JMapIdInfo&);

    /* 0x00 */ char mScene[32];
    /* 0x20 */ char mStage[32];
    /* 0x40 */ s32 mCurrentScenarioNo;
    /* 0x44 */ s32 mCurrentSelectedScenarioNo;
    /* 0x48 */ JMapIdInfo* mStartIdInfo;
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
    void exeNormal();
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
    const Nerve* getNextNerveOnResetProcessing() const;
    bool tryDestroyFileCacheHeap(bool) NO_INLINE;
    void requestChangeNerve(const Nerve*);

    /* 0x00 */ SceneControlInfo _0;
    /* 0x4C */ SceneControlInfo _4C;
    /* 0x98 */ Spine* _98;
    /* 0x9C */ const Nerve* _9C;
    /* 0xA0 */ bool _A0;
    /* 0xA4 */ ScenarioDataParser* mScenarioParser;
    /* 0xA8 */ NameObjHolder* mObjHolder;
    /* 0xAC */ Scene* mScene;
    /* 0xB0 */ SceneInitializeState mSceneInitializeState;
    /* 0xB4 */ IntermissionScene* mIntermissionScene;
    /* 0xB8 */ PlayTimerScene* mPlayTimerScene;
    /* 0xBC */ ScenarioSelectScene* mScenarioSelectScene;
};
