#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

namespace {
    static const f32 sLocalOffsetRateScale = 0.05f;
    // static const f32 sBaseSpeed =
};  // namespace

void CameraHeightArrange_FORCE_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)MR::pi();
    (void)(MR::pi() / 2.0f);
}

CameraHeightArrange::~CameraHeightArrange() {
}

CameraHeightArrange::CameraHeightArrange(Camera* pCamera)
    : NameObj("CameraHeightArrange"), mCamera(pCamera), mIsJumping(), mIsStartJump(), mIsEndJump(), mNextParam(new CameraPoseParam()),
      mCurrParam(new CameraPoseParam()), mCameraWatchPosBaseHeight(), mCameraTargetPosHeight(), mCameraWatchPosHeight(), mCameraPosHeight(),
      mCameraWatchPosOffset(), mCameraPosOffset(), mJumpState(), mStateTime(), mIsCatchup(), mChaseTime(), mRiseTime(), mDropTime(), mRiseEaseTime(),
      mUpdateGlobalAxis(), mGlobalAxis(0.0f, 1.0f, 0.0f), mOffsetScale(0.1f), mOffsetScaleMax(15.0f), mFocalScaleUpper(0.3f), mFocalScaleLower(0.1f),
      mGndInt(160), mPosOffsetMinRiseLag(500.0f), mPosOffsetMinDropLag(800.0f), mRiseDelay(120), mDropDelay(120), mMaxRiseEaseTime(120),
      mVPanUse(true), mVPanAxis(0.0f, 1.0f, 0.0f) {
}

void CameraHeightArrange::updateJump() {
    if (CameraLocalUtil::getTarget(mCamera) == nullptr) {
        return;
    }

    if (!mIsJumping && CameraLocalUtil::getTarget(mCamera)->isJumping()) {
        mIsStartJump = true;
    } else {
        mIsStartJump = false;
    }

    if (mIsJumping && !CameraLocalUtil::getTarget(mCamera)->isJumping()) {
        mIsEndJump = true;
    } else {
        mIsEndJump = false;
    }

    mIsJumping = CameraLocalUtil::getTarget(mCamera)->isJumping();
}

void CameraHeightArrange::calcWatchPos(CameraTargetObj* pTarget) {
    if (pTarget == nullptr) {
        pTarget = CameraLocalUtil::getTarget(mCamera);
    }

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, mCamera, pTarget, mOffsetScale / mOffsetScaleMax);

    TVec3f globalAxis = getGlobalAxis();
    if (!(mVPanUse && mIsJumping)) {
        mCameraWatchPosBaseHeight = globalAxis.dot(watchPoint);
    }

    watchPoint -= globalAxis * globalAxis.dot(watchPoint);
    watchPoint += globalAxis * mCameraWatchPosBaseHeight;
    CameraLocalUtil::setWatchPos(mCamera, watchPoint);
}

void CameraHeightArrange::arrange() {
    if (!mVPanUse) {
        return;
    }

    checkState();
    arrangeHeight();
    arrangeVPan();

    CameraPoseParam* param = mCamera->mPoseParam;
    param->mWatchPos.set(mCurrParam->mWatchPos);
    param->mPos.set(mCurrParam->mPos);
    param->mUpVec.set(mCurrParam->mUpVec);
    param->mWatchUpVec.set(mCurrParam->mWatchUpVec);
}

void CameraHeightArrange::resetJump() {
    mIsJumping = false;
    mIsStartJump = false;
    mIsEndJump = false;
}

void CameraHeightArrange::reset() {
    TVec3f globalAxis = getGlobalAxis();
    mCameraWatchPosBaseHeight = globalAxis.dot(CameraLocalUtil::getWatchPos(mCamera));
    mCameraTargetPosHeight = globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getPosition());
    mCameraPosHeight = globalAxis.dot(CameraLocalUtil::getPos(mCamera));

    mCameraWatchPosHeight = mCameraWatchPosBaseHeight;
    mCameraWatchPosOffset = 0.0f;
    mCameraPosOffset = 0.0f;
    mJumpState = JumpState_NoMove;
    mStateTime = 0;
    mIsCatchup = false;
    mChaseTime = 0;
    mRiseTime = 0;
    mDropTime = 0;

    mNextParam->copyFrom(*mCamera->mPoseParam);
    mCurrParam->copyFrom(*mCamera->mPoseParam);

    if (CameraLocalUtil::getTarget(mCamera)->isJumping()) {
        if (globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getLastMove()) >= 0.0f) {
            mJumpState = JumpState_Rise;
        } else {
            mJumpState = JumpState_Fall;
        }
    }
}

