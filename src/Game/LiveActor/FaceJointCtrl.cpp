#include "Game/LiveActor/FaceJointCtrl.hpp"
#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/Util/JointController.hpp"

#include "Game/Util.hpp"

namespace {
    const f32 sDefaultDegreeMax = 45.0f;
    const char* sTextOutFileName = "FaceJointCtrl";
}  // namespace

void FaceJointCtrl_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

FaceJointCtrl::FaceJointCtrl(LiveActor* pActor)
    : mHostActor(pActor), mDegreeMax(sDefaultDegreeMax), _8(0.0f, 0.0f, 1.0f), _14(0.0f, 0.0f, 1.0f), mJointController() {
    mJointCtrlRate = new JointCtrlRate();
}

void FaceJointCtrl::init() {
    const ResourceHolder* resourceHolder = MR::getResourceHolder(mHostActor);
    JMapInfo* csv = MR::tryCreateCsvParser(resourceHolder, "%s.bcsv", sTextOutFileName);
    if (csv) {
        const char* jointName = nullptr;
        csv->getValue(0, "JointName", &jointName);
        csv->getValue(0, "DegreeMax", &mDegreeMax);
        mJointController = MR::createJointDelegator< FaceJointCtrl >(this, mHostActor, &FaceJointCtrl::updateJointMtxCallBack, nullptr, jointName);
    }
}

void FaceJointCtrl::startCtrl(s32 time) {
    if (mJointController) {
        mJointCtrlRate->startCtrl(time);
        update();
    }
}

void FaceJointCtrl::endCtrl(s32 time) {
    if (mJointController) {
        mJointCtrlRate->endCtrl(time);
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

bool FaceJointCtrl::updateJointMtxCallBack(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    pMtx->getXDir(_14);
    MR::normalize(&_14);

    TVec3f position;
    pMtx->getTrans(position);
    TVec3f direction(*MR::getPlayerPos());
    direction.sub(position);

    if (MR::isNearZero(direction)) {
        return false;
    }

    MR::normalize(&direction);
    f32 rate = mJointCtrlRate->_0;
    MR::turnMtxToXDirDegree(pMtx, direction, mDegreeMax * rate);
    return true;
}
