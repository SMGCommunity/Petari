#include "Game/LiveActor/FaceJointCtrl.hpp"
#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/Util/JointController.hpp"

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