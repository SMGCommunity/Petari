#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/StageDataHolder.hpp"
#include "Game/Scene/StageFileLoader.hpp"

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

    StageFileLoader* mFileLoader;  // 0xC
    StageDataHolder* mDataHolder;  // 0x10
};