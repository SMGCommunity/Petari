#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Util/GravityUtil.hpp"

void CameraTargetObj::init(const JMapInfoIter &rIter) {

}

bool CameraTargetObj::isTurning() const {
    return false;
}

bool CameraTargetObj::isLongDrop() const {
    return false;
}

bool CameraTargetObj::isBeeMode() const {
    return false;
}

u32 CameraTargetObj::getSpecialMode() const {
    return 0;
}

GravityInfo *CameraTargetObj::getGravityInfo() const {
    return nullptr;
}

void CameraTargetObj::enableCameraWall() {
    mCameraWall = true;
}

void CameraTargetObj::disableCameraWall() {
    mCameraWall = false;
}

void CameraTargetObj::setCameraWall(bool enable) {
    mCameraWall = enable;
}

bool CameraTargetObj::isDebugMode() const {
    return false;
}

void CameraTargetObj::resetStatus() {
    
}

CameraTargetMtx::CameraTargetMtx(const char *pName) : CameraTargetObj(pName) {
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mLastMove.x = 0.0f;
    mLastMove.y = 0.0f;
    mLastMove.z = 0.0f;
    mGravityVector.x = 0.0f;
    mGravityVector.y = -1.0f;
    mGravityVector.z = 0.0f;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mFront.x = 0.0f;
    mFront.y = 0.0f;
    mFront.z = 1.0f;
    mSide.x = 0.0f;
    mSide.y = 0.0f;
    mSide.z = 1.0f;
    mInvalidLastMove = false;
    mCameraArea = nullptr;
    mMatrix.identity();
}

CameraTargetMtx::~CameraTargetMtx() {

}

#ifdef NON_MATCHING
// Wrong instruction order for the first 4 TVec3f.set(), register mismatch
void CameraTargetMtx::movement() {
    mUp.set(mMatrix.mMtx[0][1], mMatrix.mMtx[1][1], mMatrix.mMtx[2][1]);
    mFront.set(mMatrix.mMtx[0][2], mMatrix.mMtx[1][2], mMatrix.mMtx[2][2]);
    mSide.set(mMatrix.mMtx[0][0], mMatrix.mMtx[1][0], mMatrix.mMtx[2][0]);

    TVec3f position;
    position.set(mMatrix.mMtx[0][3], mMatrix.mMtx[1][3], mMatrix.mMtx[2][3]);

    if (mInvalidLastMove) {
        mInvalidLastMove = false;
        mLastMove.z = 0.0f;
        mLastMove.y = 0.0f;
        mLastMove.x = 0.0f;
    }
    else {
        register TVec3f &rPos = position;
        register TVec3f &thisPos = mPosition;
        register TVec3f &thisLastMove = mLastMove;

        __asm {
            psq_l   f0, 0(rPos), 0, 0
            psq_l   f1, 0(thisPos), 0, 0
            psq_l   f2, 8(thisPos), 1, 0
            ps_sub  f0, f0, f1
            psq_st  f0, 0(thisLastMove), 0, 0
            psq_l   f0, 8(rPos), 1, 0
            ps_sub  f0, f0, f2
            psq_st  f0, 8(thisLastMove), 1, 0
        }
    }

    mPosition.x = position.x;
    mPosition.y = position.y;
    mPosition.z = position.z;

    AreaObj *area = MR::getAreaObj("CubeCamera", mPosition);

    if (area == nullptr) {
        mCameraArea = nullptr;
    }
    else {
        mCameraArea = reinterpret_cast<CubeCameraArea *>(area);
    }

    MR::calcGravityVectorOrZero(this, mPosition, &mGravityVector, nullptr, 0);
}
#endif

const TVec3f *CameraTargetMtx::getPosition() const {
    return &mPosition;
}

const TVec3f *CameraTargetMtx::getUpVec() const {
    return &mUp;
}

const TVec3f *CameraTargetMtx::getFrontVec() const {
    return &mFront;
}

const TVec3f *CameraTargetMtx::getSideVec() const {
    return &mSide;
}

const TVec3f *CameraTargetMtx::getLastMove() const {
    return &mLastMove;
}

const TVec3f *CameraTargetMtx::getGroundPos() const {
    return &mPosition;
}

const TVec3f *CameraTargetMtx::getGravityVector() const {
    return &mGravityVector;
}

CubeCameraArea *CameraTargetMtx::getCubeCameraArea() const {
    return mCameraArea;
}

void CameraTargetMtx::invalidateLastMove() {
    mInvalidLastMove = true;
}