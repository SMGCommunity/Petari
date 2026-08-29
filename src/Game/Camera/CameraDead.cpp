#include "Game/Camera/CameraDead.hpp"
#include "Game/Camera/CamTranslatorDead.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

CameraDead::CameraDead(const char* pName) : Camera(pName), mDist(0.5f), mDeadTime(), mCameraType(), mDeadFrame() {
}

void CameraDead::reset() {
    mDeadFrame = 0;
    mFovy = CameraLocalUtil::getFovy(mCameraMan);
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    TVec3f up = CameraLocalUtil::getUpVec(mCameraMan);
    TVec3f front = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    if (!MR::isNearZero(front)) {
        MR::normalize(&front);
        TQuat4f rot;
        rot.setRotate(front, -CameraLocalUtil::getRoll(mCameraMan));
        rot.transform(up);
    }
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getWatchUpVec(mCameraMan));
}

CameraTargetObj* CameraDead::calc() {
    if (mDeadFrame <= mDeadTime) {
        TVec3f watchPoint;
        CameraLocalUtil::makeWatchPointImm(&watchPoint, this, CameraLocalUtil::getTarget(this));

        if (mCameraType != CameraType_FixedPos) {
            f32 t = static_cast< f32 >(mDeadFrame) / mDeadTime;
            t *= t;
            t *= t;

            watchPoint = CameraLocalUtil::getWatchPos(this) + (watchPoint - CameraLocalUtil::getWatchPos(this)) * t;
        }
        CameraLocalUtil::setWatchPos(this, watchPoint);
    }

    if (mDeadFrame <= mDeadTime) {
        f32 f2 = (CameraLocalUtil::getPos(this) - CameraLocalUtil::getTarget(this)->getPosition()).length();

        f32 angle = MR::atan2(CameraLocalUtil::getTarget(this)->getRadius() / mDist,
                              (CameraLocalUtil::getPos(this) - CameraLocalUtil::getTarget(this)->getPosition()).length());

        if (angle > mFovy) {
            angle = mFovy;
        }

        angle *= 180.0f / MR::pi();

        if (mDeadTime != 0 && mDeadFrame <= mDeadTime) {
            f32 t = static_cast< f32 >(mDeadFrame) / mDeadTime;
            t *= t;
            t *= t;

            mFovy += t * (angle - mFovy);
        } else {
            mFovy = angle;
        }
    }

    CameraLocalUtil::setFovy(this, mFovy);
    mDeadFrame++;

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraDead::createTranslator() {
    return new CamTranslatorDead(this);
}
