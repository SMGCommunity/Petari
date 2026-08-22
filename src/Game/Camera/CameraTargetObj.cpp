#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void CameraTargetObj_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static TVec3f sZeroVec(0.0f, 0.0f, 0.0f);
};  // namespace

CameraTargetObj::CameraTargetObj(const char* pName) : NameObj(pName), mCameraWall() {
}

CameraTargetActor::CameraTargetActor(const char* pName)
    : CameraTargetObj(pName), mActor(), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mSide(0.0f, 0.0f, 1.0f), mCameraArea() {
}

void CameraTargetActor::movement() {
    if (MR::isDead(mActor) || MR::isClipped(mActor)) {
        return;
    }

    if (mActor->getBaseMtx() != nullptr) {
        MR::calcUpVec(&mUp, mActor);
        MR::calcFrontVec(&mFront, mActor);
        MR::calcSideVec(&mSide, mActor);
    } else {
        TRot3f matrix;
        MR::makeMtxRotate(matrix, mActor->mRotation.x, mActor->mRotation.y, mActor->mRotation.z);

        matrix.getYDir(mUp);
        matrix.getZDir(mFront);
        matrix.getXDir(mSide);
    }

    CubeCameraArea* area = static_cast< CubeCameraArea* >(MR::getAreaObj("CubeCamera", mActor->mPosition));

    if (area == nullptr) {
        mCameraArea = nullptr;
    } else {
        mCameraArea = area;
    }
}

const TVec3f& CameraTargetActor::getPosition() const {
    return mActor->mPosition;
}

const TVec3f& CameraTargetActor::getUpVec() const {
    return mUp;
}

const TVec3f& CameraTargetActor::getFrontVec() const {
    return mFront;
}

const TVec3f& CameraTargetActor::getSideVec() const {
    return mSide;
}

const TVec3f& CameraTargetActor::getLastMove() const {
    return mActor->mVelocity;
}

const TVec3f& CameraTargetActor::getGroundPos() const {
    return mActor->mPosition;
}

const TVec3f& CameraTargetActor::getGravityVector() const {
    if (getGravityInfo() != nullptr) {
        return getGravityInfo()->mGravityVector;
    } else {
        return mUp;
    }
}

CubeCameraArea* CameraTargetActor::getCubeCameraArea() const {
    return mCameraArea;
}

Triangle* CameraTargetActor::getGroundTriangle() const {
    return nullptr;
}

CameraTargetPlayer::CameraTargetPlayer(const char* pName)
    : CameraTargetObj(pName), mGravity(0.0f, -1.0f, 0.0f), mGroundPos(0.0f, 0.0f, 0.0f), mCameraArea(), mGroundTriangle(), mPlayerMovementTimer(),
      mIsPlayerMoving(true) {
}

void CameraTargetPlayer::movement() {
    if (MR::isDead(mActor) || MR::isClipped(mActor)) {
        return;
    }

    if (MR::isPlayerInBind()) {
        TPos3f mtx;
        mtx.set(MR::getPlayerBaseMtx());
        mtx.getXDir(mSide);
        mtx.getYDir(mUp);
        mtx.getZDir(mFront);
    } else {
        mActor->getUpVec(&mUp);
        mActor->getFrontVec(&mFront);
        mActor->getSideVec(&mSide);
    }

    if (MR::isPlayerElementModeBee()) {
        MR::calcGravityVector(this, mActor->mPosition, &mGravity, nullptr, 0);
    } else {
        mActor->getGravityVector(&mGravity);
    }

    mGroundPos.set(*mActor->getShadowPos());

    CubeCameraArea* area = MR::getCameraCube();
    if (area != nullptr) {
        mCameraArea = area;
    } else {
        mCameraArea = nullptr;
    }

    mGroundTriangle = MR::getPlayerGroundingPolygon();

    if (MR::isNearZero(mUp)) {
        mUp.set(0.0f, 1.0f, 0.0f);
    } else {
        MR::normalize(&mUp);
    }

    if (mPlayerMovementTimer != MR::getPlayerMovementTimer()) {
        mIsPlayerMoving = true;
    } else {
        mIsPlayerMoving = false;
    }

    mPlayerMovementTimer = MR::getPlayerMovementTimer();
}

const TVec3f& CameraTargetPlayer::getPosition() const {
    return mActor->getTransForCamera();
}

const TVec3f& CameraTargetPlayer::getUpVec() const {
    return mUp;
}

const TVec3f& CameraTargetPlayer::getFrontVec() const {
    return mFront;
}

const TVec3f& CameraTargetPlayer::getSideVec() const {
    return mSide;
}

const TVec3f& CameraTargetPlayer::getGroundPos() const {
    return mGroundPos;
}

const TVec3f& CameraTargetPlayer::getGravityVector() const {
    return mGravity;
}

const TVec3f& CameraTargetPlayer::getLastMove() const {
    if (MR::isDemoActive() && !mIsPlayerMoving) {
        return sZeroVec;
    } else {
        return *MR::getPlayerLastMove();
    }
}

bool CameraTargetPlayer::isTurning() const {
    return mActor->isTurning();
}

bool CameraTargetPlayer::isJumping() const {
    return mActor->isJumping();
}

bool CameraTargetPlayer::isLongDrop() const {
    return mActor->isLongDrop();
}

bool CameraTargetPlayer::isFastDrop() const {
    return mActor->isFastDrop();
}

bool CameraTargetPlayer::isFastRise() const {
    return mActor->isFastRise();
}

bool CameraTargetPlayer::isWaterMode() const {
    return MR::isPlayerInWaterMode();
}

bool CameraTargetPlayer::isOnWaterSurface() const {
    return MR::isPlayerOnWaterSurface();
}

bool CameraTargetPlayer::isFooFighterMode() const {
    return MR::isPlayerFlying();
}

u32 CameraTargetPlayer::getSpecialMode() const {
    return mActor->getSpecialMode();
}

bool CameraTargetPlayer::isCameraStateOn(u32 status) const {
    return mActor->isCameraStateOn((SPECIAL_STATUS_FOR_CAMERA)status);
}

CubeCameraArea* CameraTargetPlayer::getCubeCameraArea() const {
    return mCameraArea;
}

Triangle* CameraTargetPlayer::getGroundTriangle() const {
    return mGroundTriangle;
}

GravityInfo* CameraTargetPlayer::getGravityInfo() const {
    return mActor->getGravityInfo();
}

bool CameraTargetPlayer::isDebugMode() const {
    return mActor->isDebugMode();
}

TPos3f* CameraTargetPlayer::getMapBaseMtx() const {
    return reinterpret_cast< TPos3f* >(mActor->getMapBaseMtx());
}

CameraTargetDemoActor::CameraTargetDemoActor(MtxPtr pMtx, const char* pName) : LiveActor(pName) {
    mMtx.set(pMtx);
}

void CameraTargetDemoActor::init(const JMapInfoIter& rIter) {
    LiveActor::init(rIter);
    makeActorAppeared();
}

void CameraTargetDemoActor::setTargetMtx(MtxPtr pNewTargetMtx) {
    mMtx.set(pNewTargetMtx);
    MR::makeRTFromMtxPtr(&mPosition, &mRotation, pNewTargetMtx, true);
}
