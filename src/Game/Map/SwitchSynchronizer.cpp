#include "Game/Map/SwitchSynchronizer.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/ObjUtil.hpp"

SwitchSynchronizer::SwitchSynchronizer(const char* pName)
    : NameObj(pName) {
    mSwitchCtrl = nullptr;
    _10 = 1;
}

void SwitchSynchronizer::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void SwitchSynchronizer::movement() {
    if (_10) {
        if (mSwitchCtrl->isOnSwitchA() && mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->offSwitchA();
            return;
        }

        if (!mSwitchCtrl->isOnSwitchA() && !mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->onSwitchA();
            return;
        }
    } else {
        if (mSwitchCtrl->isOnSwitchA() && !mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->offSwitchA();
            return;
        }

        if (!mSwitchCtrl->isOnSwitchA() && mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->onSwitchA();
        }
    }
}

SwitchSynchronizer::~SwitchSynchronizer() {
}
