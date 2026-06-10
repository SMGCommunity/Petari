#include "Game/Scene/StopSceneController.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util/ObjUtil.hpp"

StopSceneDelayRequest::StopSceneDelayRequest() : NameObj("シーン硬直遅延発行"), mFrame(), mDelay() {
    MR::connectToScene(this, 1, -1, -1, -1);
}

StopSceneController::StopSceneController() : NameObj("StopSceneController"), mDelayRequestArray(), mFrame() {
    mDelayRequestArray = new NameObjGroup("シーン硬直遅延発行者の管理", 16);

    for (s32 i = 0; i < 16; i++) {
        StopSceneDelayRequest* delayRequest = new StopSceneDelayRequest();
        delayRequest->initWithoutIter();
        mDelayRequestArray->registerObj(delayRequest);
    }
}

void StopSceneDelayRequest::movement() {
    if (mDelay == 0) {
        return;
    }

    mDelay--;

    if (mDelay != 0) {
        return;
    }

    MR::stopScene(mFrame);
}

void StopSceneController::requestStopScene(s32 frame) {
    if (isSceneStopped()) {
        if (mFrame < frame) {
            mFrame = frame;
        }
    } else {
        mFrame = frame;
    }
}

void StopSceneController::requestStopSceneDelay(s32 frame, s32 delay) {
    for (s32 i = 0; i < mDelayRequestArray->mObjectCount; i++) {
        StopSceneDelayRequest* delayRequest = static_cast< StopSceneDelayRequest* >(mDelayRequestArray->mObjects[i]);

        if (delayRequest->mDelay != 0) {
            continue;
        }

        delayRequest->mFrame = frame;
        delayRequest->mDelay = delay;
    }
}

void StopSceneController::movement() {
    if (mFrame > 0) {
        mFrame--;
    }
}

bool StopSceneController::isSceneStopped() const {
    return mFrame > 0;
}
