#include "Game/Util/MultiEventCamera.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

MultiEventCamera::MultiEventCamera() : mName(), mCameraNum(), _8(-1), _C(), _10(), mCameraType(), mCameraInfo(), mTarget() {
}

bool MultiEventCamera::isEnd() const {
    return _8 < 0;
}

void MultiEventCamera::setUp(const char* pName, const ActorCameraInfo* pCameraInfo, s32 cameraNum) {
    mName = pName;
    mCameraInfo = pCameraInfo;
    mCameraNum = cameraNum;

    for (s32 i = 0; i < mCameraNum; i++) {
        declareEventCamera(i);
    }
}

void MultiEventCamera::setEndCameraTypeSoon() {
    mCameraType = CameraType_Soon;
}

void MultiEventCamera::setEndCameraTypeAtLanding() {
    mCameraType = CameraType_AtLanding;
}

void MultiEventCamera::start(s32 param1) {
    CameraTargetArg target = CameraTargetArg();

    start(target, param1);
}

void MultiEventCamera::start(const CameraTargetArg& rParam1, s32 param2) {
    if (mCameraNum <= 0) {
        return;
    }

    _C = 0;
    _8 = param2;
    mTarget = rParam1;

    startCamera();
}

void MultiEventCamera::update() {
    if (isEnd()) {
        return;
    }

    if (_10 <= _C && _8 < mCameraNum - 1) {
        _C = 0;
        _8++;

        startCamera();
    }

    _C++;
}

void MultiEventCamera::nextForce() {
    if (isEnd()) {
        return;
    }

    if (_8 < mCameraNum - 1) {
        _C = 0;
        _8++;

        startCamera();
    } else {
        endCamera(mCameraType);
    }
}

void MultiEventCamera::endForce() {
    if (isEnd()) {
        return;
    }

    endCamera(mCameraType);
}

void MultiEventCamera::endForceSoon() {
    if (isEnd()) {
        return;
    }

    endCamera(CameraType_Soon);
}

void MultiEventCamera::endForceAtLanding() {
    if (isEnd()) {
        return;
    }

    endCamera(CameraType_AtLanding);
}

void MultiEventCamera::changeTargetPlayer() {
    CameraTargetArg target = CameraTargetArg();

    MR::setCameraTargetToPlayer(&target);
    changeTarget(target);
}

bool MultiEventCamera::isActive(s32 param1) const {
    char name[256];
    snprintf(name, sizeof(name), "%s:%03d:%02d番目", mName, mCameraInfo->mCameraSetID, param1);

    return MR::isEventCameraActive(mCameraInfo, name);
}

void MultiEventCamera::declareEventCamera(s32 param1) {
    char name[256];
    snprintf(name, sizeof(name), "%s:%03d:%02d番目", mName, mCameraInfo->mCameraSetID, param1);

    return MR::declareEventCamera(mCameraInfo, name);
}

void MultiEventCamera::startCamera() {
    char name[256];
    snprintf(name, sizeof(name), "%s:%03d:%02d番目", mName, mCameraInfo->mCameraSetID, _8);

    MR::startEventCamera(mCameraInfo, name, mTarget, -1);

    _10 = MR::getEventCameraFrames(mCameraInfo, name);
}

void MultiEventCamera::endCamera(s32 type) {
    char name[256];
    snprintf(name, sizeof(name), "%s:%03d:%02d番目", mName, mCameraInfo->mCameraSetID, _8);

    switch (type) {
    case CameraType_Soon:
        MR::endEventCamera(mCameraInfo, name, true, -1);
        break;
    case CameraType_AtLanding:
        MR::endEventCameraAtLanding(mCameraInfo, name, -1);
        break;
    }

    _8 = -1;
}

void MultiEventCamera::changeTarget(const CameraTargetArg& rTarget) {
    char name[256];
    snprintf(name, sizeof(name), "%s:%03d:%02d番目", mName, mCameraInfo->mCameraSetID, _8);

    MR::changeEventCameraTarget(mCameraInfo, name, rTarget);
}
