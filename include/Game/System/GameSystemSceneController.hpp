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
    /// @brief Creates a new `SceneControlInfo`.
    SceneControlInfo();

    void setScene(const char*);
    void setStage(const char*);
    void setStartIdInfo(const JMapIdInfo&);

    /* 0x00 */ char mScene[32];
    /* 0x20 */ char mStage[32];
    /* 0x40 */ s32 mScenarioNo;
    /* 0x44 */ s32 mSelectedScenarioNo;
    /* 0x48 */ JMapIdInfo* mStartIdInfo;
};

enum SceneInitializeState {
    SceneInitializeState_NotInit,
    SceneInitializeState_Init,
    SceneInitializeState_PlacementPlayer,
    SceneInitializeState_PlacementHighPriority,
    SceneInitializeState_Placement,
    SceneInitializeState_AfterPlacement,
    SceneInitializeState_End,
};

class GameSystemSceneController {
public:
    /// @brief Creates a new `GameSystemSceneController`.
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

    bool isCurrentNerve(const Nerve*) const;
    bool isNextNerve(const Nerve*) const;

    /* 0x00 */ SceneControlInfo mCurrSceneControlInfo;
    /* 0x4C */ SceneControlInfo mNextSceneControlInfo;
    /* 0x98 */ Spine* mSpine;
    /* 0x9C */ const Nerve* mNextNerve;
    /* 0xA0 */ bool mIsResetProcessing;
    /* 0xA4 */ ScenarioDataParser* mScenarioParser;
    /* 0xA8 */ NameObjHolder* mObjHolder;
    /* 0xAC */ Scene* mScene;
    /* 0xB0 */ SceneInitializeState mSceneInitializeState;
    /* 0xB4 */ IntermissionScene* mIntermissionScene;
    /* 0xB8 */ PlayTimerScene* mPlayTimerScene;
    /* 0xBC */ ScenarioSelectScene* mScenarioSelectScene;
};