void CameraHeightArrange::resetParameter() {
    mOffsetScale = 0.1f;
    mOffsetScaleMax = 15.0f;
    mFocalScaleUpper = 0.3f;
    mFocalScaleLower = 0.1f;
    mGndInt = 160;
    mPosOffsetMinRiseLag = 500.0f;
    mPosOffsetMinDropLag = 800.0f;
    mRiseDelay = 120;
    mDropDelay = 120;
    mMaxRiseEaseTime = 120;
    mVPanUse = true;
    mVPanAxis.set(0.0f, 1.0f, 0.0f);
    mGlobalAxis.set(0.0f, 1.0f, 0.0f);
    mUpdateGlobalAxis = false;
}

void CameraHeightArrange::checkState() {
    TVec3f globalAxis = getGlobalAxis();

    switch (mJumpState) {
    case JumpState_NoMove: {
        if (mVPanUse && mIsStartJump) {
            mJumpState = JumpState_Rise;
            mStateTime = 0;
        }
        break;
    }
    case JumpState_Rise: {
        if (mIsJumping) {
            if (globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getLastMove()) < 0.0f) {
                mJumpState = JumpState_Fall;
                mStateTime = 0;
            }
        } else {
            mJumpState = JumpState_NoMove;
            mStateTime = 0;
        }
        break;
    }
    case JumpState_Fall: {
        if (mIsJumping) {
            if (globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getLastMove()) > 0.0f) {
                mJumpState = JumpState_Rise;
                mStateTime = 0;
            }
        } else {
            mJumpState = JumpState_NoMove;
            mStateTime = 0;
        }
        break;
    }
    }
}

void CameraHeightArrange::arrangeHeight() {
    f32 blendRate = 1.0f;

    mNextParam->mWatchPos.set(CameraLocalUtil::getWatchPos(mCamera));
    mNextParam->mPos.set(CameraLocalUtil::getPos(mCamera));

    TVec3f watchUp = mNextParam->mWatchUpVec;
    watchUp += (CameraLocalUtil::getWatchUpVec(mCamera) - watchUp) * blendRate;
    MR::normalizeOrZero(&watchUp);
    mNextParam->mWatchUpVec.set(watchUp);

    TVec3f up = mNextParam->mUpVec;
    up += (CameraLocalUtil::getUpVec(mCamera) - up) * blendRate;
    mNextParam->mUpVec.set(up);
}

void CameraHeightArrange::arrangeVPan() {
    TVec3f globalAxis = getGlobalAxis();

    switch (mJumpState) {
    case JumpState_NoMove: {
        if (mStateTime == 0) {
            mChaseTime = 0;
        }
        mIsCatchup = false;
        chase();
        mCameraTargetPosHeight = globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getPosition());

        break;
    }
    case JumpState_Rise: {
        updateHeightAndOffset();
        break;
    }
    case JumpState_Fall: {
        updateHeightAndOffset();
        break;
    }
    }

    calcPose();
    updateUpper();
    updateLower();
    mStateTime++;
    mChaseTime++;
}

void CameraHeightArrange::calcPose() {
    TVec3f globalAxis = getGlobalAxis();
    mCurrParam->copyFrom(*mNextParam);

    TVec3f watchPos = mCurrParam->mWatchPos;
    watchPos -= globalAxis * globalAxis.dot(watchPos);
    watchPos += globalAxis * (mCameraWatchPosHeight + mCameraWatchPosOffset);
    mCurrParam->mWatchPos.set(watchPos);

    TVec3f pos = mCurrParam->mPos;
    pos -= globalAxis * globalAxis.dot(pos);
    pos += globalAxis * (mCameraPosHeight + mCameraPosOffset);
    mCurrParam->mPos.set(pos);
}

