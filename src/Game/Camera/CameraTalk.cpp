#include "Game/Camera/CameraTalk.hpp"
#include "Game/Camera/CamTranslatorTalk.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTalk_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)(MR::epsilon());
    (void)0.5f;
    (void)-1.0f;
    (void)(MR::pi() / 2.0f);
    (void)(-MR::pi() / 2.0f);
}

namespace {
    static const f32 sRate = 0.5f;
    static const f32 sAngleX = 0.12f;
    static const f32 sAngleY = 0.7f;
};  // namespace

CameraTalk::CameraTalk(const char* pName)
    : Camera(pName), mPos(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mHeight(120.0f), mDist(450.0f), mAxisReversed() {
}

void CameraTalk::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    calc();
}

CameraTargetObj* CameraTalk::calc() {
    TQuat4f rotUp;
    rotUp.setRotate(mUp, -CameraLocalUtil::getTarget(this)->getGravityVector(), ::sRate);

    TVec3f up = mUp;
    rotUp.transform(up);

    TVec3f pos = CameraLocalUtil::getTarget(this)->getPosition() * (1.0f - ::sRate) + mPos * ::sRate;
    TVec3f front = CameraLocalUtil::getTarget(this)->getPosition() - mPos;
    if (!MR::isNearZero(front)) {
        MR::normalize(&front);
    } else {
        front.set< f32 >(0.0f, 0.0f, 1.0f);
    }

    TVec3f side = up.cross(front);
    if (MR::isNearZero(side)) {
        return nullptr;
    }
    MR::normalize(&side);
    front.cross(side, up);

    TVec3f posDiff = CameraLocalUtil::getPos(this) - CameraLocalUtil::getTarget(this)->getPosition();

    TVec3f orientCross = posDiff.cross(front);
    f32 orientation = orientCross.dot(up);
    if (orientation >= 0.0f) {
        mAxisReversed = true;
    } else {
        mAxisReversed = false;
    }

    TPos3f transMtx;
    transMtx.makeTrans(0.0f, 0.0f, mDist);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(-1.0f, 0.0f, 0.0f), ::sAngleX);

    TPos3f rotY;
    if (front.dot(posDiff) >= 0.0f) {
        if (orientation >= 0.0f) {
            rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), -::sAngleY);
        } else {
            rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), ::sAngleY);
        }

    } else {
        if (orientation >= 0.0f) {
            rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), -MR::pi() / 2.0f);
        } else {
            rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::pi() / 2.0f);
        }
    }

    TPos3f mtx;
    mtx.identity();
    mtx.setXDir(side);
    mtx.setYDir(up);
    mtx.setZDir(front);
    mtx.setTrans(pos);

    mtx.concat(mtx, rotY);
    mtx.concat(mtx, rotX);
    mtx.concat(mtx, transMtx);

    TVec3f basePos;
    mtx.getTrans(basePos);

    CameraLocalUtil::setWatchPos(this, pos + up * mHeight);
    CameraLocalUtil::setPos(this, basePos + up * mHeight);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

void CameraTalk::setParam(const TVec3f& pos, const TVec3f& up, f32 height, f32 dist) {
    mPos.set(pos);
    mUp.set(up);
    mHeight = height;
    mDist = dist;
}

CamTranslatorBase* CameraTalk::createTranslator() {
    return new CamTranslatorTalk(this);
}
