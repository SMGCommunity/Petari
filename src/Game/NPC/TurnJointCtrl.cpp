#include "Game/NPC/TurnJointCtrl.hpp"
#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void TurnJointCtrl_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    void makeMtxRotVecDegree(MtxPtr pMtx, const TVec3f& rFrom, const TVec3f& rTo, f32 degree) {
        TVec3f axis;
        axis.cross(rFrom, rTo);
        f32 dot = rFrom.dot(rTo);

        if (MR::normalizeOrZero(&axis)) {
            PSMTXIdentity(pMtx);
        } else {
            f32 maxAngle = __fabsf(0.017453292f * degree);
            f32 angle = acos(dot);
            PSMTXRotAxisRad(pMtx, &axis, -MR::min(angle, maxAngle));
        }
    }
}  // namespace

TurnJointCtrl::TurnJointCtrl(LiveActor* pActor)
    : mActor(pActor), mCtrlRate(new JointCtrlRate()), mHorizontalDegreeMax(45.0f), mPositiveDegreeMax(22.5f), mNegativeDegreeMax(22.5f),
      mStarePos(0.0f, 0.0f, 0.0f), _5C(1000.0f), _60(0.04f), mCtrlFrames(60), mEnabled(true) {
    mFace.mEnabled = false;
    mFace.mBlendRate = 0.0f;
    mFace.mJointController = nullptr;
    mWaist.mEnabled = false;
    mWaist.mBlendRate = 0.0f;
    mWaist.mJointController = nullptr;

    MR::extractMtxZDir(pActor->getBaseMtx(), &mStarePos);
    mStarePos += pActor->mPosition;
}

void TurnJointCtrl::init(f32 horizontalDegreeMax, f32 positiveDegreeMax, f32 negativeDegreeMax) {
    mHorizontalDegreeMax = horizontalDegreeMax;
    mPositiveDegreeMax = positiveDegreeMax;
    mNegativeDegreeMax = negativeDegreeMax;
}

void TurnJointCtrl::addFace(const char* pJointName, f32 blendRate, AXIS verticalTurnAxis, AXIS horizontalTurnAxis, AXIS frontAxis) {
    mFace.mJointController =
        MR::createJointController< TurnJointCtrl >(this, mActor, pJointName, &TurnJointCtrl::updateJointMtxCallBackFace, nullptr);
    mFace.mBlendRate = blendRate;
    mFace.mFrontAxis = frontAxis;
    mFace.mVerticalTurnAxis = verticalTurnAxis;
    mFace.mHorizontalTurnAxis = horizontalTurnAxis;
    mFace.mEnabled = true;
}

void TurnJointCtrl::addWaist(const char* pJointName, f32 blendRate, AXIS verticalTurnAxis, AXIS horizontalTurnAxis, AXIS frontAxis) {
    mWaist.mJointController =
        MR::createJointController< TurnJointCtrl >(this, mActor, pJointName, &TurnJointCtrl::updateJointMtxCallBackWaist, nullptr);
    mWaist.mBlendRate = blendRate;
    mWaist.mFrontAxis = frontAxis;
    mWaist.mVerticalTurnAxis = verticalTurnAxis;
    mWaist.mHorizontalTurnAxis = horizontalTurnAxis;
    mWaist.mEnabled = true;
}

void TurnJointCtrl::startCtrl(s32 frames) {
    mEnabled = true;
    mCtrlRate->startCtrl(frames);
    mCtrlRate->update();
}

void TurnJointCtrl::endCtrl(s32 frames) {
    mEnabled = false;
    mCtrlRate->endCtrl(frames);
    mCtrlRate->update();
}

void TurnJointCtrl::validate() {
    startCtrl(0);
}

void TurnJointCtrl::invalidate() {
    endCtrl(0);
}

