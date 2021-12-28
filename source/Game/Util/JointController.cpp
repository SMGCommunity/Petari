#include "Game/Util/JointController.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"

JointController::JointController() {
    mModel = 0;
    mJoint = 0;
}

s32 JointController::calcJointMatrix(TPos3f *, const JointControllerInfo &) {
    return 0;
}

s32 JointController::calcJointMatrixAfterChild(TPos3f *, const JointControllerInfo &) {
    return 0;
}

void JointController::registerCallBack() {
    mJoint->mJointCallback = JointController::staticCallBack;
    mJoint->mJointData = this;
}