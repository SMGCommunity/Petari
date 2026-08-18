#include "Game/Camera/OnlyCamera.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Util/MathUtil.hpp"

void OnlyCamera_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
}

namespace {
    static const f32 sMinDistance = 1.0f;
    static const f32 sBaseLength = 300.0f;
    static const f32 sAccel = 1.0f;
    static const f32 sSpeedMax = 100.0f;
    // static const s32 sPenetrateCount =
}  // namespace

OnlyCamera::OnlyCamera(const char* pName)
    : NameObj(pName), mPoseParam(new CameraPoseParam()), mStartPose(true), mCalcIdeal(), mPos(0.0f, 0.0f, 0.0f), mSpeed(), _28(), _2C(true),
      _2D(true), _30(0.0f, 0.0f, 0.0f), mIsZeroFrameMoveOff(), mIsResetting() {
}

void OnlyCamera::calcPose(CameraMan* pCameraMan) {
    if (mIsResetting) {
        mCalcIdeal = false;
        _24 = 0;
        mIsResetting = false;
        mStartPose = true;
    }

    if (mStartPose) {
        calcStartPose(pCameraMan);
        mStartPose = false;
    } else {
        calcSafePose(pCameraMan);
    }
}

void OnlyCamera::calcStartPose(CameraMan* pCameraMan) {
    TVec3f pos = CameraLocalUtil::getPos(pCameraMan);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(pCameraMan);
    TVec3f up = CameraLocalUtil::getUpVec(pCameraMan);

    TVec3f offset = watchPos - pos;
    if (offset.length() < ::sBaseLength) {
        if (MR::isNearZero(offset)) {
            watchPos.set(pos + TVec3f(0.0f, 0.0f, -1.0f));
        } else {
            MR::normalize(&offset);
            watchPos.set(pos + offset * ::sBaseLength);
        }
    }

    TVec3f front = watchPos - pos;
    MR::normalize(&front);
    MR::normalizeOrZero(&up);

    if (MR::isNearZero(up) || MR::abs(front.dot(up)) > 0.98f) {
        TQuat4f rot;
        rot.setRotate(TVec3f(0.0f, 0.0f, -1.0f), front);
        up.set< f32 >(0.0f, 1.0f, 0.0f);
        rot.transform(up, up);
    }

    mPoseParam->mPos.set(pos);
    mPoseParam->mUpVec.set(up);
    mPoseParam->mWatchPos.set(watchPos);
    mPoseParam->mWatchUpVec.set(CameraLocalUtil::getWatchUpVec(pCameraMan));
    mPoseParam->mGlobalOffset.set(CameraLocalUtil::getGlobalOffset(pCameraMan));
    mPoseParam->mLocalOffset.set(CameraLocalUtil::getLocalOffset(pCameraMan));
    mPoseParam->mRoll = CameraLocalUtil::getRoll(pCameraMan);

    mPos.set(pos);
}

void OnlyCamera::calcSafePose(CameraMan* pCameraMan) {
    TVec3f pos = CameraLocalUtil::getPos(pCameraMan);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(pCameraMan);
    TVec3f up = CameraLocalUtil::getUpVec(pCameraMan);

    TVec3f offset = watchPos - pos;
    f32 dist = offset.length();
    if (dist < ::sBaseLength) {
        if (dist < ::sMinDistance) {
            watchPos.set(pos + mPoseParam->mWatchPos - mPoseParam->mPos);
        } else {
            MR::normalize(&offset);
            watchPos.set(pos + offset * ::sBaseLength);
        }
    }

    moveToIdealPosition(&pos);
    mIsZeroFrameMoveOff = false;

    TVec3f front = watchPos - pos;
    MR::normalize(&front);
    MR::normalizeOrZero(&up);

    if (MR::isNearZero(up) || MR::abs(front.dot(up)) > 0.98f) {
        TVec3f poseFront = mPoseParam->mWatchPos - mPoseParam->mPos;
        MR::normalize(&poseFront);
        if (MR::abs(front.dot(poseFront)) > 0.98f) {
            up.set(mPoseParam->mUpVec);
        } else {
            TQuat4f rot;
            rot.setRotate(poseFront, front);
            rot.transform(mPoseParam->mUpVec, up);
        }
    }
    CameraLocalUtil::recalcUpVec(&up, front);

    mPoseParam->mPos.set(pos);
    mPoseParam->mUpVec.set(up);
    mPoseParam->mWatchPos.set(watchPos);
    mPoseParam->mWatchUpVec.set(CameraLocalUtil::getWatchUpVec(pCameraMan));
    mPoseParam->mGlobalOffset.set(CameraLocalUtil::getGlobalOffset(pCameraMan));
    mPoseParam->mLocalOffset.set(CameraLocalUtil::getLocalOffset(pCameraMan));
    mPoseParam->mRoll = CameraLocalUtil::getRoll(pCameraMan);
}

void OnlyCamera::moveToIdealPosition(TVec3f* pPos) {
    TVec3f pos = *pPos;
    if (mCalcIdeal) {
        TVec3f diff = pos - mPos;
        pPos->set(mPoseParam->mPos + diff);

        TVec3f offset = pos - *pPos;
        f32 dist = offset.length();

        if (dist < mSpeed * 0.5f * (s32)mSpeed) {
            mSpeed -= ::sAccel;
            if (mSpeed < 0.0f) {
                mSpeed = 0.0f;
            }
        } else {
            mSpeed += ::sAccel;
            if (mSpeed > ::sSpeedMax) {
                mSpeed = ::sSpeedMax;
            }
        }

        if (dist < mSpeed || mSpeed == 0.0f) {
            mCalcIdeal = false;
            mSpeed = 0.0f;
            pPos->set(pos);
        } else {
            MR::normalize(&offset);
            offset.scale(mSpeed);
            pPos->add(offset);
        }
    } else {
        pPos->set(pos);
    }
    mPos.set(pos);
}
