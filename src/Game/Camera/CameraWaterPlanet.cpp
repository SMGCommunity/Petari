#include "Game/Camera/CameraWaterPlanet.hpp"
#include "Game/Camera/CamTranslatorWaterPlanet.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraWaterPlanet_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
}

CameraWaterPlanet::CameraWaterPlanet(const char* pName) : Camera(pName), mDistMin(500.0f), mDistMax(2000.0f), mAngleX(MR::pi() / 4.0f) {
}

void CameraWaterPlanet::reset() {
    TVec3f watchPos = CameraLocalUtil::getWatchPos(mCameraMan);
    TVec3f pos = CameraLocalUtil::getPos(mCameraMan);
    TVec3f up = CameraLocalUtil::getUpVec(mCameraMan);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f front = watchPos - pos;
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    TVec3f watchFront = watchPoint - pos;
    if (MR::isNearZero(watchFront)) {
        return;
    }
    MR::normalize(&watchFront);

    TQuat4f frontRot;
    frontRot.setRotate(front, watchFront);
    TQuat4f rotRoll;
    rotRoll.setRotate(front, CameraLocalUtil::getRoll(mCameraMan));
    rotRoll.transform(up);
    frontRot.transform(up);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

CameraTargetObj* CameraWaterPlanet::calc() {
    TVec3f currWatchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f up = CameraLocalUtil::getUpVec(this);

    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f front = currWatchPos - pos;
    if (MR::isNearZero(front)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&front);

    TVec3f targetFront = CameraLocalUtil::getTarget(this)->getSideVec().cross(-CameraLocalUtil::getTarget(this)->getGravityVector());
    if (MR::isNearZero(targetFront)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&targetFront);

    TPos3f mtx;
    mtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mtx.setYDir(-CameraLocalUtil::getTarget(this)->getGravityVector());
    mtx.setZDir(targetFront);
    mtx.setTrans(watchPos);

    TPos3f inv = mtx;
    inv.invert(inv);
    inv.mult(pos, pos);

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), pos, &length, &angleX, &angleY);

    if (angleX < mAngleX) {
        angleX += (mAngleX - angleX) * 0.05f;
        MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &pos, length, angleX, angleY);
    }

    mtx.mult(pos, pos);

    TVec3f watchFront = watchPos - pos;
    if (MR::isNearZero(watchFront)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&watchFront);

    TQuat4f frontRot;
    frontRot.setRotate(front, watchFront);
    frontRot.transform(up);

    TVec3f offset = pos - watchPos;
    f32 dist = offset.length();

    if (dist < mDistMin) {
        offset.setLength(mDistMin);
    } else if (dist > mDistMax) {
        offset.setLength(mDistMax);
    }

    pos.set(watchPos + offset);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraWaterPlanet::createTranslator() {
    return new CamTranslatorWaterPlanet(this);
}
