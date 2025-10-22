#include "Game/System/PauseButtonCheckerInGame.hpp"
#include "Game/Util/GamePadUtil.hpp"

namespace {
    static const s32 sHoldCounterMax = 12;
};

PauseButtonCheckerInGame::PauseButtonCheckerInGame() :
    mPlusHoldFrame(0),
    mMinusHoldFrame(0)
{
    
}

void PauseButtonCheckerInGame::update() {
    if (MR::testCorePadButtonPlus(0)) {
        mPlusHoldFrame++;
    }
    else {
        mPlusHoldFrame = 0;
    }

    if (MR::testCorePadButtonMinus(0)) {
        mMinusHoldFrame++;
    }
    else {
        mMinusHoldFrame = 0;
    }
}

bool PauseButtonCheckerInGame::isPermitToPlusPause() const {
    return isPermitToPause() && mPlusHoldFrame == sHoldCounterMax;
}

bool PauseButtonCheckerInGame::isPermitToMinusPause() const {
    return isPermitToPause() && mMinusHoldFrame == sHoldCounterMax;
}

bool PauseButtonCheckerInGame::isPermitToPause() const {
    return !MR::isOperatingWPad(0);
}
