#include "Game/Camera/CameraObjParallel.hpp"
#include "Game/Camera/CamTranslatorObjParallel.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

void CameraObjParallel_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraObjParallel::CameraObjParallel(const char* pName) : Camera(pName), mAngleX(), mAngleY(), mDist(3000.0f) {
}

void CameraObjParallel::reset() {
}

CameraTargetObj* CameraObjParallel::calc() {
    TPos3f mtx;
    mtx.identity();

    mtx.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
    mtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
    mtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mAngleX);

    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngleY);

    mtx.concat(rotY);
    mtx.concat(rotX);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    TVec3f offset;
    mtx.getZDir(offset);

    offset.set(watchPoint + offset * mDist);
    CameraLocalUtil::setPos(this, offset);

    TVec3f up;
    mtx.getYDir(up);
    CameraLocalUtil::setUpVec(this, up);

    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraObjParallel::createTranslator() {
    return new CamTranslatorObjParallel(this);
}
