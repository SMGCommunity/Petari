#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"

SpinDriverCamera::SpinDriverCamera() {
    mCamera = nullptr;
    mTargetMtx = nullptr;
    mCameraInfo = nullptr;
    mAppearCameraFrame = 0;
}

void SpinDriverCamera::startAppearCamera(LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, const TVec3f &a4) {
    if (mCameraInfo) {
        mAppearCameraFrame = MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用");

        if (mAppearCameraFrame > 0) {
            MR::startMultiActorCameraTargetOther(pActor, mCameraInfo, "出現イベント用", CameraTargetArg(mTargetMtx), -1);
            TPos3f upPos;
            MR::makeMtxUpFrontPos(&upPos, a2, a3, a4);
            mTargetMtx->setMtx(upPos.toMtxPtr());
        }
    }
}

void SpinDriverCamera::endAppearCamera(LiveActor *pActor) {
    if (mCameraInfo && mAppearCameraFrame > 0) {
        MR::endMultiActorCamera(pActor, mCameraInfo, "出現イベント用", false, -1);
    }
}

u32 SpinDriverCamera::getAppearCameraFrames() const {
    return mAppearCameraFrame;
}

void SpinDriverCamera::update(const TVec3f &a1, const TVec3f &a2) {
    if (mCamera) {
        switch(mCamera->isEnd()) {
            case false:
                mCamera->update();
                updateTargetMatrix(a1, a2);
                break;
        }
    }
}

void SpinDriverCamera::cancel() {
    if (mCamera) {
        mCamera->endForceSoon();
    }
}

void SpinDriverCamera::end() {
    if (mCamera) {
        mCamera->endForceAtLanding();
        mCamera->changeTargetPlayer();
    }
}

// SpinDriverCamera::updateTargetMatrix

BOOL SpinDriverCamera::isUseAppearCamera(LiveActor *pActor) const {
    if (!mCameraInfo) {
        return false;
    }
    
    u32 frames = MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用");
    return (-frames & ~frames) >> 31;
}

void SpinDriverCamera::init(const JMapInfoIter &rIter, LiveActor *pActor) {
    ActorCameraInfo info(rIter);

    if (info.mCameraSetID != -1) {
        s32 arg3 = 1;
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        mCamera = new MultiEventCamera();
        mCamera->setUp(pActor->mName, new ActorCameraInfo(rIter), arg3);
        mCamera->setEndCameraTypeAtLanding();
        mTargetMtx =  new CameraTargetMtx("カメラターゲットダミー");
    }
}

void SpinDriverCamera::initAppearCamera(const JMapInfoIter &rIter, LiveActor *pActor) {
    ActorCameraInfo info(rIter);
    if (info.mCameraSetID != -1) {
        MR::initMultiActorCamera(pActor, rIter, &mCameraInfo, "出現イベント用");
        mAppearCameraFrame = MR::getMultiActorCameraFrames(pActor, mCameraInfo, "出現イベント用");
    }
}