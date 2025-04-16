#pragma once

#include "Game/System/NerveExecutor.hpp"

class GameScene;
class PauseButtonCheckerInGame;

class GameScenePauseControl : public NerveExecutor {
public:
    GameScenePauseControl(GameScene *);

    void registerNervePauseMenu(const Nerve *);
    void requestPauseMenuOff();
    void exeNormal();
    bool tryStartPauseMenu();
    

    PauseButtonCheckerInGame* mPauseChecker;        // 0xC
    bool mPauseMenuOff;                             // 0x10
    const Nerve* mPauseMenuNerve;                   // 0x14
};
