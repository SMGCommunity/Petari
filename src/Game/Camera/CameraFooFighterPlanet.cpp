#include "Game/Camera/CameraFooFighterPlanet.hpp"
#include "Game/Camera/CamTranslatorFooFighterPlanet.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

void CameraFooFighterPlanet_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
    (void)0.5f;
}

namespace {
    static f32 sRateWhenBraking = 0.05f;
};  // namespace

CameraFooFighterPlanet::~CameraFooFighterPlanet() {
}

CameraFooFighterPlanet::CameraFooFighterPlanet(const char* pName) : Camera(pName), mDistMin(500.0f), mDistMax(2000.0f), mPitchMax(MR::pi() / 4.0f) {
}

void CameraFooFighterPlanet::reset() {
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

    TVec3f newFront = watchPoint - pos;
    if (MR::isNearZero(newFront)) {
        return;
    }
    MR::normalize(&newFront);

    TQuat4f upRot;
    upRot.setRotate(front, newFront);
    TQuat4f rollRot;
    rollRot.setRotate(front, CameraLocalUtil::getRoll(mCameraMan));

    rollRot.transform(up);
    upRot.transform(up);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

CameraTargetObj* CameraFooFighterPlanet::calc() {
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f up = CameraLocalUtil::getUpVec(this);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f front = watchPos - pos;
    if (MR::isNearZero(front)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&front);

    TVec3f front2 = CameraLocalUtil::getTarget(this)->getSideVec().cross(-CameraLocalUtil::getTarget(this)->getGravityVector());
    if (MR::isNearZero(front2)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&front2);

    TPos3f mtx1;
    mtx1.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mtx1.setYDir(-CameraLocalUtil::getTarget(this)->getGravityVector());
    mtx1.setZDir(front2);
    mtx1.setTrans(watchPoint);

    TPos3f inv = mtx1;
    inv.invert(inv);
    inv.mult(pos, pos);

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), pos, &length, &angleX, &angleY);
    if (angleX < mPitchMax) {
        angleX += (mPitchMax - angleX) * 0.05f;
    }
    MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &pos, length, angleX, angleY);
    mtx1.mult(pos, pos);

    TVec3f newFront = watchPoint - pos;
    if (MR::isNearZero(newFront)) {
        return CameraLocalUtil::getTarget(this);
    }
    MR::normalize(&newFront);

    TQuat4f upRot;
    upRot.setRotate(front, newFront);
    upRot.transform(up);

    TVec3f camOffs = pos - watchPoint;
    f32 dist = camOffs.length();
    if (dist < mDistMin) {
        camOffs.setLength(mDistMin);
    } else if (dist > mDistMax) {
        camOffs.setLength(mDistMax);
    }
    pos.set(watchPoint + camOffs);

    if (MR::isPlayerNeedBrakingCamera()) {
        goRoundBehind(pos, watchPoint, up);
    }

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraFooFighterPlanet::createTranslator() {
    return new CamTranslatorFooFighterPlanet(this);
}

void CameraFooFighterPlanet::goRoundBehind(TVec3f& rPos, TVec3f& rWatchPos, TVec3f& rUp) {
    TVec3f front = rWatchPos - rPos;
    f32 dist = front.length();
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    rUp.orthogonalize(front);
    if (MR::isNearZero(rUp)) {
        return;
    }
    MR::normalize(&rUp);

    TVec3f side = rUp.cross(front);

    TPos3f rotMtx;
    rotMtx.identity();
    rotMtx.setXDir(side);
    rotMtx.setYDir(rUp);
    rotMtx.setZDir(front);

    TQuat4f rot1;
    rotMtx.getQuat(rot1);

    TPos3f targetMtx;
    targetMtx.identity();
    targetMtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    targetMtx.setYDir(-CameraLocalUtil::getTarget(this)->getFrontVec());
    targetMtx.setZDir(CameraLocalUtil::getTarget(this)->getUpVec());

    TQuat4f targetRot;
    targetMtx.getQuat(targetRot);

    TQuat4f rot;
    rot.slerp(rot1, targetRot, ::sRateWhenBraking);
    rotMtx.makeQuat(rot);
    rotMtx.getZDir(front);
    rotMtx.getYDir(rUp);
    rPos = rWatchPos - front * dist;
}
