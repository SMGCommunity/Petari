#include "Game/Camera/CameraMedianTower.hpp"
#include "Game/Camera/CamTranslatorMedianTower.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraMedianTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
    (void)0.5f;
}

namespace {
    static const f32 sPartialRate = 0.5f;
    static const f32 sAngularVel = 0.5f;
    static const s32 sInterpolate = 120;
}  // namespace

CameraMedianTower::CameraMedianTower(const char* pName)
    : Camera(pName), mString(), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 1.0f, 0.0f), mAngleA(MR::toRadian(30.0f)), mDist(1200.0f), mMaxFocusDist(),
      mPartialRate(::sPartialRate), mBaseFront(0.0f, 0.0f, 1.0f), mIsResetting(true), mInterpolateTime(::sInterpolate) {
}

void CameraMedianTower::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    mIsResetting = true;
}

CameraTargetObj* CameraMedianTower::calc() {
    TVec3f watchOffset;
    CameraLocalUtil::makeWatchOffset(&watchOffset, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f pos;
    pos.set(CameraLocalUtil::getTarget(this)->getPosition() * (1.0f - mPartialRate) + CameraLocalUtil::getVecReg(mString) * mPartialRate);

    TVec3f watchPos = mWPoint;
    mZoneMatrix.mult(watchPos, watchPos);

    TVec3f axis = mAxis;
    mZoneMatrix.mult33(axis, axis);

    pos.add(watchOffset);

    TPos3f towerMtx;
    if (!CameraLocalUtil::makeTowerCameraMtx(&towerMtx, mZoneMatrix, mWPoint, mAxis, pos)) {
        return nullptr;
    }

    TPos3f transMtx;
    transMtx.makeTrans(0.0f, 0.0f, mDist);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), -mAngleA);

    TPos3f mtx;
    mtx.concat(rotX, transMtx);
    mtx.concat(towerMtx, mtx);

    TVec3f newPos;
    mtx.getTrans(newPos);
    TVec3f up;
    mtx.getYDir(up);

    limitAngularVelocity(&newPos, &up, pos, watchPos, axis);
    calcDistance(&newPos, pos);

    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setPos(this, newPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
    updateReset(newPos, watchPos, axis);

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraMedianTower::createTranslator() {
    return new CamTranslatorMedianTower(this);
}

void CameraMedianTower::calcFovyPos(TVec3f* pFovPos, const TVec3f& rFront, const TVec3f& rVec, const TVec3f& rPos) {
    TVec3f projZ = rFront * rVec.dot(rFront);
    TVec3f vecOrtho = rVec - projZ;

    f32 halfFov = MR::getFovy() * 0.5f;
    TVec3f focusOffset = projZ - rFront * (vecOrtho.length() / MR::tanDegree(halfFov));

    pFovPos->set(rPos + focusOffset);
    pFovPos->add(rFront * (-200.0f / MR::sinDegree(halfFov)));
}

void CameraMedianTower::limitAngularVelocity(TVec3f* pPos, TVec3f* pUp, const TVec3f& rPos, const TVec3f& rWatchPos, const TVec3f& rAxis) {
    TVec3f outDir = *pPos - rWatchPos;
    outDir.orthogonalize(rAxis);
    if (MR::isNearZero(outDir)) {
        outDir.set(mBaseFront);
    } else {
        MR::normalize(&outDir);
    }

    if (mIsResetting) {
        mBaseFront = outDir;
        mIsResetting = false;
        return;
    }

    f32 angle = outDir.angle(mBaseFront);
    f32 angleMax = MR::toRadian(::sAngularVel);
    f32 rate = 0.0f;
    if (angle > angleMax) {
        rate = 1.0f - (angleMax / angle);
    }

    TQuat4f rot;
    rot.setRotate(outDir, mBaseFront, rate);

    TVec3f offset = *pPos - rPos;
    rot.transform(offset);
    *pPos = rPos + offset;

    rot.transform(*pUp);
    rot.transform(outDir);
    mBaseFront.set(outDir);
}

void CameraMedianTower::calcDistance(TVec3f* pPos, const TVec3f& rPos) {
    TVec3f dir = rPos - *pPos;
    MR::normalize(&dir);
    TVec3f regDiff = CameraLocalUtil::getVecReg(mString) - *pPos;
    TVec3f posDiff = CameraLocalUtil::getTarget(this)->getPosition() - *pPos;
    TVec3f fovPos;
    if (regDiff.squared() > posDiff.squared()) {
        calcFovyPos(&fovPos, dir, posDiff, *pPos);
    } else {
        calcFovyPos(&fovPos, dir, regDiff, *pPos);
    }

    TVec3f focusDiff;
    focusDiff.sub(fovPos, rPos);
    f32 fovDist = focusDiff.length();
    if (fovDist > mDist) {
        if (mDist <= mMaxFocusDist && fovDist > mMaxFocusDist) {
            focusDiff.setLength(mMaxFocusDist);
        }
        *pPos = rPos + focusDiff;
    }
}

void CameraMedianTower::updateReset(const TVec3f& rPos, const TVec3f& rWatchPos, const TVec3f& rAxis) {
    mInterpolateTime--;
    if (mInterpolateTime < 0) {
        mInterpolateTime = 0;
    }

    TVec3f outDir = rPos - rWatchPos;
    outDir.orthogonalize(rAxis);
    MR::normalizeOrZero(&outDir);

    TVec3f targetDir = CameraLocalUtil::getTarget(this)->getPosition() - rWatchPos;
    targetDir.orthogonalize(rAxis);
    MR::normalizeOrZero(&targetDir);

    if (outDir.dot(targetDir) < MR::cosDegree(140.0f)) {
        if (mInterpolateTime == 0) {
            mInterpolateTime = ::sInterpolate;
            MR::startCameraInterpolation(::sInterpolate);
            reset();
        }
    }
}
