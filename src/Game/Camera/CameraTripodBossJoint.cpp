#include "Game/Camera/CameraTripodBossJoint.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"

void CameraTripodBossJoint_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sDistance = 300.0f;
    static const f32 sAtternuation = 0.7f;
    static const f32 sRoundDiv = 8.0f;
    static const f32 sRoundAddition = 0.08f;
};  // namespace

CameraTripodBossJoint::~CameraTripodBossJoint() {
}

CameraTripodBossJoint::CameraTripodBossJoint(const char* pName)
    : Camera(pName), mAngleX(), mAngleY(), mDist(3000.0f), mIsRounding(), mRoundTarget(), mRoundAngle() {
}

void CameraTripodBossJoint::reset() {
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPos);
    calcIdealPose();
    mIsRounding = false;
    mRoundTarget = 0.0f;
    mRoundAngle = 0.0f;
}

CameraTargetObj* CameraTripodBossJoint::calc() {
    calcIdealPose();
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraTripodBossJoint::createTranslator() {
    return new CamTranslatorTripodBossJoint(this);
}

void CameraTripodBossJoint::calcIdealPose() {
    if (mDist < ::sDistance) {
        mDist = ::sDistance;
    }

    if (mIsRounding) {
        if (mRoundTarget > mRoundAngle) {
            mRoundAngle += ::sRoundAddition;
            if (mRoundAngle >= mRoundTarget) {
                mRoundAngle = mRoundTarget;
                mIsRounding = false;
            }
        } else {
            mRoundAngle -= ::sRoundAddition;
            if (mRoundAngle <= mRoundTarget) {
                mRoundAngle = mRoundTarget;
                mIsRounding = false;
            }
        }
    } else {
        f32 roundInterval = MR::toRadian(360.0f / ::sRoundDiv);

        if (CameraLocalUtil::tryCameraReset()) {
            mRoundAngle *= ::sAtternuation;
            mRoundTarget = mRoundAngle;
        } else if (CameraLocalUtil::testCameraPadTriggerRoundLeft()) {
            s32 divNum = (mRoundAngle < 0.0f ? -1 : 1) * (s32)(((roundInterval / 2) + MR::abs(mRoundAngle)) / roundInterval) - 1;

            if (divNum < -::sRoundDiv / 2) {
                divNum += ::sRoundDiv;
                mRoundAngle += MR::pi() * 2.0f;
            }

            mRoundTarget = divNum * roundInterval;
            mIsRounding = true;
        } else if (CameraLocalUtil::testCameraPadTriggerRoundRight()) {
            s32 divNum = (mRoundAngle < 0.0f ? -1 : 1) * (s32)(((roundInterval / 2) + MR::abs(mRoundAngle)) / roundInterval) + 1;

            if (divNum > ::sRoundDiv / 2) {
                divNum -= ::sRoundDiv;
                mRoundAngle -= MR::pi() * 2.0f;
            }

            mRoundTarget = divNum * roundInterval;
            mIsRounding = true;
        }
    }

    TVec3f pos;
    MR::polarToCrossDegree(TVec3f(0.0f, 0.0f, 0.0f), &pos, mDist, mAngleX, (mAngleY - 90.0f) + mRoundAngle);

    TVec3f up;
    if (MR::isCreatedTripodBoss()) {
        TPos3f mtx;
        MR::getTripodBossJointMatrix(&mtx, mJointID);
        mtx.mult33(pos, pos);
        mtx.getYDir(up);
    } else {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    if (MR::isCreatedTripodBoss()) {
        TPos3f mtx;
        MR::getTripodBossJointMatrix(&mtx, mJointID);
        TVec3f offset;
        mtx.mult33(mAxis, offset);
        watchPoint.add(offset);
    }

    pos.add(watchPoint);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setWatchUpVec(this, up);
}
