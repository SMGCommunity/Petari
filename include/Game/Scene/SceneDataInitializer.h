#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Scene/StageFileLoader.h"
#include "Game/Scene/StageDataHolder.h"

class SceneDataInitializer : public NameObj {
public:
    SceneDataInitializer();

    virtual ~SceneDataInitializer();

    void startStageFileLoad();
    void startStageFileLoadAfterScenarioSelected();
    void waitDoneStageFileLoad();
    void startActorFileLoadCommon();
    void startActorFileLoadScenario();
    void startActorPlacement();
    void initAfterScenarioSelected();

    StageFileLoader* mFileLoader;   // _C
    StageDataHolder* mDataHolder;   // _10
};