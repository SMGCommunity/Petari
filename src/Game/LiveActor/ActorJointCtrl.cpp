#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/LiveActor/FaceJointCtrl.hpp"
#include "Game/LiveActor/IKJointCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"

ActorJointCtrl::ActorJointCtrl(LiveActor *pActor) {
    mActor = pActor;
    mJointCtrlKeeper = 0;
    mFaceJointCtrl = 0;
    mJointCtrlHolder = 0;
    _10 = 1;
    _11 = 0;

    mJointCtrlKeeper = new DynamicJointCtrlKeeper(mActor);
    mFaceJointCtrl = new FaceJointCtrl(mActor);
    mFaceJointCtrl->init();
    mJointCtrlHolder = ActorJoint::createIKJointCtrlHolder(mActor);

    if (mJointCtrlHolder) {
        _11 = 1;
    }
}

void ActorJointCtrl::startDynamicCtrl(const char *pName, s32 val) {
    mJointCtrlKeeper->startCtrl(pName, val);
}
void ActorJointCtrl::endDynamicCtrl(const char *pName, s32 val) {
    mJointCtrlKeeper->endCtrl(pName, val);
}

void ActorJointCtrl::resetDynamicCtrl() {
    mJointCtrlKeeper->reset();
}

void ActorJointCtrl::startFaceCtrl(s32 val) {
    mFaceJointCtrl->startCtrl(val);
}

void ActorJointCtrl::endFaceCtrl(s32 val) {
    mFaceJointCtrl->endCtrl(val);
}

void ActorJointCtrl::setIKEndPosition(const char *pName, const TVec3f &a2, f32 a3) {
    mJointCtrlHolder->setEndPosition(pName, a2, a3);
}

void ActorJointCtrl::setIKEndDirection(const char *pName, const TVec3f &a2, f32 a3) {
    mJointCtrlHolder->setEndDirection(pName, a2, a3);
}

void ActorJointCtrl::endIKCtrlAll() {
    mJointCtrlHolder->endCtrlAll();
}

void ActorJointCtrl::update() {
    startUpdate();
    endUpdate();
}

void ActorJointCtrl::startUpdate() {
    if (mJointCtrlHolder) {
        mJointCtrlHolder->startUpdate();
    }

    if (_11) {
        mActor->calcAnim();
    }

    _10 = 0;
}

void ActorJointCtrl::endUpdate() {
    if (mJointCtrlKeeper) {
        mJointCtrlKeeper->update();
    }

    if (mFaceJointCtrl) {
        mFaceJointCtrl->update();
    }

    if (mJointCtrlHolder) {
        mJointCtrlHolder->endUpdate();
    }

    _10 = 1;
}

void ActorJointCtrl::setCallBackFunction() {
    if (_10) {
        if (mJointCtrlKeeper) {
            mJointCtrlKeeper->setCallBackFunction();
        }

        if (mFaceJointCtrl) {
            mFaceJointCtrl->setCallBackFunction();
        }

        if (mJointCtrlHolder) {
            mJointCtrlHolder->setCallBackFunction();
        }
    }
}