#include "Game/Map/GroupSwitchWatcher.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

GroupSwitchWatcher::GroupSwitchWatcher(const char* pName) : LiveActor(pName) {
    mSwitchCtrl = nullptr;
    _90 = -1;
    _94 = -1;
    _98 = -1;
}

void GroupSwitchWatcher::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &_90);
    MR::getJMapInfoArg1NoInit(rIter, &_94);
    MR::getJMapInfoArg2NoInit(rIter, &_98);
    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

/* https://decomp.me/scratch/a9a21 */
void GroupSwitchWatcher::control() {
    if (_98 == -1) {
        if (!mSwitchCtrl->isOnSwitchA()) {
            bool res = false;

            if (_94 != 0) {
                if (_94 == -1) {
                    res = mSwitchCtrl->isOnAllSwitchAfterB(_90);
                }
            } else {
                res = mSwitchCtrl->isOnAnyOneSwitchAfterB(_90);
            }

            if (res) {
                mSwitchCtrl->onSwitchA();
                kill();
            }
        }
    } else {
        bool res = false;
        if (_94 != 0) {
            if (_94 == -1) {
                res = mSwitchCtrl->isOnAllSwitchAfterB(_90);
            }
        } else {
            res = mSwitchCtrl->isOnAnyOneSwitchAfterB(_90);
        }

        if (res) {
            mSwitchCtrl->onSwitchA();
        } else {
            mSwitchCtrl->offSwitchA();
        }
    }
}

GroupSwitchWatcher::~GroupSwitchWatcher() {}
