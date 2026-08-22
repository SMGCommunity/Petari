#include "Game/Camera/CameraTowerBase.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTowerBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

CameraTowerBase::~CameraTowerBase() {
}

CameraTowerBase::CameraTowerBase(const char* pName) : Camera(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 1.0f, 0.0f), mAngleX(), mDist(2000.0f) {
}

void CameraTowerBase::reset() {
    resetBegin();

    if (mVPan != nullptr) {
        mVPan->resetJump();
        mVPan->calcWatchPos(nullptr);
    } else {
        TVec3f watchPoint;
        CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
        CameraLocalUtil::setWatchPos(this, watchPoint);
    }

    calcIdealPoseOfTowerBase();

    resetPreVPan();
    if (mVPan != nullptr) {
        mVPan->reset();
    }
    resetPostVPan();
}

CameraTargetObj* CameraTowerBase::calc() {
    if (mVPan != nullptr) {
        mVPan->updateJump();
        mVPan->calcWatchPos(nullptr);
    } else {
        TVec3f watchPoint;
        CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
        CameraLocalUtil::setWatchPos(this, watchPoint);
    }

    calcIdealPoseOfTowerBase();

    arrangePreVPan();
    if (mVPan != nullptr) {
        mVPan->arrange();
    }
    arrangePostVPan();

    return CameraLocalUtil::getTarget(this);
}

void CameraTowerBase::calcIdealPoseOfTowerBase() {
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);

    if (MR::isNearZero(mAxis)) {
        mAxis.set(0.0f, 1.0f, 0.0f);
    } else {
        mAxis.normalize();
    }

    TVec3f front = watchPos - mWPoint;
    front.set(front - mAxis * mAxis.dot(front));
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    TVec3f up(0.0f, 1.0f, 0.0f);
    TVec3f side = front.cross(up);

    TQuat4f rot;
    rot.setRotate(side, mAngleX);
    rot.transform(front);

    TVec3f pos = front * mDist + watchPos;

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}
