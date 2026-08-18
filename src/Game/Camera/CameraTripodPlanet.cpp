#include "Game/Camera/CameraTripodPlanet.hpp"
#include "Game/Camera/CamTranslatorTripodPlanet.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTripodPlanet_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraTripodPlanet::CameraTripodPlanet(const char* pName)
    : Camera(pName), mAxis(0.0f, 1.0f, 0.0f), mWPoint(0.0f, 0.0f, 0.0f), mAngleX(), mAngleY(), mDist(1000.0f) {
}

void CameraTripodPlanet::reset() {
}

CameraTargetObj* CameraTripodPlanet::calc() {
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TPos3f mtx = mZoneMatrix;
    TPos3f inv = mtx;
    inv.invert(inv);

    TVec3f localWatchPos = watchPos;
    inv.mult(localWatchPos, localWatchPos);

    TVec3f localPos = CameraLocalUtil::getTarget(this)->getPosition();
    inv.mult(localPos, localPos);

    TVec3f rotUp = localPos - mWPoint;
    if (MR::isNearZero(rotUp)) {
        return nullptr;
    }
    MR::normalize(&rotUp);

    TVec3f rotFront = mAxis;
    MR::normalize(&rotFront);
    rotFront.negate();

    TVec3f rotSide = rotUp.cross(rotFront);
    MR::normalize(&rotSide);

    rotFront.cross(rotSide, rotUp);

    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.setXDir(rotSide);
    baseMtx.setYDir(rotUp);
    baseMtx.setZDir(rotFront);
    baseMtx.setTrans(localWatchPos);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), -mAngleX);

    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngleY);

    TPos3f transMtx;
    transMtx.makeTrans(0.0f, 0.0f, mDist);

    TPos3f dstMtx;
    dstMtx.concat(rotX, transMtx);
    dstMtx.concat(rotY, dstMtx);
    dstMtx.concat(baseMtx, dstMtx);

    TVec3f pos;
    dstMtx.getTrans(pos);
    TVec3f up;
    dstMtx.getYDir(up);

    mtx.mult(pos, pos);
    mtx.mult33(up, up);

    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraTripodPlanet::createTranslator() {
    return new CamTranslatorTripodPlanet(this);
}
