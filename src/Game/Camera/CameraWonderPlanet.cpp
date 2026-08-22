#include "Game/Camera/CameraWonderPlanet.hpp"
#include "Game/Camera/CamTranslatorWonderPlanet.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraWonderPlanet_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const s32 sCameraResetFrame = 30;
};  // namespace

CameraWonderPlanet::~CameraWonderPlanet() {
}

CameraWonderPlanet::CameraWonderPlanet(const char* pName)
    : Camera(pName), mDistMin(500.0f), mDistMax(2000.0f), mAngleX(MR::pi() / 4.0f), mIsRotating(), mRotateFrame() {
}

void CameraWonderPlanet::reset() {
    TVec3f watchPosCameraMan = CameraLocalUtil::getWatchPos(mCameraMan);
    TVec3f pos = CameraLocalUtil::getPos(mCameraMan);
    TVec3f up;
    mCameraMan->mMatrix.getYDir(up);

    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f front = watchPosCameraMan - pos;
    MR::normalize(&front);
    TVec3f watchFront = watchPos - pos;
    MR::normalize(&watchFront);

    TQuat4f frontRot;
    frontRot.setRotate(front, watchFront);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    mIsRotating = false;
    mRotateFrame = 0;
}

CameraTargetObj* CameraWonderPlanet::calc() {
    // FIXME
    TVec3f currWatchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f up = CameraLocalUtil::getUpVec(this);

    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f front = currWatchPos - pos;
    MR::normalize(&front);

    TPos3f mtx;
    mtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mtx.setYDir(-CameraLocalUtil::getTarget(this)->getGravityVector());
    mtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
    mtx.setTrans(watchPos);

    TPos3f inv = mtx;
    inv.invert(inv);
    inv.mult(pos, pos);

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), pos, &length, &angleX, &angleY);

    if (angleX < mAngleX) {
        angleX += (mAngleX - angleX) * 0.05f;
        MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &pos, length, angleX, angleY);
    }

    mtx.mult(pos, pos);

    TVec3f watchFront = watchPos - pos;
    MR::normalize(&watchFront);

    TQuat4f frontRot;
    frontRot.setRotate(front, watchFront);
    frontRot.transform(up);

    TVec3f offset = pos - watchPos;
    f32 dist = offset.length();

    if (dist < mDistMin) {
        offset.setLength(mDistMin);
    } else if (dist > mDistMax) {
        offset.setLength(mDistMax);
    }

    pos.set(watchPos + offset);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    updateCameraReset();

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraWonderPlanet::createTranslator() {
    return new CamTranslatorWonderPlanet(this);
}

void CameraWonderPlanet::updateCameraReset() {
    if (mIsRotating) {
        rotateCamera();
    } else {
        checkReset();
    }
}

void CameraWonderPlanet::rotateCamera() {
    if (++mRotateFrame > ::sCameraResetFrame) {
        mIsRotating = false;
    }
}

void CameraWonderPlanet::checkReset() {
    if (!CameraLocalUtil::tryCameraResetTrigger()) {
        return;
    }

    if (MR::isNearZero(CameraLocalUtil::getTarget(this)->getGravityVector())) {
        return;
    }

    TVec3f offset = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    TVec3f front = offset.killElement(CameraLocalUtil::getTarget(this)->getGravityVector());
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    // TODO: this is the first time this specific pattern has come up,
    // killElement and orthogonalize may need to be looked at again.
    TVec3f targetFront = CameraLocalUtil::getTarget(this)->getGravityVector().getOrthogonal(CameraLocalUtil::getTarget(this)->getFrontVec());

    if (MR::isNearZero(targetFront)) {
        return;
    }
    MR::normalize(&targetFront);

    TQuat4f rot;
    rot.setRotate(front, targetFront);
    rot.transform(offset);

    TVec3f up = -CameraLocalUtil::getTarget(this)->getGravityVector();
    if (CameraLocalUtil::getUpVec(this).dot(-CameraLocalUtil::getTarget(this)->getGravityVector()) < 0.0f) {
        up.negate();
    }

    front.set(offset);
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    rot.setRotate(CameraLocalUtil::getTarget(this)->getFrontVec(), front);
    rot.transform(up);

    CameraLocalUtil::setPos(this, CameraLocalUtil::getWatchPos(this) - offset);
    CameraLocalUtil::setUpVec(this, up);

    mIsRotating = true;
    mRotateFrame = 0;

    MR::startCameraInterpolation(::sCameraResetFrame);
}
