#include "Game/Camera/CameraFix.hpp"
#include "Game/Camera/CamTranslatorFix.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraFix_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraFix::CameraFix(const char* pName) : Camera(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 0.0f, -2500.0f), mUp(0.0f, 1.0f, 0.0f) {
    mTarget = new CameraTargetMtx("カメラターゲットダミー");
}

void CameraFix::reset() {
    calc();
}

CameraTargetObj* CameraFix::calc() {
    TVec3f axis = mAxis;
    TVec3f watchPos = mWPoint;
    TVec3f up = mUp;
    mZoneMatrix.mult33(axis);
    mZoneMatrix.mult(watchPos, watchPos);
    mZoneMatrix.mult33(up);

    CameraLocalUtil::setLocalOffset(this, TVec3f(0.0f, 0.0f, 0.0f));
    mTarget->setMtx(mZoneMatrix);
    mTarget->mMatrix.setTrans(watchPos);
    mTarget->movement();

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPointImm(&watchPoint, this, mTarget);

    TVec3f front = axis;
    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    if (MR::abs(front.dot(up)) > 0.98f) {
        up.set< f32 >(front.y, front.z, front.x);
    }

    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setWatchUpVec(this, up);
    CameraLocalUtil::setPos(this, watchPos - axis);
    CameraLocalUtil::setUpVec(this, up);
    return mTarget;
}

void CameraFix::setParam(const TVec3f& rWPoint, const TVec3f& rAxis, const TVec3f& rUp) {
    mWPoint.set(rWPoint);
    mAxis.set(rAxis);
    mUp.set(rUp);
}

CamTranslatorBase* CameraFix::createTranslator() {
    return new CamTranslatorFix(this);
}