void CameraHeightArrange::updateUpper() {
    f32 t = (f32)mRiseEaseTime / mMaxRiseEaseTime;

    // ease in-out
    if (t > 1.0f) {
        t = 1.0f;
    }
    t = (MR::sin(t * MR::pi() + MR::pi() / 2) + 1.0f) * 0.5f;
    if (t > 1.0f) {
        t = 1.0f;
    }
    if (t < 0.0f) {
        t = 0.0f;
    }

    f32 offset = calcOffset(mFocalScaleUpper * t);

    if (offset > 0.0f) {
        bool isFastRise = CameraLocalUtil::getTarget(mCamera) != nullptr && CameraLocalUtil::getTarget(mCamera)->isFastRise();

        f32 riseRate;
        if (!isFastRise && mRiseDelay > 0) {
            riseRate = (f32)mRiseTime / mRiseDelay;
        } else {
            riseRate = 1.0f;
        }

        f32 watchRiseOffset = mCameraWatchPosOffset + offset * riseRate;
        f32 posOffsetLag = watchRiseOffset - mCameraPosOffset;
        mCameraWatchPosOffset = watchRiseOffset;

        if (posOffsetLag > mPosOffsetMinRiseLag) {
            mCameraPosOffset += (posOffsetLag - mPosOffsetMinRiseLag);
        }

        calcPose();
        mIsCatchup = true;

        mRiseTime++;
        if (mRiseTime > mRiseDelay) {
            mRiseTime = mRiseDelay;
        }
    } else {
        mRiseTime--;
        if (mRiseTime < 0) {
            mRiseTime = 0;
            mRiseEaseTime = 0;
        }
    }

    if (mRiseTime > 0) {
        mRiseEaseTime++;
    }
    if (mRiseEaseTime > mMaxRiseEaseTime) {
        mRiseEaseTime = mMaxRiseEaseTime;
    }
}

void CameraHeightArrange::updateLower() {
    TVec3f globalAxis = getGlobalAxis();

    f32 offset = calcOffset(-mFocalScaleLower);
    if (offset < 0.0f) {
        if (mIsCatchup || mCameraTargetPosHeight > globalAxis.dot(CameraLocalUtil::getTarget(mCamera)->getPosition())) {
            bool isFastDrop = CameraLocalUtil::getTarget(mCamera) != nullptr && CameraLocalUtil::getTarget(mCamera)->isFastDrop();

            f32 dropRate;
            if (!isFastDrop && mDropDelay > 0) {
                dropRate = (f32)mDropTime / mDropDelay;
            } else {
                dropRate = 1.0f;
            }

            f32 watchDropOffset = mCameraWatchPosOffset + offset * dropRate;
            f32 posOffsetLag = mCameraPosOffset - watchDropOffset;
            mCameraWatchPosOffset = watchDropOffset;

            if (posOffsetLag > mPosOffsetMinDropLag) {
                mCameraPosOffset -= (posOffsetLag - mPosOffsetMinDropLag);
            }

            calcPose();

            mDropTime++;
            if (mDropTime > mDropDelay) {
                mDropTime = mDropDelay;
            }
        }
    } else {
        mDropTime--;
        if (mDropTime < 0) {
            mDropTime = 0;
        }
    }
}

f32 CameraHeightArrange::calcOffset(f32 focalScale) {
    TVec3f globalAxis = getGlobalAxis();

    f32 fovAngle = MR::atan2(MR::tan(((CameraLocalUtil::getFovy(mCamera) * MR::pi()) * 0.5f) / 180.0f) * focalScale, 1.0f);

    TVec3f front = mCurrParam->mWatchPos - mCurrParam->mPos;
    if (MR::isNearZero(front)) {
        return 0.0f;
    }
    MR::normalize(&front);

    TVec3f up = globalAxis;
    MR::normalize(&up);

    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return 0.0f;
    }
    MR::normalize(&side);

    up.cross(front, side);
    f32 cosBetween = up.dot(globalAxis);
    if (MR::abs(cosBetween) < 0.1f) {
        return 0.0f;
    }

    TVec3f toTarget = CameraLocalUtil::getTarget(mCamera)->getPosition() - mCurrParam->mPos;
    return (up.dot(toTarget) - front.dot(toTarget) * MR::tan(fovAngle)) / cosBetween;
}

void CameraHeightArrange::chase() {
    TVec3f globalAxis = getGlobalAxis();

    f32 t = (f32)mChaseTime / mGndInt;

    if (t > 1.0f) {
        t = 1.0f;
    }

    if (t > 0.5f) {
        mRiseEaseTime = 0;
    }

    updateHeightAndOffset();
}

void CameraHeightArrange::updateHeightAndOffset() {
    TVec3f globalAxis = getGlobalAxis();
    f32 rate = mVPanUse ? ::sLocalOffsetRateScale : 1.0f;

    mCameraWatchPosOffset += -mCameraWatchPosOffset * rate;
    mCameraPosOffset += -mCameraPosOffset * rate;

    mCameraWatchPosHeight += rate * (globalAxis.dot(mNextParam->mWatchPos) - mCameraWatchPosHeight);
    mCameraPosHeight += rate * (globalAxis.dot(mNextParam->mPos) - mCameraPosHeight);
}

const TVec3f& CameraHeightArrange::getGlobalAxis() {
    if (mUpdateGlobalAxis) {
        mUpdateGlobalAxis = false;

        mGlobalAxis.set(mVPanAxis);
        mCamera->mZoneMatrix.mult33(mGlobalAxis);
    }

    return mGlobalAxis;
}
