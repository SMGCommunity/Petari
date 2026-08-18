#include "Game/Camera/CameraRailFollow.hpp"
#include "Game/Camera/CamTranslatorRailFollow.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraRailFollow_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

CameraRailFollow::CameraRailFollow(const char* pName)
    : Camera(pName), mRailRider(), mRailCoord(), mRailSpeed(), mTargetMargin(), mRailSpeedMax(30.0f), mRailAccel(0.35f) {
}

void CameraRailFollow::reset() {
    if (mRailRider == nullptr) {
        return;
    }

    mRailCoord = mRailRider->calcNearestPos(CameraLocalUtil::getPos(mCameraMan));
    mRailSpeed = 0.0f;
}

CameraTargetObj* CameraRailFollow::calc() {
    if (mRailRider == nullptr) {
        return nullptr;
    }

    f32 targetCoord = mRailRider->calcNearestPos(CameraLocalUtil::getTarget(this)->getPosition());
    if (mRailRider->isLoop()) {
        if (mRailCoord - targetCoord > mRailRider->getTotalLength() / 2) {
            targetCoord += mRailRider->getTotalLength();
        } else if (targetCoord - mRailCoord > mRailRider->getTotalLength() / 2) {
            targetCoord -= mRailRider->getTotalLength();
        }
    }

    if (mMarginScale == 0) {
        if (mRailCoord < targetCoord) {
            targetCoord -= mTargetMargin;
        } else {
            targetCoord += mTargetMargin;
        }
    } else {
        targetCoord += mTargetMargin * mMarginScale;
    }

    if (MR::abs(targetCoord - mRailCoord) > getStopLength()) {
        if (mRailCoord < targetCoord) {
            mRailSpeed += mRailAccel;
            if (mRailSpeed > mRailSpeedMax) {
                mRailSpeed = mRailSpeedMax;
            }
        } else {
            mRailSpeed -= mRailAccel;
            if (mRailSpeed < -mRailSpeedMax) {
                mRailSpeed = -mRailSpeedMax;
            }
        }
    } else {
        if (mRailCoord < targetCoord) {
            mRailSpeed -= mRailAccel;
            if (mRailSpeed < 0.0f) {
                mRailSpeed = 0.0f;
            }
        } else {
            mRailSpeed += mRailAccel;
            if (mRailSpeed > 0.0f) {
                mRailSpeed = 0.0f;
            }
        }
    }

    mRailCoord += mRailSpeed;

    if (mRailRider->isLoop()) {
        if (mRailCoord < 0.0f) {
            mRailCoord += mRailRider->getTotalLength();
        } else if (mRailCoord > mRailRider->getTotalLength()) {
            mRailCoord -= mRailRider->getTotalLength();
        }
    } else {
        if (mRailCoord < 0.0f) {
            mRailCoord = 0.0f;
            mRailSpeed = 0.0f;
        } else if (mRailCoord > mRailRider->getTotalLength()) {
            mRailCoord = mRailRider->getTotalLength();
            mRailSpeed = 0.0f;
        }
    }

    mRailRider->setCoord(mRailCoord);
    CameraLocalUtil::setPos(this, mRailRider->mCurPos);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    TPos3f mtx = mZoneMatrix;
    TVec3f up(0.0f, 1.0f, 0.0f);
    mtx.mult33(up);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

void CameraRailFollow::setParam(s32 zoneID, s32 railID, f32 stopMargin, f32 railSpeedMax, f32 accel, s32 marginScale) {
    mRailRider = MR::getCameraDirector()->mRailHolder->getRider(zoneID, railID);
    mTargetMargin = stopMargin;
    mRailSpeedMax = railSpeedMax;
    mRailAccel = accel;
    mMarginScale = marginScale;
}

CamTranslatorBase* CameraRailFollow::createTranslator() {
    return new CamTranslatorRailFollow(this);
}
