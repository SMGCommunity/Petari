#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DJoint.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphBase/J3DSys.hpp"

JointController::JointController() : mModel(), mJoint() {
}

bool JointController::calcJointMatrix(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    return false;
}

bool JointController::calcJointMatrixAfterChild(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    return false;
}

void JointController::registerCallBack() {
    mJoint->setCallBack(staticCallBack);
    mJoint->mCallBackUserData = this;
}

void JointController::calcJointMatrixAndSetSystem(J3DJoint* pJoint) {
    MtxPtr pJointMtx = mModel->getAnmMtx(pJoint->getJntNo());
    TPos3f mtx;
    PSMTXCopy(pJointMtx, mtx);
    JointControllerInfo info = {this, pJoint};

    if (calcJointMatrix(&mtx, info)) {
        PSMTXCopy(mtx, pJointMtx);
        PSMTXCopy(mtx, J3DSys::mCurrentMtx);
    }
}

void JointController::calcJointMatrixAfterChildAndSetSystem(J3DJoint* pJoint) {
    MtxPtr pJointMtx = mModel->getAnmMtx(pJoint->getJntNo());
    TPos3f mtx;
    PSMTXCopy(pJointMtx, mtx);
    JointControllerInfo info = {this, pJoint};

    if (calcJointMatrixAfterChild(&mtx, info)) {
        PSMTXCopy(mtx, pJointMtx);
    }
}

int JointController::staticCallBack(J3DJoint* pJoint, int timing) {
    if (!pJoint) {
        return 0;
    }

    JointController* pController = static_cast< JointController* >(pJoint->mCallBackUserData);
    if (!pController) {
        return 0;
    }

    if (timing == 0) {
        pController->calcJointMatrixAndSetSystem(pJoint);
    }

    if (timing == 1) {
        pController->calcJointMatrixAfterChildAndSetSystem(pJoint);
        pJoint->setCallBack(nullptr);
        pJoint->mCallBackUserData = nullptr;
    }

    return 0;
}

namespace MR {
    void setJointControllerParam(JointController* pController, const LiveActor* pActor, const char* pJointName) {
        J3DJoint* pJoint = getJoint(pActor, pJointName);
        pController->mModel = getJ3DModel(pActor);
        pController->mJoint = pJoint;
    }

    void setJointControllerParam(JointController* pController, const LiveActor* pActor, u16 jointIndex) {
        J3DJoint* pJoint = getJoint(pActor, jointIndex);
        pController->mModel = getJ3DModel(pActor);
        pController->mJoint = pJoint;
    }
}  // namespace MR
