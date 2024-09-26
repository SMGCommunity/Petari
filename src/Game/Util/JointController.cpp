#include "Game/Util/JointController.hpp"
#include "JSystem/J3DGraphAnimator/J3DJoint.hpp"

JointController::JointController() {
    mModel = 0;
    mJoint = 0;
}

bool JointController::calcJointMatrix(TPos3f *, const JointControllerInfo &) {
    return 0;
}

bool JointController::calcJointMatrixAfterChild(TPos3f *, const JointControllerInfo &) {
    return 0;
}

void JointController::registerCallBack() {
    mJoint->mJointCallback = JointController::staticCallBack;
    mJoint->mJointData = this;
}