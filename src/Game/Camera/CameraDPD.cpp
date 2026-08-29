#include "Game/Camera/CameraDPD.hpp"
#include "Game/Camera/CamTranslatorDPD.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "revolution/wpad.h"

void CameraDPD_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    f32 f6 = MR::pi() / 2.0f;
    (void)2.0f;
}

namespace {
    static const f32 sMaxDistance = 0.15f;
    static const f32 sFovyBlendRate = 0.05f;
};  // namespace

CameraDPD::CameraDPD(const char* pName)
    : Camera(pName), mCorePadDistToDisplay(), mRealPointPos(0.0f, 0.0f), mBlendPointPos(0.0f, 0.0f), mResetPointingPos(true), mCameraType(), mDist(),
      mFovy(40.0f), mAngleXRange(PI / 2.0f), mAngleYRange(PI / 2.0f), mAngleX(), mSpeedBlendRate(0.05f), mBlendFriction(0.99f), mIsVertical(),
      mAngleYMin() {
    mMtx.identity();
}

void CameraDPD::reset() {
    if (mCameraType == CameraType_FixedPos) {
        CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
        TVec3f pos = (CameraLocalUtil::getTarget(this)->getPosition() + TVec3f(0.0f, 100.0f, 0.0f));
        TVec3f front = pos - CameraLocalUtil::getPos(this);
        MR::normalize(&front);
        TVec3f up = CameraLocalUtil::getTarget(this)->getUpVec();
        TVec3f side = up.cross(front);
        MR::normalize(&side);
        up.cross(front, side);
        mMtx.identity();
        mMtx.setXDir(side);
        mMtx.setYDir(up);
        mMtx.setZDir(front);
    } else if (mCameraType == CameraType_FollowWorldPos) {
        TVec3f worldPos;
        worldPos.set(*MR::getStarPointerWorldPosUsingDepth(WPAD_CHAN0));
        TVec3f basePos = CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * mDist;
        TVec3f front = worldPos - basePos;
        if (MR::isNearZero(front)) {
            front.set(CameraLocalUtil::getTarget(this)->getFrontVec());
        } else {
            MR::normalize(&front);
        }

        TVec3f up = CameraLocalUtil::getTarget(this)->getUpVec();
        TVec3f side = up.cross(front);
        if (MR::isNearZero(side)) {
            CameraMan* camMan = mCameraMan;
            TVec3f diff = CameraLocalUtil::getWatchPos(camMan) - CameraLocalUtil::getPos(camMan);
            TQuat4f rot;
            rot.setRotate(diff, front);
            side.cross(CameraLocalUtil::getUpVec(mCameraMan), diff);
            rot.transform(side);
        } else {
            MR::normalize(&side);
        }
        up.cross(front, side);
        mMtx.identity();
        mMtx.setXDir(side);
        mMtx.setYDir(up);
        mMtx.setZDir(front);
    }

    MR::getCorePadPointingPos(&mRealPointPos, WPAD_CHAN0);
    mResetPointingPos = true;
    mBlendPointPos.set(0.0f, 0.0f);
    mRealPointPos.set(0.0f, 0.0f);
}

CameraTargetObj* CameraDPD::calc() {
    if (mCameraType == CameraType_UpdateWithTarget) {
        mMtx.identity();
        mMtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
        mMtx.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
        mMtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
        CameraLocalUtil::setPos(this, CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * mDist);
    } else if (mCameraType == CameraType_FollowWorldPos) {
        CameraLocalUtil::setPos(this, CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * mDist);
    }

    TVec2f pointPos;
    MR::getCorePadPointingPos(&pointPos, WPAD_CHAN0);

    if (mResetPointingPos) {
        pointPos.set(0.0f, 0.0f);
        if (MR::abs(mRealPointPos.x) < 0.5f && MR::abs(mRealPointPos.y) < 0.5f) {
            mResetPointingPos = false;
        }
    }

    if (MR::isCorePadPointInScreen(WPAD_CHAN0)) {
        mRealPointPos = mRealPointPos * 0.9f + pointPos * 0.1f;
        if (mCameraType != CameraType_FollowWorldPos) {
            if (MR::testSubPadTriggerZ(WPAD_CHAN0)) {
                mCorePadDistToDisplay = MR::getCorePadDistanceToDisplay(WPAD_CHAN0);
            }
            if (MR::testSubPadButtonZ(WPAD_CHAN0)) {
                f32 updateDist = MR::clamp(mCorePadDistToDisplay - MR::getCorePadDistanceToDisplay(WPAD_CHAN0), -::sMaxDistance, ::sMaxDistance);
                mFovy =
                    (40.0f - ((updateDist * 30.0f) / ::sMaxDistance)) * (1.0f - ::sFovyBlendRate) + CameraLocalUtil::getFovy(this) * ::sFovyBlendRate;
            }
        }
    }

    pointPos.x = MR::clamp(mRealPointPos.x, -1.0f, 1.0f);
    pointPos.y = MR::clamp(mRealPointPos.y, -1.0f, 1.0f);

    f32 moveSpeed = MR::clamp01((pointPos - mBlendPointPos).length() / MR::sqrt(2.0f));

    f32 t = 1.0f - moveSpeed * mSpeedBlendRate;
    t *= mBlendFriction;
    mBlendPointPos = mBlendPointPos * t + pointPos * (1.0f - t);

    if (mCameraType != CameraType_FollowWorldPos) {
        if (!MR::testSubPadButtonZ(WPAD_CHAN0)) {
            mFovy = CameraLocalUtil::getFovy(this) * ::sFovyBlendRate + 38.0f;
        }
        CameraLocalUtil::setFovy(this, mFovy);
    }

    TPos3f panYMtx;
    panYMtx.identity();
    if (mIsVertical) {
        f32 f1 = mBlendPointPos.y * MR::max(mAngleYRange, mAngleYMin);
        f32 f2;
        if (mAngleYRange < mAngleYMin) {
            f2 = f1 > mAngleYRange ? mAngleYRange : f1;
        } else {
            f2 = f1 < -mAngleYMin ? -mAngleYMin : f1;
        }
    }
    panYMtx.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mBlendPointPos.y * mAngleYRange);

    TPos3f panXMtx;
    panXMtx.identity();
    panXMtx.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), -mBlendPointPos.x * mAngleXRange);

    TPos3f mtx3;
    mtx3.identity();
    mtx3.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), -mAngleX);

    TPos3f rotMtx;
    rotMtx.concat(panXMtx, panYMtx);
    rotMtx.concat(mtx3, rotMtx);
    rotMtx.concat(mMtx, rotMtx);
    TVec3f front;
    rotMtx.getZDir(front);
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getPos(this) + front * 1000.0f);
    TVec3f up;
    rotMtx.getYDir(up);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);

    return nullptr;
}

CamTranslatorBase* CameraDPD::createTranslator() {
    return new CamTranslatorDPD(this);
}
