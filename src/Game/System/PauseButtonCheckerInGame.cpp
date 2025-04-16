#include "Game/System/PauseButtonCheckerInGame.hpp"
#include "Game/Util.hpp"

PauseButtonCheckerInGame::PauseButtonCheckerInGame() {
    _0 = 0;
    _4 = 0;
}

void PauseButtonCheckerInGame::update() {
    if (MR::testCorePadButtonPlus(0)) {
        _0++;
    }
    else {
        _0 = 0;
    }

    if (MR::testCorePadButtonMinus(0)) {
        _4++;
    }
    else {
        _4 = 0;
    }
}

bool PauseButtonCheckerInGame::isPermitToPlusPause() const {
    bool ret = false;
    if (isPermitToPause() && _0 == 12) {
        ret = true;
    }

    return ret;
}

bool PauseButtonCheckerInGame::isPermitToMinusPause() const {
    bool ret = false;
    if (isPermitToPause() && _4 == 12) {
        ret = true;
    }

    return ret;
}

bool PauseButtonCheckerInGame::isPermitToPause() const {
    return MR::isOperatingWPad(0) == false;
}
