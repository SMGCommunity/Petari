#include "Game/LiveActor/FaceJointCtrl.h"
#include "Game/LiveActor/DynamicJointCtrl.h"
#include "Game/Util/JointController.h"

FaceJointCtrl::FaceJointCtrl(LiveActor *pActor) {
    mHostActor = pActor;
    _4 = 45.0f;
    _8 = 0.0f;
    _C = 0.0f;
    _10 = 1.0f;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 1.0f;
    mJointController = 0;
    mJointCtrlRate = new JointCtrlRate();
}

void FaceJointCtrl::startCtrl(s32 val) {
    if (mJointController) {
        mJointCtrlRate->startCtrl(val);
        update();
    }
}

void FaceJointCtrl::endCtrl(s32 val) {
    if (mJointController) {
        mJointCtrlRate->endCtrl(val);
        update();
    }
}

void FaceJointCtrl::update() {
    if (!mJointController) {
        return;
    }

    mJointCtrlRate->update();
}

void FaceJointCtrl::setCallBackFunction() {
    if (!mJointController) {
        return;
    }

    mJointController->registerCallBack();
}