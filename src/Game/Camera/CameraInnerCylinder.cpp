#include "Game/Camera/CameraInnerCylinder.hpp"
#include "Game/Camera/CamTranslatorInnerCylinder.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTargetObj_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraInnerCylinder::CameraInnerCylinder(const char* pName)
    : Camera(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 0.0f, 0.0f), mAngleA(), mAngleB(), mDist(), mLastWatchPoint(0.0f, 0.0f, 0.0f),
      mIsNotInitialized() {
}

void CameraInnerCylinder::reset() {
    mIsNotInitialized = false;
}

CameraTargetObj* CameraInnerCylinder::calc() {
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    watchPoint = watchPoint - CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getGroundPos();

    if (mIsNotInitialized) {
        mIsNotInitialized = false;
    } else {
        TVec3f prevDir = mLastWatchPoint - CameraLocalUtil::getTarget(this)->getPosition();
        f32 prevUp = CameraLocalUtil::getTarget(this)->getUpVec().dot(prevDir);

        TVec3f watchDir = watchPoint - CameraLocalUtil::getTarget(this)->getPosition();
        f32 newUp = CameraLocalUtil::getTarget(this)->getUpVec().dot(watchDir);

        f32 length = newUp * 0.05f + prevUp * 0.95f;

        watchPoint.set(CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * length);
    }

    mLastWatchPoint.set(watchPoint);

    TPos3f zoneMtx;
    zoneMtx.set(mZoneMatrix);
    TPos3f invMtx = zoneMtx;
    invMtx.invert(invMtx);
    invMtx.mult(watchPoint, watchPoint);

    TVec3f groundPos = CameraLocalUtil::getTarget(this)->getGroundPos();
    invMtx.mult(groundPos, groundPos);

    TVec3f front = groundPos - mWPoint;
    front -= mAxis * front.dot(mAxis);
    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    TVec3f up(0.0f, 1.0f, 0.0f);
    TVec3f side = up.cross(mAxis);
    if (MR::isNearZero(side)) {
        return nullptr;
    }
    MR::normalize(&side);

    TVec3f camUp = mAxis.cross(side);
    f32 angleX = camUp.dot(front);
    f32 angleY = side.dot(front);

    TVec3f watchDir = watchPoint - groundPos;
    TVec3f watchSide = side * side.dot(watchDir);

    TVec3f watchFront = watchDir - watchSide;
    watchSide *= mSideDist;
    watchPoint.set(watchSide + watchFront + groundPos);

    TPos3f mtx1;
    mtx1.setXDir(side);
    mtx1.setYDir(camUp);
    mtx1.setZDir(mAxis);
    mtx1.setTrans(watchPoint);

    TPos3f mtx2;
    mtx2.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mAngleA * angleX);

    TPos3f mtx3;
    mtx3.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), -mAngleB * angleY);

    mtx1.concat(mtx1, mtx3);
    mtx1.concat(mtx1, mtx2);

    TVec3f pos(0.0f, 0.0f, mDist);
    mtx1.mult(pos, pos);
    zoneMtx.mult(watchPoint, watchPoint);
    zoneMtx.mult33(up, up);
    zoneMtx.mult(pos, pos);

    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraInnerCylinder::createTranslator() {
    return new CamTranslatorInnerCylinder(this);
}
