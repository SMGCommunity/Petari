#include "Game/Camera/CameraGround.hpp"
#include "Game/Camera/CamTranslatorGround.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/VectorUtil.hpp"

void CameraGround_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraGround::CameraGround(const char* pName)
    : Camera(pName), mAngleA(), mAngleB(), mDist(1000.0f), mUp(0.0f, 1.0f, 0.0f), mLastWatchPoint(0.0f, 0.0f, 0.0f), mIsNotInitialized(true) {
}

void CameraGround::reset() {
    mIsNotInitialized = true;
}

CameraTargetObj* CameraGround::calc() {
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

    TVec3f pos;
    MR::polarToCross(watchPoint, &pos, mDist, mAngleA, mAngleB);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, mUp);
    CameraLocalUtil::setWatchUpVec(this, mUp);

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraGround::createTranslator() {
    return new CamTranslatorGround(this);
}
