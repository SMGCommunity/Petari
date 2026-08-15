#include "Game/Camera/CameraSubjective.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraSubjective_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.0f;
    (void)-1.0f;
    (void)MR::pi();
    (void)2.0f;
}

namespace {
    static const f32 sDistance = 200.0f;
    static const f32 sHeight = 120.0f;
    static const f32 sDivW = 10.0f;
    static const f32 sDivH = 10.0f;
    static const f32 sRotXMax = 80.0f;
    static const f32 sRotYMax = 90.0f;
    static const f32 sPadStickPlay = 0.3f;
    static const f32 sRotSpeedEff = 3.0f;
};  // namespace

CameraSubjective::CameraSubjective(const char* pName) : Camera(pName), mAngleVel(0.0f, 0.0f), mAngle(0.0f, 0.0f) {
    mMtx.identity();
}

void CameraSubjective::reset() {
    mAngle.zero();

    CameraLocalUtil::setPos(this, CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * ::sHeight);

    mMtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mMtx.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
    mMtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
    mMtx.setTrans(TVec3f(0.0f, 0.0f, 0.0f));

    mAngleVel.zero();
}

CameraTargetObj* CameraSubjective::calc() {
    TVec2f accel(0.0f, 0.0f);

    TVec2f stick(0.0f, 0.0f);
    if (MR::getSubPadStickX(WPAD_CHAN0) < -::sPadStickPlay) {
        stick.x = (MR::getSubPadStickX(WPAD_CHAN0) + ::sPadStickPlay) / (1.0f - ::sPadStickPlay);
    } else if (MR::getSubPadStickX(WPAD_CHAN0) > ::sPadStickPlay) {
        stick.x = (MR::getSubPadStickX(WPAD_CHAN0) - ::sPadStickPlay) / (1.0f - ::sPadStickPlay);
    }

    if (MR::getSubPadStickY(WPAD_CHAN0) < -::sPadStickPlay) {
        stick.y = (MR::getSubPadStickY(WPAD_CHAN0) + ::sPadStickPlay) / (1.0f - ::sPadStickPlay);
    } else if (MR::getSubPadStickY(WPAD_CHAN0) > ::sPadStickPlay) {
        stick.y = (MR::getSubPadStickY(WPAD_CHAN0) - ::sPadStickPlay) / (1.0f - ::sPadStickPlay);
    }

    if (stick.x > 1.0f) {
        stick.x = 1.0f;
    } else if (stick.x < -1.0f) {
        stick.x = -1.0f;
    }

    if (stick.y > 1.0f) {
        stick.y = 1.0f;
    } else if (stick.y < -1.0f) {
        stick.y = -1.0f;
    }

    stick.y *= -1.0f;

    accel.x -= stick.x;
    accel.y -= stick.y;

    mAngle.x = MR::clamp(mAngle.x + mAngleVel.x * ::sRotSpeedEff, -45.0f, ::sRotYMax);
    mAngle.y = MR::clamp(mAngle.y + mAngleVel.y * ::sRotSpeedEff, -::sRotXMax, ::sRotXMax);

    mAngleVel.x *= 0.9f;
    mAngleVel.y *= 0.9f;

    mAngleVel.x = MR::clamp(mAngleVel.x + accel.y / ::sDivH, -1.0f, 2.0f);
    mAngleVel.y = MR::clamp(mAngleVel.y + accel.x / ::sDivW, -1.0f, 2.0f);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), mAngle.x * MR::pi() / 180.0f);
    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngle.y * MR::pi() / 180.0f);

    TPos3f rot;
    rot.concat(mMtx, rotY);
    rot.concat(rotX);

    TVec3f front;
    rot.getZDir(front);
    front.scale(-::sDistance);
    TVec3f up;
    rot.getYDir(up);

    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getPos(this) - front);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraSubjective::createTranslator() {
    return new CamTranslatorDummy(this);
}
