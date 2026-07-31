#include "Game/Camera/CameraBlackHole.hpp"
#include "Game/Camera/CamTranslatorBlackHole.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

void CameraBlackHole_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)(f32) JGeometry::TUtil< f32 >::epsilon();
}

CameraBlackHole::CameraBlackHole(const char* pName) : Camera(pName), mFovy(1.0f), mRoll(), mWPoint(0.0f, 0.0f, 0.0f), mBasePos(0.0f, 0.0f, 0.0f) {
}

void CameraBlackHole::reset() {
    mFovy = CameraLocalUtil::getFovy(mCameraMan);
    mRoll = CameraLocalUtil::getRoll(mCameraMan);
    TVec3f camWatchDir = mBasePos - mWPoint;
    CameraMan* camMan = mCameraMan;
    TVec3f watchDir = CameraLocalUtil::getPos(camMan) - CameraLocalUtil::getWatchPos(camMan);
    if (MR::isNearZero(camWatchDir) || MR::isNearZero(watchDir)) {
        CameraLocalUtil::setWatchPos(this, mWPoint);
        CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
        CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getWatchUpVec(mCameraMan));
        CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    } else {
        f32 camWatchDist = camWatchDir.length();
        f32 targetDist = (CameraLocalUtil::getPos(mCameraMan) - mWPoint).length();
        f32 targetLength = camWatchDist + (targetDist - camWatchDist) * 0.7f;
        TVec3f up = CameraLocalUtil::getUpVec(mCameraMan);
        TVec3f watchUp = CameraLocalUtil::getWatchUpVec(mCameraMan);
        MR::normalize(&camWatchDir);
        MR::normalize(&watchDir);
        TQuat4f rot;
        rot.setRotate(watchDir, camWatchDir);
        rot.transform(up);
        CameraLocalUtil::setUpVec(this, up);
        rot.transform(watchUp);
        CameraLocalUtil::setWatchUpVec(this, watchUp);
        camWatchDir.setLength(targetLength);
        CameraLocalUtil::setWatchPos(this, mWPoint);
        CameraLocalUtil::setPos(this, camWatchDir + mWPoint);
    }
}

CameraTargetObj* CameraBlackHole::calc() {
    CameraLocalUtil::setFovy(this, mFovy);
    CameraLocalUtil::setRoll(this, mRoll);
    mRoll += 0.002f;
    return nullptr;
}

CamTranslatorBase* CameraBlackHole::createTranslator() {
    return new CamTranslatorBlackHole(this);
}
