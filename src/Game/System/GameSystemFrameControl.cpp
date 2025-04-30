#include "Game/System/GameSystemFrameControl.hpp"
#include "Game/System/MainLoopFramework.hpp"

GameSystemFrameControl::GameSystemFrameControl() {
    mIs60Fps = true;
    setMovement60fps();
}

void GameSystemFrameControl::setMovement60fps() {
    mIs60Fps = true;
    if (VIGetTvFormat() == 1) {
        MainLoopFramework::sManager->setTickRateFromFrame(1);
    }
    else {
        MainLoopFramework* fw = MainLoopFramework::get();
        fw->_10 = 1;
        fw->_14 = 0;
    }
}
