#include "Game/Camera/CameraManPause.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

void CameraManPause_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
}

namespace {
    static const f32 sDistance = 6000.0f;
};  // namespace

CameraManPause::CameraManPause(const char* pName)
    : CameraMan(pName), mDistBase(::sDistance), mDistOffset(), mAngle(0.0f, 0.0f), mBasePos(3230.0f, 13350.0f, 520.0f) {
    CameraLocalUtil::setFovy(this, 45.0f);
}

void CameraManPause::init(const JMapInfoIter& rIter) {
}

void CameraManPause::calc() {
    if (CameraLocalUtil::getTarget(this) == nullptr) {
        return;
    }

    TPos3f mtx;
    if (CameraLocalUtil::getTarget(this)->getMapBaseMtx() != nullptr) {
        mtx.set(*CameraLocalUtil::getTarget(this)->getMapBaseMtx());
    } else {
        mtx.identity();
    }

    TVec3f pos = CameraLocalUtil::getTarget(this)->getPosition();
    TVec3f watchUp, watchPos;
    mtx.getYDir(watchUp);
    mtx.getTrans(watchPos);

    mtx.setTrans(watchPos + watchUp * watchUp.dot(pos - watchPos));

    TPos3f inv = mtx;
    inv.invert(inv);
    inv.mult(pos, pos);

    f32 length, angleX, angleY;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), pos, &length, &angleX, &angleY);

    if (MR::testSubPadButtonC(WPAD_CHAN0)) {
        mDistOffset -= MR::getSubPadStickY(WPAD_CHAN0) * 200.0f;
    } else {
        mAngle.x += MR::getSubPadStickY(WPAD_CHAN0) * 0.1f;
        mAngle.y += MR::getSubPadStickX(WPAD_CHAN0) * 0.1f;
    }

    MR::clampBoth(&mDistOffset, -mDistBase * 0.7f, mDistBase * 4.5f);
    MR::clampBoth(&mAngle.x, -1.4959966f, 1.4959966f);  // TODO: result of a const calculation?

    if (MR::testSubPadButtonC(WPAD_CHAN0) && MR::testSubPadTriggerZ(WPAD_CHAN0)) {
        mDistOffset = mDistBase * 4.5f;
        mAngle.zero();
    }

    TVec3f newPos;
    MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &newPos, mDistBase + mDistOffset + length, mAngle.x, mAngle.y + angleY);

    mtx.mult(newPos, newPos);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(-1.0f, 0.0f, 0.0f), mAngle.x);

    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngle.y + angleY);

    TPos3f rotMtx;
    rotMtx.concat(rotY, rotX);

    TVec3f up;
    rotMtx.getYDir(up);

    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setWatchUpVec(this, watchUp);
    CameraLocalUtil::setPos(this, newPos);
    CameraLocalUtil::setUpVec(this, up);
}

void CameraManPause::notifyActivate() {
    mAngle.x = 0.0f;
    mAngle.y = 0.0f;
    mDistOffset = 0.0f;

    if (CameraLocalUtil::getTarget(this) == nullptr) {
        return;
    }

    f32 dist = (mBasePos - CameraLocalUtil::getTarget(this)->getPosition()).length();

    f32 focalDist = dist * 1.5f / (MR::tan(CameraLocalUtil::getFovy(this) * 0.5f));

    if (focalDist > mDistBase) {
        mDistOffset = focalDist - mDistBase;
    }
}

void CameraManPause::notifyDeactivate() {
}
