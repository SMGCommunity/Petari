#include "Game/Camera/CameraCharmedTripodBoss.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraCharmedTripodBoss_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

CameraCharmedTripodBoss::CameraCharmedTripodBoss(const char* pName)
    : Camera(pName), mJointId(-1), mUp(0.0f, 1.0f, 0.0f), mWPoint(0.0f, 0.0f, 1000.0f), mAngleX(), mAngleY() {
}

void CameraCharmedTripodBoss::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraCharmedTripodBoss::calc() {
    CameraTargetObj* obj = CameraLocalUtil::getTarget(this);
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f up = mUp;
    TVec3f jointPos(0.0f, 0.0f, 0.0f);

    if (mJointId >= 0) {
        TPos3f jointMtx;
        MR::getTripodBossJointMatrix(&jointMtx, mJointId);
        jointMtx.getTrans(jointPos);
        jointMtx.mult33(up, up);
    }

    TPos3f rotX;
    rotX.makeRotate(TVec3f(-1.0f, 0.0f, 0.0f), mAngleX);
    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngleY);

    TPos3f rotMtx;
    rotMtx.concat(rotY, rotX);

    TVec3f front = CameraLocalUtil::getTarget(this)->getPosition() - jointPos;
    if (MR::isNearZero(front)) {
        return obj;
    }
    MR::normalize(&front);

    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return obj;
    }
    MR::normalize(&side);

    up.cross(front, side);
    MR::normalize(&front);

    TPos3f mtx;
    mtx.identity();
    mtx.setXYZDir(side, up, front);
    mtx.setTrans(CameraLocalUtil::getTarget(this)->getPosition());
    mtx.concat(mtx, rotMtx);
    mtx.getZDir(front);
    mtx.getYDir(up);

    TVec3f pos = mWPoint;
    mtx.mult(pos, pos);

    watchPos = pos - front * mWPoint.z;
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, mUp);

    return obj;
}

void CameraCharmedTripodBoss::setParam(s32 jointID, TVec3f up, const TVec3f& wPoint, const TVec2f& angle) {
    mAngleX = angle.x;
    mAngleY = angle.y;
    mUp.set(up);
    mWPoint.set(wPoint);
    mJointId = jointID;
}

CamTranslatorBase* CameraCharmedTripodBoss::createTranslator() {
    return new CamTranslatorCharmedTripodBoss(this);
}
