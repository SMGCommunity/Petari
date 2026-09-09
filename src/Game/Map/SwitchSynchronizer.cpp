#include "Game/Map/SwitchSynchronizer.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/ObjUtil.hpp"

SwitchSynchronizer::SwitchSynchronizer(const char* pName) : NameObj(pName), mSwitchCtrl(), mIsCheckSameSwitchState(true) {
}

void SwitchSynchronizer::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);

    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void SwitchSynchronizer::movement() {
    if (mIsCheckSameSwitchState) {
        if (mSwitchCtrl->isOnSwitchA() && mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->offSwitchA();
        } else if (!mSwitchCtrl->isOnSwitchA() && !mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->onSwitchA();
        }
    } else {
        if (mSwitchCtrl->isOnSwitchA() && !mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->offSwitchA();
        } else if (!mSwitchCtrl->isOnSwitchA() && mSwitchCtrl->isOnSwitchB()) {
            mSwitchCtrl->onSwitchA();
        }
    }
}
