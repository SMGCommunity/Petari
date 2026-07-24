#include "Game/Camera/CameraCharmedFix.hpp"
#include "Game/Camera/CamTranslatorCharmedFix.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraCharmedFix_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraCharmedFix::CameraCharmedFix(const char* pName)
    : Camera(pName), mBasePos(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mWPoint(0.0f, 0.0f, 1000.0f) {
}

void CameraCharmedFix::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraCharmedFix::calc() {
    CameraTargetObj* target = CameraLocalUtil::getTarget(this);

    const TPos3f& zoneMtx = mZoneMatrix;
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 1.0f / 150.0f);

    TVec3f pos = mBasePos;
    zoneMtx.mult(pos, pos);
    TVec3f front = CameraLocalUtil::getTarget(this)->getPosition() - pos;
    if (MR::isNearZero(front)) {
        return target;
    }

    MR::normalize(&front);
    TVec3f up = mUp;
    zoneMtx.mult33(up, up);
    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return target;
    }

    MR::normalize(&side);
    up.cross(front, side);

    TPos3f mtx;
    mtx.identity();
    mtx.setXYZDir(side, up, front);
    mtx.setTrans(CameraLocalUtil::getTarget(this)->getPosition());

    TVec3f watchPos = mWPoint;
    mtx.mult(watchPos, watchPos);
    watchPoint = watchPos - front * mWPoint.z;
    CameraLocalUtil::setPos(this, watchPos);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return target;
}

CamTranslatorBase* CameraCharmedFix::createTranslator() {
    return new CamTranslatorCharmedFix(this);
}
