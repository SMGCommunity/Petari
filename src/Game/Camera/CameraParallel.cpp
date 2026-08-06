#include "Game/Camera/CameraParallel.hpp"
#include "Game/Camera/CamTranslatorParallel.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraParallel_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sDistance = 3000.0f;
    static const f32 sRoundDiv = 8.0f;
    static const f32 sRoundAddition = 0.08f;
};  // namespace

CameraParallel::CameraParallel(const char* pName)
    : Camera(pName), mAngleX(), mAngleY(), mDist(::sDistance), mIsEnableValid(), mIsRounding(), mRoundTarget(), mRoundAngle(), mRoundAddition() {
    createVPanObj();
}

void CameraParallel::reset() {
    mVPan->resetJump();
    mVPan->calcWatchPos(nullptr);
    calcIdealPose();
    mVPan->reset();
    mIsRounding = false;
    mRoundTarget = 0.0f;
    mRoundAngle = 0.0f;
    mRoundAddition = ::sRoundAddition;
}

CameraTargetObj* CameraParallel::calc() {
    mVPan->updateJump();
    mVPan->calcWatchPos(nullptr);
    calcIdealPose();
    mVPan->arrange();

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraParallel::createTranslator() {
    return new CamTranslatorParallel(this);
}

bool CameraParallel::isEnableToRoundLeft() const {
    return mIsEnableValid;
}

bool CameraParallel::isEnableToRoundRight() const {
    return mIsEnableValid;
}

bool CameraParallel::isEnableToReset() const {
    return mIsEnableValid;
}

void CameraParallel::calcIdealPose() {
    if (mDist < 300.0f) {
        mDist = 300.0f;
    }

    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);

    TVec3f pos;
    MR::polarToCrossDegree(TVec3f(0.0f, 0.0f, 0.0f), &pos, mDist, mAngleX, mAngleY - 90.0f);

    TVec3f up;
    up.set< f32 >(0.0f, 1.0f, 0.0f);
    calcRound();

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), pos, &length, &angleX, &angleY);
    angleY += mRoundAngle;
    MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &pos, length, angleX, angleY);

    mZoneMatrix.mult33(pos);
    mZoneMatrix.mult33(up);

    pos.add(watchPos);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setWatchUpVec(this, up);
}

void CameraParallel::calcRound() {
    if (!mIsEnableValid) {
        return;
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
        return;
    }

    f32 roundInterval = MR::toRadian(360.0f / ::sRoundDiv);

    if (CameraLocalUtil::testCameraPadTriggerReset()) {
        mRoundTarget = 0.0f;
        mIsRounding = true;
        mRoundAddition = ::sRoundAddition * 2.0f;
        return;
    }

    if (CameraLocalUtil::testCameraPadTriggerRoundLeft()) {
        s32 divNum = (mRoundAngle < 0.0f ? -1 : 1) * (s32)(((roundInterval / 2) + MR::abs(mRoundAngle)) / roundInterval) - 1;

        if (divNum < -::sRoundDiv / 2) {
            divNum += ::sRoundDiv;
            mRoundAngle += MR::pi() * 2.0f;
        }

        mRoundTarget = divNum * roundInterval;
        mIsRounding = true;
        mRoundAddition = ::sRoundAddition;
        return;
    }

    if (CameraLocalUtil::testCameraPadTriggerRoundRight()) {
        s32 divNum = (mRoundAngle < 0.0f ? -1 : 1) * (s32)(((roundInterval / 2) + MR::abs(mRoundAngle)) / roundInterval) + 1;

        if (divNum > ::sRoundDiv / 2) {
            divNum -= ::sRoundDiv;
            mRoundAngle -= MR::pi() * 2.0f;
        }

        mRoundTarget = divNum * roundInterval;
        mIsRounding = true;
        mRoundAddition = ::sRoundAddition;
        return;
    }
}
