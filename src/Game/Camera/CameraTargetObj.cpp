#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

CameraTargetObj::CameraTargetObj(const char *pName) : NameObj(pName) {
    mCameraWall = false;
}

CameraTargetActor::CameraTargetActor(const char *pName) : CameraTargetObj(pName) {
    mActor = nullptr;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mFront.x = 0.0f;
    mFront.y = 0.0f;
    mFront.z = 1.0f;
    mSide.x = 0.0f;
    mSide.y = 0.0f;
    mSide.z = 1.0f;
    mCameraArea = nullptr;
}

CameraTargetActor::~CameraTargetActor() {

}

void CameraTargetActor::movement() {
    if (MR::isDead(mActor) || MR::isClipped(mActor)) {
        return;
    }

    if (mActor->getBaseMtx() != nullptr) {
        MR::calcUpVec(&mUp, mActor);
        MR::calcFrontVec(&mFront, mActor);
        MR::calcSideVec(&mSide, mActor);
    }
    else {
        TRot3f matrix;
        MR::makeMtxRotate(reinterpret_cast<MtxPtr>(&matrix), mActor->mRotation.x, mActor->mRotation.y, mActor->mRotation.z);

        matrix.getYDir(mUp);
        matrix.getZDir(mFront);
        matrix.getXDir(mSide);
    }

    CubeCameraArea *area = reinterpret_cast<CubeCameraArea *>(MR::getAreaObj("CubeCamera", mActor->mPosition));

    if (area == nullptr) {
        mCameraArea = nullptr;
    }
    else {
        mCameraArea = area;
    }
}

const TVec3f *CameraTargetActor::getPosition() const {
    return &mActor->mPosition;
}

const TVec3f *CameraTargetActor::getUpVec() const {
    return &mUp;
}

const TVec3f *CameraTargetActor::getFrontVec() const {
    return &mFront;
}

const TVec3f *CameraTargetActor::getSideVec() const {
    return &mSide;
}

const TVec3f *CameraTargetActor::getLastMove() const {
    return &mActor->mVelocity;
}

const TVec3f *CameraTargetActor::getGroundPos() const {
    return &mActor->mPosition;
}

const TVec3f *CameraTargetActor::getGravityVector() const {
    if (getGravityInfo() != nullptr) {
        return &getGravityInfo()->mGravityVector;
    }
    else {
        return &mUp;
    }
}

f32 CameraTargetActor::getRadius() const {
    return 150.0f;
}

CubeCameraArea *CameraTargetActor::getCubeCameraArea() const {
    return mCameraArea;
}

Triangle *CameraTargetActor::getGroundTriangle() const {
    return nullptr;
}

CameraTargetPlayer::CameraTargetPlayer(const char *pName) : CameraTargetObj(pName) {
    mGravity.x = 0.0f;
    mGravity.y = -1.0f;
    mGravity.z = 0.0f;
    mGroundPos.x = 0.0f;
    mGroundPos.y = 0.0f;
    mGroundPos.z = 0.0f;
    mCameraArea = nullptr;
    mGroundTriangle = nullptr;
    _58 = 0;
    _5A = true;
}

CameraTargetPlayer::~CameraTargetPlayer() {

}

const TVec3f *CameraTargetPlayer::getUpVec() const {
    return &mUp;
}

const TVec3f *CameraTargetPlayer::getFrontVec() const {
    return &mFront;
}

const TVec3f *CameraTargetPlayer::getSideVec() const {
    return &mSide;
}

const TVec3f *CameraTargetPlayer::getGroundPos() const {
    return &mGroundPos;
}

const TVec3f *CameraTargetPlayer::getGravityVector() const {
    return &mGravity;
}