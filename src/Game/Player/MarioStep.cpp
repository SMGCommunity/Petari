#include "Game/Player/MarioStep.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioStep::MarioStep(MarioActor* pActor) : MarioState(pActor, 0x10) {
    _14 = 0.0f;
    _18 = 0.0f;
}

bool MarioStep::start() {
    _18 = getPlayer()->_278;
    f32 v2 = (16.0f * _18);
    u16 v3 = 20 - v2;

    if (v3 < 4) {
        v3 = 4;
    }

    mActor->setBlendMtxTimer(v3);
    _14 = v3;

    if (isAnimationRun(nullptr) && !mActor->_3E5) {
        stopAnimation(nullptr, (const char*)nullptr);
    }

    return true;
}

bool MarioStep::update() {
    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    } else if (getPlayer()->mMovementStates.jumping) {
        return false;
    }

    getPlayer()->mainMove();
    clearVelocity();

    f32 _278 = getPlayer()->_278;
    if (_278 < 0.1f) {
        _278 = 0.1f;
    }

    getPlayer()->_278 = _278;
    getPlayer()->updateWalkSpeed();

    if (_278 < 0.5f) {
        _278 = 0.5f;
    }

    f32 v7 = _14 - _278;
    _14 -= _278;

    if (v7 < 0.0f) {
        return false;
    } else {
        mActor->setBlendMtxTimer(v7);
        return true;
    }
}

bool MarioStep::close() {
    return true;
}
