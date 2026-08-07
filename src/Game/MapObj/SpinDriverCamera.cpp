#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"

SpinDriverCamera::SpinDriverCamera() : mCamera(), mTargetMtx(), mCameraInfo(), mAppearCameraFrame() {
}

void SpinDriverCamera::startAppearCamera(LiveActor* pActor, const TVec3f& a2, const TVec3f& a3, const TVec3f& a4) {
    if (mCameraInfo == nullptr) {
        return;
    }

    mAppearCameraFrame = MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用");

    if (mAppearCameraFrame <= 0) {
        return;
    }

    MR::startMultiActorCameraTargetOther(pActor, mCameraInfo, "出現イベント用", CameraTargetArg(mTargetMtx), -1);

    TPos3f upPos;
    MR::makeMtxUpFrontPos(&upPos, a2, a3, a4);
    mTargetMtx->setMtx(upPos.toMtxPtr());
}

void SpinDriverCamera::endAppearCamera(LiveActor* pActor) {
    if (mCameraInfo == nullptr) {
        return;
    }

    if (mAppearCameraFrame <= 0) {
        return;
    }

    MR::endMultiActorCamera(pActor, mCameraInfo, "出現イベント用", false, -1);
}

s32 SpinDriverCamera::getAppearCameraFrames() const {
    return mAppearCameraFrame;
}

void SpinDriverCamera::start(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3) {
    if (mCamera == nullptr) {
        return;
    }

    mCamera->start(CameraTargetArg(mTargetMtx), 0);

    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, a1, a2, a3);
    mTargetMtx->setMtx(mtx.toMtxPtr());
}

void SpinDriverCamera::update(const TVec3f& a1, const TVec3f& a2) {
    if (mCamera == nullptr) {
        return;
    }

    switch (mCamera->isEnd()) {
    case false:
        mCamera->update();
        updateTargetMatrix(a1, a2);
        break;
    }
}

void SpinDriverCamera::cancel() {
    if (mCamera == nullptr) {
        return;
    }

    mCamera->endForceSoon();
}

void SpinDriverCamera::end() {
    if (mCamera == nullptr) {
        return;
    }

    mCamera->endForceAtLanding();
    mCamera->changeTargetPlayer();
}

// SpinDriverCamera::updateTargetMatrix

bool SpinDriverCamera::isUseAppearCamera(LiveActor* pActor) const {
    if (mCameraInfo == nullptr) {
        return false;
    }

    return MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用") > 0;
}

void SpinDriverCamera::init(const JMapInfoIter& rIter, LiveActor* pActor) {
    ActorCameraInfo info(rIter);

    if (info.mCameraSetID == -1) {
        return;
    }

    s32 arg3 = 1;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);

    mCamera = new MultiEventCamera();
    mCamera->setUp(pActor->mName, new ActorCameraInfo(rIter), arg3);
    mCamera->setEndCameraTypeAtLanding();

    mTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
}

void SpinDriverCamera::initAppearCamera(const JMapInfoIter& rIter, LiveActor* pActor) {
    ActorCameraInfo info(rIter);

    if (info.mCameraSetID == -1) {
        return;
    }

    MR::initMultiActorCamera(pActor, rIter, &mCameraInfo, "出現イベント用");

    mAppearCameraFrame = MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用");
}
