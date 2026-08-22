#include "Game/Camera/CameraRailWatch.hpp"
#include "Game/Camera/CamTranslatorRailWatch.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraRailWatch_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

CameraRailWatch::CameraRailWatch(const char* pName)
    : Camera(pName), mRailRider(), mDirection(), mSetDirection(), mRailCoordOffset(), mDist(1200.0f), mAngleX() {
}

void CameraRailWatch::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));

    if (mRailRider == nullptr) {
        return;
    }

    if (mSetDirection == 0) {
        if (mRailRider->calcNearestPos(CameraLocalUtil::getPos(this)) <=
            mRailRider->calcNearestPos(CameraLocalUtil::getTarget(this)->getPosition())) {
            mDirection = 1;
        } else {
            mDirection = -1;
        }
    }
}

CameraTargetObj* CameraRailWatch::calc() {
    if (mRailRider == nullptr) {
        return nullptr;
    }

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    if (mSetDirection != 0) {
        mDirection = mSetDirection;
    }

    f32 watchCoord = mRailRider->calcNearestPos(watchPoint);
    mRailRider->setCoord(watchCoord);

    TVec3f up = -CameraLocalUtil::getTarget(this)->getGravityVector();
    TVec3f front = mRailRider->mCurDirection * mDirection;
    TVec3f side = up.cross(front);
    up.cross(front, side);

    TPos3f mtx;
    mtx.setXDir(side);
    mtx.setYDir(up);
    mtx.setZDir(front);
    mtx.setTrans(TVec3f(0.0f, 0.0f, 0.0f));

    mRailRider->setCoord(watchCoord + mDirection * mRailCoordOffset);

    TVec3f watchFront = mRailRider->mCurPos - watchPoint;
    if (MR::isNearZero(watchFront)) {
        return nullptr;
    }
    MR::normalize(&watchFront);

    TPos3f rotMtx;
    rotMtx.setRotate(front, watchFront);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mAngleX);

    mtx.concat(mtx, rotX);
    mtx.concat(rotMtx, mtx);

    mtx.getYDir(up);
    mtx.getZDir(front);

    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setPos(this, watchPoint - front * mDist);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

void CameraRailWatch::setParam(s32 zoneID, s32 railID, s32 direction, f32 offset, f32 dist, f32 angleX) {
    mRailRider = MR::getCameraDirector()->mRailHolder->getRider(zoneID, railID);
    mSetDirection = direction;
    mRailCoordOffset = offset;
    mDist = dist;
    mAngleX = angleX;
}

CamTranslatorBase* CameraRailWatch::createTranslator() {
    return new CamTranslatorRailWatch(this);
}
