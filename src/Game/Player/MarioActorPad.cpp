#include "Game/Player/MarioActor.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void MarioActor::resetPadSwing() {
    _EF8 = 0;
    _EFC = 0;
    _F04 = 0;
    _F08 = 0;
    _F0C = false;
    _F00 = 0;
    _F1C = 10;
    _F1E = 10;
}

bool MarioActor::isRequestHipDrop() const {
    if (_934 || _38C != 0) {
        return false;
    }

    if (_935) {
        return false;
    }

    return checkButtonType(_F0E, false);
}

bool MarioActor::isRequestRush() const {
    if (!_EEB) {
        return false;
    }

    if (_946 != 0) {
        return false;
    }

    if (_EF6 != 0) {
        return false;
    }

    if (!mMario->isEnableRush()) {
        return false;
    }

    if (_935) {
        return false;
    }

    if (mPlayerMode == 5) {
        return false;
    }

    bool out = checkButtonType(_F10, false);
    if (mMario->_10._D) {
        if (!out) {
            mMario->_10._D = false;
        }

        out = false;
    }

    return out;
}

bool MarioActor::isRequestThrow() const {
    if (!_EEB) {
        return false;
    }

    if (!mMario->isEnableRush()) {
        return false;
    }

    bool out = checkButtonType(_F10, false);
    if (mMario->_10._D) {
        if (!out) {
            mMario->_10._D = false;
        }

        out = false;
    }

    return out;
}

bool MarioActor::isRequestSpin() const {
    if (_935) {
        return false;
    }

    if (isJumping() && mMario->_3BC > 6 && MR::isStarPointerPointing2P(this, "弱", true, false) && MR::testCorePadTriggerA(1)) {
        return true;
    }

    return checkButtonType(_F12, false);
}

bool MarioActor::isRequestJump() const {
    if (_934 || _38C != 0) {
        return false;
    }

    if (_935) {
        return false;
    }

    return checkButtonType(_F1A, false);
}

bool MarioActor::isRequestJump2P() const {
    if (mMario->isInputDisable()) {
        return false;
    }

    if (_3C0) {
        return false;
    }

    if (_934 || _38C != 0) {
        return false;
    }

    if (_935) {
        return false;
    }

    if (MR::isStarPointerPointing2P(this, "弱", true, false) && MR::testCorePadTriggerA(1)) {
        return true;
    }

    return false;
}

bool MarioActor::isKeepJump() const {
    return checkButtonType(5, false);
}

bool MarioActor::isKeepJump2P() const {
    if (mMario->isInputDisable()) {
        return false;
    }

    if (_3C0) {
        return false;
    }

    return MR::testCorePadButtonA(1);
}

bool MarioActor::checkButtonType(u16 type, bool isObeyDisable) const {
    if (!isObeyDisable && mMario->isInputDisable()) {
        return false;
    }

    if (_3C0) {
        return false;
    }

    switch (type) {
    case 2:
        if (_F0C || _F00) {
            return true;
        }
        break;

    case 0:
        if (_F0C) {
            return true;
        }
        break;

    case 1:
        if (_F00) {
            return true;
        }
        break;

    case 3:
        if (MR::getPlayerTriggerA()) {
            return true;
        }
        break;

    case 4:
        if (MR::getPlayerTriggerB()) {
            return true;
        }
        break;

    case 7:
        if (MR::getPlayerTriggerZ()) {
            return true;
        }
        break;

    case 8:
        if (MR::getPlayerTriggerC()) {
            return true;
        }
        break;

    case 5:
        if (MR::getPlayerLevelA()) {
            return true;
        }
        break;

    case 6:
        if (MR::getPlayerLevelB()) {
            return true;
        }
        break;

    case 9:
        if (MR::getPlayerLevelZ()) {
            return true;
        }
        break;

    case 10:
        if (MR::getPlayerLevelC()) {
            return true;
        }
        break;
    }

    return false;
}

void MarioActor::getStickValue(f32* pStickX, f32* pStickY) {
    *pStickX = 0.0f;
    *pStickY = 0.0f;

    if (mMario->isInputDisable()) {
        return;
    }

    if (mMario->getDrawStates()._7) {
        return;
    }

    if (pStickX != nullptr) {
        *pStickX = MR::getPlayerStickX();
    }

    if (pStickY != nullptr) {
        *pStickY = MR::getPlayerStickY();
    }
}

void MarioActor::updateActionTrigger() {
    _EED = false;
    _EEE = false;

    if (checkButtonType(3, false) || _F00 != 0) {
        if (_EEC) {
            return;
        }

        _EEC = true;
        _EED = true;
    } else {
        _EEC = false;
    }

    if (mMario->checkLvlA()) {
        _EEE = true;
    }
}

void MarioActor::updateControllerSwing() {
    if (checkButtonType(3, false) || checkButtonType(4, false)) {
        _F1C = 10;
        _EF8 = 0;
    }

    bool F20 = _F20;
    _F00 = false;

    bool isCorePadSwing = !mMario->isSwimming() ? MR::isCorePadSwing(0) : MR::isCorePadSwingTrigger(0);
    _F20 = isCorePadSwing;

    if (_F1C != 0) {
        _F1C--;
    } else if (isCorePadSwing && F20 == 0) {
        _F00 = true;
    }

    if (isCorePadSwing && _946 > 10 && _F1C < 8) {
        _F1C = 8;
    }
}

void MarioActor::updateControllerSwingLeft() {
    if (checkButtonType(7, false) || checkButtonType(8, false)) {
        _F1E = 10;
        _F04 = 0;
    }

    bool F21 = _F21;
    _F0C = false;

    bool isSubPadSwing = MR::isSubPadSwing(0);
    _F21 = isSubPadSwing;

    if (_F1E != 0) {
        _F1E--;
    } else if (isSubPadSwing && !F21) {
        _F0C = true;
    }

    if (isSubPadSwing && _946 > 10 && _F1E < 8) {
        _F1E = 8;
    }
}

void MarioActor::lockOnDPD() {
}

bool MarioActor::isRequestSpinJump2P() const {
    if (mMario->isInputDisable()) {
        return false;
    }

    if (_3C0) {
        return false;
    }

    if (!_EEB) {
        return false;
    }

    if (_946) {
        return false;
    }

    if (_EF6) {
        return false;
    }

    if (!mMario->isEnableRush()) {
        return false;
    }

    if (_935) {
        return false;
    }

    if (mPlayerMode == 5) {
        return false;
    }

    if (mMario->getMovementStates().jumping && mMario->_430 == 2 && mMario->_10._1F) {
        return false;
    }

    if (isJumping() && mMario->_3BC > 6 && MR::isStarPointerPointing2P(this, "弱", true, false) && MR::testCorePadTriggerA(1)) {
        return true;
    }

    return false;
}
