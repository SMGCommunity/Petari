#include "Game/Camera/CameraCharmedVecReg.hpp"
#include "Game/Camera/CamTranslatorCharmedVecReg.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

void CameraCharmedVecReg_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sRate = 0.02f;
};  // namespace

CameraCharmedVecReg::CameraCharmedVecReg(const char* pName)
    : Camera(pName), mName(), mDist(1000.0f), mAngleX(), mAngleYMin(), mAngleYMax(), mCamDistRatio(0.5f), mCamAngleRate(::sRate) {
}

void CameraCharmedVecReg::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

CameraTargetObj* CameraCharmedVecReg::calc() {
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f watchOffs = watchPoint - CameraLocalUtil::getTarget(this)->getPosition();

    const TVec3f& vecReg = CameraLocalUtil::getVecReg(mName);

    TVec3f basePos = CameraLocalUtil::getTarget(this)->getPosition() * (1.0f - mCamDistRatio) + vecReg * mCamDistRatio;

    TVec3f up = -CameraLocalUtil::getTarget(this)->getGravityVector();
    TVec3f front = CameraLocalUtil::getTarget(this)->getPosition() - vecReg;
    MR::projectVecOnPlane(&front, front, up);

    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return nullptr;
    }
    MR::normalize(&side);

    TPos3f mtx;
    mtx.identity();
    mtx.setXYZDir(side, up, front);

    TVec3f camDir = CameraLocalUtil::getPos(this) - CameraLocalUtil::getTarget(this)->getPosition();
    MR::projectVecOnPlane(&camDir, camDir, up);
    if (MR::isNearZero(camDir)) {
        return nullptr;
    }
    MR::normalize(&camDir);

    TVec3f camUp = front.cross(camDir);
    f32 angleDiff = camDir.angle(front);
    f32 angle = angleDiff;
    if (angle < mAngleYMin) {
        angle = mAngleYMin;
    } else if (angle > mAngleYMax) {
        angle = mAngleYMax;
    }

    if (camUp.dot(up) < 0.0f) {
        angle = -angle;
        angleDiff = -angleDiff;
    }
    angle = angleDiff * (1.0f - mCamAngleRate) + angle * mCamAngleRate;

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), -mAngleX);

    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), angle);

    mtx.concat(rotY);
    mtx.concat(rotX);

    TVec3f posOffs(0.0f, 0.0f, mDist);
    mtx.mult33(posOffs);

    CameraLocalUtil::setWatchPos(this, basePos + watchOffs);
    CameraLocalUtil::setPos(this, posOffs + CameraLocalUtil::getTarget(this)->getPosition());
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, up);

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraCharmedVecReg::createTranslator() {
    return new CamTranslatorCharmedVecReg(this);
}
