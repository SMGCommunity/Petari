#include "Game/LiveActor/ActorJointCtrl.h"
#include "Game/LiveActor/FaceJointCtrl.h"

void ActorJointCtrl::startFaceCtrl(s32 val) {
    mFaceJointCtrl->startCtrl(val);
}

void ActorJointCtrl::endFaceCtrl(s32 val) {
    mFaceJointCtrl->endCtrl(val);
}

void ActorJointCtrl::update() {
    startUpdate();
    endUpdate();
}