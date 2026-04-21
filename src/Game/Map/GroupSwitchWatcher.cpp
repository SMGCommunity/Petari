#include "Game/Map/GroupSwitchWatcher.hpp"

GroupSwitchWatcher::GroupSwitchWatcher(const char* pName)
    : LiveActor(pName), mSwitchCtrl(nullptr), mNumSwitches(-1), mLogicType(-1), mActionType(-1) {
}

void GroupSwitchWatcher::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mNumSwitches);
    MR::getJMapInfoArg1NoInit(rIter, &mLogicType);
    MR::getJMapInfoArg2NoInit(rIter, &mActionType);
    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void GroupSwitchWatcher::control() {
    if (mActionType == -1) {
        if (!mSwitchCtrl->isOnSwitchA()) {
            bool activate = false;
            switch (mLogicType) {
            case -1:
                activate = mSwitchCtrl->isOnAllSwitchAfterB(mNumSwitches);
                break;
            case 0:
                activate = mSwitchCtrl->isOnAnyOneSwitchAfterB(mNumSwitches);
                break;
            }

            if (activate) {
                mSwitchCtrl->onSwitchA();
                kill();
            }
        }
    } else {
        bool activate = false;
        switch (mLogicType) {
        case -1:
            activate = mSwitchCtrl->isOnAllSwitchAfterB(mNumSwitches);
            break;
        case 0:
            activate = mSwitchCtrl->isOnAnyOneSwitchAfterB(mNumSwitches);
            break;
        }

        if (activate) {
            mSwitchCtrl->onSwitchA();
        } else {
            mSwitchCtrl->offSwitchA();
        }
    }
}
