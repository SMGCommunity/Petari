#include "Game/Camera/CameraSlide.hpp"
#include "Game/Camera/CamTranslatorSlide.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraSlide_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)MR::pi();
}

CameraSlide::CameraSlide(const char* pName)
    : Camera(pName), mAxis(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mWPoint(0.0f, 0.0f, 0.0f), mAngleA(0.0f), mDist(1000.0f) {
}

void CameraSlide::reset() {
}

CameraTargetObj* CameraSlide::calc() {
    TVec3f front = mAxis.cross(mUp);
    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    TVec3f up = front.cross(mAxis);
    MR::normalize(&up);

    TQuat4f rot;
    rot.setRotate(front, mAngleA * MR::pi() / 180.0f);

    TVec3f side = mAxis;
    rot.transform(side);

    const TPos3f& mtx = mZoneMatrix;

    mtx.mult33(front);
    mtx.mult33(up);
    mtx.mult33(side);

    TVec3f watchPoint = mWPoint;
    mtx.mult(watchPoint, watchPoint);

    TVec3f camZ = CameraLocalUtil::getTarget(this)->getPosition() - watchPoint;
    TVec3f watchPos = side * side.dot(camZ) + watchPoint;

    TVec3f watchOffset;
    CameraLocalUtil::makeWatchOffset(&watchOffset, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    watchPos.add(watchOffset);

    TVec3f pos = watchPos + front * mDist;
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraSlide::createTranslator() {
    return new CamTranslatorSlide(this);
}
