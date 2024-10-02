#include "Game/AreaObj/SwitchArea.hpp"
#include "Game/Util.hpp"

SwitchArea::SwitchArea(int type, const char *pName) : AreaObj(type, pName) {

}

void SwitchArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void SwitchArea::movement() {
    if (!isUpdate()) {
        return;
    }

    bool bVar1 = false;

    if (!isValidSwitchB() || isOnSwitchB()) {
        bVar1 = true;
    }

    if (bVar1 && isInVolume(*MR::getPlayerPos())) {
        if (mObjArg1 == -1) {
            onSwitchA();
        }
        else {
            offSwitchA();
        }
    }
    else if (mObjArg0 != -1) {
        offSwitchA();
    }
}

const char *SwitchArea::getManagerName() const {
    return "SwitchArea";
}

bool SwitchArea::isUpdate() const {
    if (mObjArg2 != -1 && !MR::isOnGroundPlayer()) {
        return false;
    }

    if (mObjArg0 != -1) {
        return true;
    }

    if (mObjArg1 == -1) {
        return !isOnSwitchA();
    }

    return isOnSwitchA();
}
