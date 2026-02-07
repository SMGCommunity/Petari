#include "Game/Enemy/JumpGuarder.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

JumpEmitter::JumpEmitter(const char* pName) : LiveActor(pName) {
    _8C = 0;
    mHeadModel = nullptr;
    _C4.x = 0.0f;
    _C4.y = 0.0f;
    _C4.z = 0.0f;
    _D0 = 0;
    mCameraInfo = nullptr;
    mMultEventCamera = nullptr;
    _90.identity();
}

void JumpEmitter::kill() {
    LiveActor::kill();
    endEventCamera();
}

void JumpEmitter::initEventCamera(const JMapInfoIter& rIter) {
    mCameraInfo = new ActorCameraInfo(rIter);

    if (mCameraInfo->mCameraSetID != -1) {
        mMultEventCamera = new MultiEventCamera();
        mMultEventCamera->setUp(mName, mCameraInfo, 2);
        mMultEventCamera->setEndCameraTypeSoon();
    }
}

void JumpEmitter::startEventCamera() {
    if (mMultEventCamera != nullptr) {
        mMultEventCamera->start(0);
        _D0 = 1;
        _C4.set< f32 >(mPosition);
    }
}

void JumpEmitter::updateEventCamera() {
    if (_D0) {
        mMultEventCamera->update();

        if (mMultEventCamera->isActive(0) && !MR::isPlayerJumpRising()) {
            mMultEventCamera->nextForce();
        }

        f32 dist = JMathInlineVEC::PSVECSquareDistance(&_C4, MR::getPlayerCenterPos());

        if (6250000.0f < dist || MR::isOnGroundPlayer() || MR::isPlayerInRush()) {
            endEventCamera();
        }
    }
}

void JumpEmitter::endEventCamera() {
    if (mMultEventCamera != nullptr) {
        if (_D0) {
            _D0 = 0;
            mMultEventCamera->endForceSoon();
        }
    }
}

// JumpEmitter::updateRotate