void TurnJointCtrl::setStarePos(const TVec3f& rPos) {
    if (!mEnabled && MR::isNearZero(mCtrlRate->_0)) {
        return;
    }

    if (MR::isNear(mActor, rPos, _5C)) {
        if (MR::isNearZero(mCtrlRate->_0)) {
            mCtrlRate->startCtrl(mCtrlFrames);
        }
    } else {
        if (MR::isNearZero(mCtrlRate->_0 - 1.0f)) {
            mCtrlRate->endCtrl(mCtrlFrames);
        }
    }

    MR::vecBlendSphere(mStarePos, rPos, &mStarePos, _60);
}

void TurnJointCtrl::update() {
    mCtrlRate->update();
}

void TurnJointCtrl::setCallBackFunction() {
    if (!mEnabled && MR::isNearZero(mCtrlRate->_0)) {
        return;
    }

    if (mFace.mEnabled) {
        mFace.mJointController->registerCallBack();
    }

    if (mWaist.mEnabled) {
        mWaist.mJointController->registerCallBack();
    }
}

bool TurnJointCtrl::updateJointMtxCallBackFace(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    return updateJointMtxCallBack(pMtx, mFace);
}

bool TurnJointCtrl::updateJointMtxCallBackWaist(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    return updateJointMtxCallBack(pMtx, mWaist);
}

void TurnJointCtrl::getMtxDir(TVec3f* pDir, const TPos3f* pMtx, AXIS axis) {
    switch (axis) {
    case X:
        pMtx->getXDir(*pDir);
        break;
    case Y:
        pMtx->getYDir(*pDir);
        break;
    case Z:
        pMtx->getZDir(*pDir);
        break;
    }

    MR::normalizeOrZero(pDir);
}

bool TurnJointCtrl::updateJointMtxCallBack(TPos3f* pMtx, const Ctrl& rCtrl) {
    if (!mEnabled && MR::isNearZero(mCtrlRate->_0)) {
        return false;
    }

    TVec3f position;
    pMtx->getTrans(position);
    TVec3f direction(mStarePos);
    direction.sub(position);

    if (MR::normalizeOrZero(&direction)) {
        return false;
    }

    TPos3f result = *pMtx;
    TPos3f rotation;
    result.setTrans(TVec3f(0.0f));

    TVec3f verticalAxis;
    TVec3f horizontalAxis;
    TVec3f front;
    TVec3f projected;
    getMtxDir(&verticalAxis, pMtx, rCtrl.mVerticalTurnAxis);
    MR::vecKillElement(direction, verticalAxis, &projected);

    if (!MR::normalizeOrZero(&projected)) {
        getMtxDir(&front, pMtx, rCtrl.mFrontAxis);
        TVec3f cross;
        cross.cross(direction, front);
        MR::normalizeOrZero(&cross);
        MR::vecBlendSphere(front, projected, &projected, rCtrl.mBlendRate);

        if (verticalAxis.dot(cross) > 0.0f) {
            const f32 rate = mCtrlRate->_0;
            makeMtxRotVecDegree(rotation, projected, front, mPositiveDegreeMax * rate);
        } else {
            const f32 rate = mCtrlRate->_0;
            makeMtxRotVecDegree(rotation, projected, front, mNegativeDegreeMax * rate);
        }

        PSMTXConcat(rotation, result, result);
    }

    getMtxDir(&horizontalAxis, pMtx, rCtrl.mHorizontalTurnAxis);
    MR::vecKillElement(direction, horizontalAxis, &projected);

    if (!MR::normalizeOrZero(&projected)) {
        getMtxDir(&front, pMtx, rCtrl.mFrontAxis);
        MR::vecBlendSphere(front, projected, &projected, rCtrl.mBlendRate);

        const f32 rate = mCtrlRate->_0;
        makeMtxRotVecDegree(rotation, projected, front, mHorizontalDegreeMax * rate);
        PSMTXConcat(rotation, result, result);
    }

    result.setTrans(position);
    pMtx->set(result);
    return true;
}
