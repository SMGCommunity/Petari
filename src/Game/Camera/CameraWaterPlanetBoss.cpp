#include "Game/Camera/CameraWaterPlanetBoss.hpp"
#include "Game/Camera/CamTranslatorWaterPlanetBoss.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraWaterPlanetBoss_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::pi();
    (void)(MR::pi() / 2.0f);
}

namespace {
    static const f32 sCollideRate = 0.03f;
    static const s32 sCollideCountMax = 60;
    static const f32 sLastMoveLen = 5.0f;
    static const f32 sAngleRate = 0.02f;
    static const s32 sRoundingFrame = 60;
};  // namespace

CameraWaterPlanetBoss::CameraWaterPlanetBoss(const char* pName)
    : Camera(pName), mDistMin(300.0f), mDistMax(1200.0f), mSideBlendRateMin(0.01f), mSideBlendRate(0.01f), mCollideCount(),
      mLastMoveDir(0.0f, 0.0f, 0.0f), mIsBlendUpEnabled(), mWPoint(0.0f, 0.0f, 0.0f), mDotFrontMin(), mAngleXMin(), mDotFrontMax(), mAngleXMax(),
      mIsRounding(), mRoundingFrame(), mSide(1.0f, 0.0f, 0.0f) {
}

void CameraWaterPlanetBoss::reset() {
    mIsRounding = false;
    mSideBlendRate = mSideBlendRateMin;
    mCollideCount = 0;
    mLastMoveDir.zero();

    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraWaterPlanetBoss::calc() {
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f gravUp = -CameraLocalUtil::getTarget(this)->getGravityVector();
    TVec3f watchFront = watchPos - CameraLocalUtil::getPos(this);
    f32 camDist = watchFront.length();

    f32 dist;
    if (MR::isNearZero(watchFront)) {
        watchFront.cross(CameraLocalUtil::getTarget(this)->getSideVec(), gravUp);
        dist = camDist;
    } else {
        dist = camDist;
        if (dist < mDistMin) {
            dist = mDistMin;
        } else if (dist > mDistMax) {
            dist = mDistMax;
        }
    }
    MR::normalize(&watchFront);

    TVec3f camUp = CameraLocalUtil::getUpVec(this);
    TVec3f camFront = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    MR::normalize(&camFront);
    TQuat4f rotFront;
    rotFront.setRotate(camFront, watchFront);

    TVec3f front, up, side;
    front.set(watchFront);
    up.set(camUp);
    rotFront.transform(up);
    side.cross(up, front);
    side -= gravUp * gravUp.dot(side);
    MR::normalize(&side);

    up.cross(front, side);

    if (gravUp.dot(up) < 0.0f) {
        TQuat4f upsideDownRot;
        if (front.dot(gravUp) < 0.0f) {
            upsideDownRot.setRotate(front, -gravUp);
        } else {
            upsideDownRot.setRotate(front, gravUp);
        }
        upsideDownRot.transform(front);
        upsideDownRot.transform(side);
        upsideDownRot.transform(up);
    }

    TVec3f gravSide = CameraLocalUtil::getTarget(this)->getSideVec();
    gravSide -= gravUp * gravUp.dot(gravSide);
    MR::normalize(&gravSide);

    if (mIsRounding) {
        gravSide = mSide;
        mRoundingFrame++;

        f32 rate = static_cast< f32 >(mRoundingFrame) / static_cast< f32 >(::sRoundingFrame);
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        rate *= rate;

        f32 angle = gravSide.angle(side) * 180.0f / MR::pi();
        f32 angle2 = angle - angle * rate;
        if (MR::isNearZero(angle)) {
            mSideBlendRate = 1.0f;
        } else {
            mSideBlendRate = 1.0f - angle2 / angle;
        }

        if (mRoundingFrame >= ::sRoundingFrame) {
            mIsRounding = false;
        }
    } else {
        if (CameraLocalUtil::tryCameraResetTrigger()) {
            mIsRounding = true;
            mRoundingFrame = 0;
            mSide.set(gravSide);
        }

        f32 rate = mSideBlendRateMin * MR::abs(gravUp.dot(front));
        mSideBlendRate *= 0.005f;
        if (mSideBlendRate < rate) {
            mSideBlendRate = rate;
        }
    }

    TQuat4f rot;
    rot.setRotate(side, gravSide, mSideBlendRate);
    rot.transform(front);
    rot.transform(up);
    rot.transform(side);

    TVec3f pos = watchPos - front * dist;
    f32 lastMoveLen = CameraLocalUtil::getTarget(this)->getLastMove().length();
    if (lastMoveLen > ::sLastMoveLen) {
        mLastMoveDir.set(CameraLocalUtil::getTarget(this)->getLastMove());
        MR::normalize(&mLastMoveDir);
    }

    if (lastMoveLen > ::sLastMoveLen && Collision::checkStrikeLineToMap(pos, front * dist, 0, nullptr, nullptr) != 0) {
        mCollideCount++;
        if (mCollideCount > ::sCollideCountMax) {
            mCollideCount = ::sCollideCountMax;
        }
    } else {
        mCollideCount--;
        if (mCollideCount < 0) {
            mCollideCount = 0;
        }
    }

    if (mCollideCount > 0) {
        rot.setRotate(front, mLastMoveDir, mCollideCount * ::sCollideRate / ::sCollideCountMax);
        rot.transform(front);
        rot.transform(up);
        rot.transform(side);
    }

    if (mIsBlendUpEnabled && mCollideCount == 0) {
        TVec3f wPoint = mWPoint;
        mZoneMatrix.mult(wPoint, wPoint);

        TVec3f dir = CameraLocalUtil::getTarget(this)->getPosition() - wPoint;
        f32 gravDot = gravUp.dot(dir);
        f32 rate = (gravDot - mDotFrontMin) / (mDotFrontMax - mDotFrontMin);
        if (rate > 1.0f) {
            rate = 1.0f;
        } else if (rate < 0.0f) {
            rate = 0.0f;
        }

        TVec3f rotMin = gravUp;
        rot.setRotate(side, mAngleXMin + MR::pi() / 2.0f);
        rot.transform(rotMin);

        TVec3f rotMax = gravUp;
        rot.setRotate(side, mAngleXMax + MR::pi() / 2.0f);
        rot.transform(rotMax);

        TVec3f upBlend = rotMin;
        rot.setRotate(rotMin, rotMax, rate);
        rot.transform(upBlend);

        rot.setRotate(front, upBlend, ::sAngleRate);
        rot.transform(front);
        rot.transform(side);
        rot.transform(up);
    }

    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setPos(this, watchPos - front * dist);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraWaterPlanetBoss::createTranslator() {
    return new CamTranslatorWaterPlanetBoss(this);
}
