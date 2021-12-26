#include "Game/AreaObj/CubeCamera.h"
#include "Game/Camera/CameraTargetObj.h"
#include "Game/Gravity/GravityInfo.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/LiveActorUtil.h"
#include "JSystem/JGeometry/TRotation.h"

CameraTargetObj::CameraTargetObj(const char *pName) : NameObj(pName) {
    mCameraWall = false;
}

CameraTargetActor::CameraTargetActor(const char *pName) : CameraTargetObj(pName) {
    mLiveActor = NULL;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mFront.x = 0.0f;
    mFront.y = 0.0f;
    mFront.z = 1.0f;
    mSide.x = 0.0f;
    mSide.y = 0.0f;
    mSide.z = 1.0f;
    mCameraArea = NULL;
}

CameraTargetActor::~CameraTargetActor() {

}

void CameraTargetActor::movement() {
    if (MR::isDead(mLiveActor) || MR::isClipped(mLiveActor)) {
        return;
    }

    if (mLiveActor->getBaseMtx() != NULL) {
        MR::calcUpVec(&mUp, mLiveActor);
        MR::calcFrontVec(&mFront, mLiveActor);
        MR::calcSideVec(&mSide, mLiveActor);
    }
    else {
        TRot3f matrix;
        MR::makeMtxRotate(reinterpret_cast<MtxPtr>(&matrix), mLiveActor->mRotation.x, mLiveActor->mRotation.y, mLiveActor->mRotation.z);

        matrix.getYDir(mUp);
        matrix.getZDir(mFront);
        matrix.getXDir(mSide);
    }

    CubeCameraArea *area = reinterpret_cast<CubeCameraArea *>(MR::getAreaObj("CubeCamera", mLiveActor->mPosition));

    if (area == NULL) {
        mCameraArea = NULL;
    }
    else {
        mCameraArea = area;
    }
}

const TVec3f *CameraTargetActor::getPosition() const {
    return &mLiveActor->mPosition;
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
    return &mLiveActor->mVelocity;
}

const TVec3f *CameraTargetActor::getGroundPos() const {
    return &mLiveActor->mPosition;
}

const TVec3f *CameraTargetActor::getGravityVector() const {
    if (getGravityInfo() != NULL) {
        return &getGravityInfo()->_0;
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

void *CameraTargetActor::getGroundTriangle() const {
    return NULL;
}

CameraTargetPlayer::CameraTargetPlayer(const char *pName) : CameraTargetObj(pName) {
    _38 = 0.0f;
    _3C = -1.0f;
    _40 = 0.0f;
    _44 = 0.0f;
    _48 = 0.0f;
    _4C = 0.0f;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    _5A = 1;
}