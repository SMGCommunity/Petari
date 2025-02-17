#include "Game/Demo/DemoCtrlBase.hpp"
#include "Game/Demo/DemoParamCommonDataTable.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

DemoCtrlBase::DemoCtrlBase(LiveActor *pActor, const char *pName) {
    mActor = pActor;
    mCameraInfo = nullptr;
    mDemoName = pName;
    mCurrentFrame = -1;
    _10 = false;
}

void DemoCtrlBase::init(const JMapInfoIter &rIter) {
    MR::initActorCamera(mActor, rIter, &mCameraInfo);
}

void DemoCtrlBase::end() {
    MR::endDemo(mActor, mDemoName);

    if (!_10) {
        MR::validateClipping(mActor);
    }

    MR::endActorCamera(mActor, mCameraInfo, false, -1);
    if (MR::Demo::isExistCameraShaking(mDemoName)) {
        MR::stopShakingCamera(mActor);
    }

    mCurrentFrame = -1;
}

void DemoCtrlBase::update() {
    if (mCurrentFrame >= 0 && (MR::getActorCameraFrames(mActor, mCameraInfo) <= 0 || mCurrentFrame >= 0)) {
        if (MR::Demo::isExistPadRumble(mDemoName)) {
            MR::tryRumblePad(mActor, MR::Demo::getPadRumble(mDemoName), 0);
        }

        s32 frames = MR::getActorCameraFrames(mActor, mCameraInfo);
        s32 curFrame = mCurrentFrame + 1;
        mCurrentFrame = curFrame;

        if (curFrame == frames) {
            end();
        }
    }
}

bool DemoCtrlBase::isDone() const {
    return mCurrentFrame < 0;
}

bool DemoCtrlBase::isExistEndFrame() const {
    return MR::getActorCameraFrames(mActor, mCameraInfo) > 0;
}

bool DemoCtrlBase::tryStart() {
    if (!MR::tryStartDemo(mActor, mDemoName)) {
        return false;
    }

    mCurrentFrame = 0;
    _10 = MR::isInvalidClipping(mActor);

    if (!_10) {
        MR::invalidateClipping(mActor);
    }

    MR::startActorCameraNoTarget(mActor, mCameraInfo, -1);
    if (MR::Demo::isExistCameraShaking(mDemoName)) {
        f32 shakeSpeed = MR::Demo::getCameraShakeSpeed(mDemoName);
        f32 shakeIntensity = MR::Demo::getCameraShakeIntensity(mDemoName);
        MR::shakeCameraInfinity(mActor, shakeIntensity, shakeSpeed);
    }

    MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
    MR::emitEffect(mActor, MR::Demo::getStartEffect(mDemoName));
    return true;
}
