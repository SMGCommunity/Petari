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

    /* 0x0C */ StageFileLoader* mFileLoader;
    /* 0x10 */ StageDataHolder* mDataHolder;
};
