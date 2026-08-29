#include "Game/Camera/CameraCubePlanet.hpp"
#include "Game/Camera/CamTranslatorCubePlanet.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

void CameraCubePlanet_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 f3 = MR::epsilon();
    (void)0.5f;
}

CameraCubePlanet::CameraCubePlanet(const char* pName)
    : Camera(pName), mDist(3000.0f), mAngleX(MR::pi() / 6.0f), mAngleY(0.35f), mUp(0.0f, 1.0f, 0.0f) {
}

void CameraCubePlanet::reset() {
    mUp.set(-CameraLocalUtil::getTarget(this)->getGravityVector());
}

CameraTargetObj* CameraCubePlanet::calc() {
    TVec3f pos = CameraLocalUtil::getPos(mCameraMan);
    TVec3f up = mUp;

    TQuat4f upRot;
    upRot.setRotate(mUp, -CameraLocalUtil::getTarget(this)->getGravityVector(), CameraLocalUtil::isForceCameraChange() ? 1.0f : mAngleY);
    upRot.transform(up);
    mUp.set(up);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPointImm(&watchPoint, this, CameraLocalUtil::getTarget(this));
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setWatchUpVec(this, up);

    TVec3f toWatch = watchPoint - pos;
    TVec3f front = toWatch - up * up.dot(toWatch);
    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    TVec3f side = up.cross(front);
    TQuat4f rot;
    rot.setRotate(side, mAngleX);
    rot.transform(up);
    rot.transform(front);
    front.setLength(mDist);
    CameraLocalUtil::setPos(this, watchPoint - front);
    CameraLocalUtil::setUpVec(this, up);
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraCubePlanet::createTranslator() {
    return new CamTranslatorCubePlanet(this);
}
