#pragma once

#include "Game/System/NerveExecutor.hpp"

class GameScene;
class PauseButtonCheckerInGame;

class GameScenePauseControl : public NerveExecutor {
public:
    GameScenePauseControl(GameScene*);

    virtual ~GameScenePauseControl();

    void registerNervePauseMenu(const Nerve*);
    void requestPauseMenuOff();
    void exeNormal();
    bool tryStartPauseMenu();

    /* 0x08 */ GameScene* mScene;
    /* 0x0C */ PauseButtonCheckerInGame* mPauseChecker;
    /* 0x10 */ bool mPauseMenuOff;
    /* 0x14 */ const Nerve* mPauseMenuNerve;
};
