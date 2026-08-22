#include "Game/Camera/CameraFooFighter.hpp"
#include "Game/Camera/CamTranslatorFooFighter.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void CameraFooFighter_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace {
    static const f32 sCollideRate = 0.03f;
    static const s32 sCollideCountMax = 60;
    static const f32 sLastMoveLen = 5.0f;
    static const f32 sRateWhenBraking = 0.05f;
}  // namespace

CameraFooFighter::CameraFooFighter(const char* pName)
    : Camera(pName), mDistMin(300.0f), mDistMax(1200.0f), mAngleXRateMin(0.03f), mAngleXRate(0.03f), mCollideCount(),
      mTargetLastMoveDir(0.0f, 0.0f, 0.0f) {
}

void CameraFooFighter::reset() {
    mCollideCount = 0;
    mAngleXRate = mAngleXRateMin;
    mTargetLastMoveDir.zero();

    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraFooFighter::calc() {
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f gravUp = -CameraLocalUtil::getTarget(this)->getGravityVector();
    TVec3f watchFront = watchPoint - CameraLocalUtil::getPos(this);
    f32 camDist = watchFront.length();
    f32 dist;
    if (MR::isNearZero(watchFront)) {
        watchFront.cross(CameraLocalUtil::getTarget(this)->getSideVec(), gravUp);
        dist = camDist;
    } else {
        if (camDist < mDistMin) {
            dist = mDistMin;
        } else if (camDist > mDistMax) {
            dist = mDistMax;
        } else {
            dist = camDist;
        }
    }
    MR::normalize(&watchFront);

    TVec3f up = CameraLocalUtil::getUpVec(this);
    TVec3f front = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    MR::normalize(&front);
    TQuat4f rotFront;
    rotFront.setRotate(front, watchFront);

    TVec3f newFront, newUp;
    newFront.set(watchFront);
    newUp.set(up);
    rotFront.transform(newUp);

    TVec3f newSide = newUp.cross(newFront);
    newSide -= gravUp * gravUp.dot(newSide);
    MR::normalize(&newSide);

    newUp.cross(newFront, newSide);
    newSide.cross(newUp, newFront);

    if (gravUp.dot(newUp) < 0.0f) {
        TQuat4f rotFrontUp;
        if (newFront.dot(gravUp) < 0.0f) {
            rotFrontUp.setRotate(newFront, -gravUp);
        } else {
            rotFrontUp.setRotate(newFront, gravUp);
        }

        rotFrontUp.transform(newFront);
        rotFrontUp.transform(newSide);
        rotFrontUp.transform(newUp);
    }

    if (MR::isPlayerNeedBrakingCamera()) {
        TPos3f mtx1;
        mtx1.identity();
        mtx1.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
        mtx1.setYDir(-CameraLocalUtil::getTarget(this)->getFrontVec());
        mtx1.setZDir(CameraLocalUtil::getTarget(this)->getUpVec());

        TQuat4f rot1;
        mtx1.getQuat(rot1);

        TPos3f mtx2;
        mtx2.identity();
        mtx2.setXDir(newSide);
        mtx2.setYDir(newUp);
        mtx2.setZDir(newFront);

        TQuat4f rot2;
        mtx2.getQuat(rot2);

        TQuat4f rotQ;
        rotQ.slerp(rot2, rot1, ::sRateWhenBraking);

        mtx2.makeQuat(rotQ);
        mtx2.getXDir(newSide);
        mtx2.getYDir(newUp);
        mtx2.getZDir(newFront);
    } else {
        TVec3f targetSide = CameraLocalUtil::getTarget(this)->getSideVec();
        targetSide -= gravUp * gravUp.dot(targetSide);
        MR::normalize(&targetSide);
        f32 angle = targetSide.angle(newSide);

        f32 rate = mAngleXRateMin * MR::abs(gravUp.dot(newFront));
        mAngleXRate *= 0.995f;
        if (mAngleXRate < rate) {
            mAngleXRate = rate;
        }

        TQuat4f rot;
        rot.setRotate(newSide, targetSide, rate);
        rot.transform(newFront);
        rot.transform(newUp);
        TVec3f pos = watchPoint - newFront * dist;

        f32 lastMoveLen = CameraLocalUtil::getTarget(this)->getLastMove().length();
        if (lastMoveLen > ::sLastMoveLen) {
            mTargetLastMoveDir.set(CameraLocalUtil::getTarget(this)->getLastMove());
            MR::normalize(&mTargetLastMoveDir);
        }

        if (lastMoveLen > ::sLastMoveLen && Collision::checkStrikeLineToMap(pos, newFront * dist, 0, nullptr, nullptr) != 0) {
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
            rot.setRotate(newFront, mTargetLastMoveDir, (mCollideCount * ::sCollideRate) / ::sCollideCountMax);
            rot.transform(newFront);
            rot.transform(newUp);
        }
    }

    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setPos(this, watchPoint - newFront * dist);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, newUp);
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraFooFighter::createTranslator() {
    return new CamTranslatorFooFighter(this);
}
