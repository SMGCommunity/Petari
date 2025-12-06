#pragma once

#include "Game/Screen/LayoutActor.hpp"

class PauseMenu;

namespace MR {
    class FunctorBase;
};

class GamePauseSequence : public LayoutActor {
public:
    enum MenuType { ActivePause = 0, SceneInformation = 1 };

    GamePauseSequence();

    virtual void init(const JMapInfoIter&);

    void initWindowMenu(const MR::FunctorBase&);
    void startPause(MenuType);
    void deactivate();
    void exeDeactive();
    void exeActivePauseMenu();
    void exeActivePowerStarList();
    void exeSceneInformation();

    /* 0x20 */ MenuType mMenuType;
    /* 0x24 */ PauseMenu* mPauseMenu;
    /* 0x28 */ MR::FunctorBase* mWindowMenuFunc;
};
