#include "Game/System/GameSystemFrameControl.hpp"
#include "Game/System/MainLoopFramework.hpp"

GameSystemFrameControl::GameSystemFrameControl() :
    mIs60fps(true)
{
    setMovement60fps();
}

void GameSystemFrameControl::setMovement60fps() {
    mIs60fps = true;

    if (VIGetTvFormat() == VI_PAL) {
        MainLoopFramework::get()->setTickRateFromFrame(1);
    }
    else {
        MainLoopFramework* pFramework = MainLoopFramework::get();

        pFramework->_10 = 1;
        pFramework->_14 = 0;
    }
}
