#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util.hpp"

class PauseMenu;

class GamePauseSequence : public LayoutActor {
public:
    enum MenuType {
        ActivePause = 0,
        SceneInformation = 1
    };

    GamePauseSequence();

    virtual ~GamePauseSequence();
    virtual void init(const JMapInfoIter &);

    void initWindowMenu(const MR::FunctorBase &);
    void startPause(MenuType);
    void deactivate();
    void exeActivePauseMenu();
    void exeActivePowerStarList();
    inline void exeSceneInformation();
    inline void exeDeactive();

    MenuType mMenuType;                     // 0x20
    PauseMenu* mPauseMenu;                  // 0x24
    MR::FunctorBase* mWindowMenuFunc;       // 0x28
};
