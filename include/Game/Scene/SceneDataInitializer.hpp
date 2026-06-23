#pragma once

#include "Game/NameObj/NameObj.hpp"

class StageDataHolder;
class StageFileLoader;

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