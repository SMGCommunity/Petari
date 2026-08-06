#include "Game/Camera/CameraInwardTower.hpp"
#include "Game/Camera/CamTranslatorInwardTower.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraInwardTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    // static const s32 sFollowFrame =
    static const f32 sRoundThreshold = 0.3f;
    static const f32 sAttenuation = 20.0f;
    static const f32 sResetRotSpeed = 0.2f;
};  // namespace

CameraInwardTower::~CameraInwardTower() {
}

CameraInwardTower::CameraInwardTower(const char* pName)
    : Camera(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 1.0f, 0.0f), mAngleX(), mAngleMax(), mDist(2000.0f), mPos(0.0f, 10.0f, 0.0f), mAngleY(),
      mIsResetting() {
    createVPanObj();
}

void CameraInwardTower::reset() {
    mVPan->resetJump();
    mVPan->calcWatchPos(nullptr);
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getWatchUpVec(mCameraMan));
    mVPan->reset();
    mPos.set(CameraLocalUtil::getPos(this));
    mIsResetting = false;
    mAngleY = 0.0f;
}

CameraTargetObj* CameraInwardTower::calc() {
    mVPan->updateJump();
    mVPan->calcWatchPos(nullptr);
    calcIdealPose();
    mPos.set(CameraLocalUtil::getWatchPos(this));
    mVPan->arrange();
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraInwardTower::createTranslator() {
    return new CamTranslatorInwardTower(this);
}

void CameraInwardTower::calcIdealPose() {
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f axis = mAxis;
    mZoneMatrix.mult33(axis, axis);

    TVec3f wPoint = mWPoint;
    mZoneMatrix.mult(wPoint, wPoint);

    TVec3f outDir = watchPos - wPoint;
    outDir.set(outDir - axis * axis.dot(outDir));
    if (MR::isNearZero(outDir)) {
        return;
    }
    MR::normalize(&outDir);

    TVec3f up = -CameraLocalUtil::getTarget(this)->getGravityVector();
    TVec3f camFront = watchPos - mPos;
    MR::normalizeOrZero(&camFront);

    TVec3f camUp = outDir.cross(camFront);
    f32 rate = camUp.length();
    if (camUp.dot(up) < 0.0f) {
        rate = -rate;
    }

    f32 speed, angle;
    angle = mAngleY;
    speed = mAngleMax * MR::abs(rate) / ::sAttenuation;
    if (CameraLocalUtil::tryCameraResetTrigger()) {
        mIsResetting = true;
    }

    if (mIsResetting) {
        if (mAngleY >= 0.0f) {
            mAngleY -= ::sResetRotSpeed;
            if (mAngleY < 0.0f) {
                mAngleY = 0.0f;
                mIsResetting = false;
            }
        } else {
            mAngleY += ::sResetRotSpeed;
            if (mAngleY > 0.0f) {
                mAngleY = 0.0f;
                mIsResetting = false;
            }
        }
    } else {
        if (rate > ::sRoundThreshold) {
            angle += speed;
            if (angle > mAngleMax) {
                angle = mAngleMax;
            }
            mAngleY = mAngleY * 0.9f + angle * 0.1f;
        } else if (rate < -::sRoundThreshold) {
            angle -= speed;
            if (angle < -mAngleMax) {
                angle = -mAngleMax;
            }
            mAngleY = mAngleY * 0.9f + angle * 0.1f;
        }
    }

    TPos3f rotY;
    rotY.makeRotate(up, mAngleY);

    TVec3f front = outDir;
    rotY.mult(front, front);
    TVec3f side = up;
    side.cross(side, front);
    if (MR::isNearZero(side)) {
        return;
    }
    MR::normalize(&side);

    TPos3f rotX;
    rotX.makeRotate(side, mAngleX);
    rotX.mult(front, front);

    TVec3f pos = watchPos - front * mDist;
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}
