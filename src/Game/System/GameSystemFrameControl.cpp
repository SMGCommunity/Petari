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
        MainLoopFramework::getManager()->setTickRateFromFrame(1);
    }
    else {
        MainLoopFramework* pFramework = MainLoopFramework::getManager();
        pFramework->mRetraceCount = 1;
        pFramework->mTickDuration = 0;
    }
}
