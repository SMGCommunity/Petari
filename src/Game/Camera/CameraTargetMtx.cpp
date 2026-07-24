#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/GravityUtil.hpp"

void CameraTargetMtx_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraTargetMtx::CameraTargetMtx(const char* pName)
    : CameraTargetObj(pName), mPosition(0.0f, 0.0f, 0.0f), mLastMove(0.0f, 0.0f, 0.0f), mGravityVector(0.0f, -1.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f),
      mFront(0.0f, 0.0f, 1.0f), mSide(0.0f, 0.0f, 1.0f), mInvalidLastMove(), mCameraArea() {
    mMatrix.identity();
}

const TVec3f& CameraTargetMtx::getPosition() const {
    return mPosition;
}

const TVec3f& CameraTargetMtx::getUpVec() const {
    return mUp;
}

const TVec3f& CameraTargetMtx::getFrontVec() const {
    return mFront;
}

const TVec3f& CameraTargetMtx::getSideVec() const {
    return mSide;
}

const TVec3f& CameraTargetMtx::getLastMove() const {
    return mLastMove;
}

const TVec3f& CameraTargetMtx::getGroundPos() const {
    return mPosition;
}

const TVec3f& CameraTargetMtx::getGravityVector() const {
    return mGravityVector;
}

CubeCameraArea* CameraTargetMtx::getCubeCameraArea() const {
    return mCameraArea;
}

void CameraTargetMtx::invalidateLastMove() {
    mInvalidLastMove = true;
}

CameraTargetMtx::~CameraTargetMtx() {
}

void CameraTargetMtx::movement() {
    mMatrix.getYDir(mUp);
    mMatrix.getZDir(mFront);
    mMatrix.getXDir(mSide);

    TVec3f position;
    mMatrix.getTrans(position);

    if (mInvalidLastMove) {
        mInvalidLastMove = false;
        mLastMove.z = 0.0f;
        mLastMove.y = 0.0f;
        mLastMove.x = 0.0f;
    } else {
        mLastMove.sub(position, mPosition);
    }

    mPosition.set(position);

    AreaObj* area = MR::getAreaObj("CubeCamera", mPosition);

    if (area == nullptr) {
        mCameraArea = nullptr;
    } else {
        mCameraArea = static_cast< CubeCameraArea* >(area);
    }

    MR::calcGravityVectorOrZero(this, mPosition, &mGravityVector, nullptr, 0);
}
