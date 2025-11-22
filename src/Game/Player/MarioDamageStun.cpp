#include "Game/Player/MarioStun.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioStun::MarioStun(MarioActor* pActor)
    : MarioState(pActor, 0xe), _12(0), _14(0) {
}

bool MarioStun::close() {
    stopAnimation("しびれ", (char*)nullptr); // "hesitation"
    return true;
}

bool MarioStun::start() {
    changeAnimationNonStop("しびれ");
    startPadVib("マリオ[しびれ]");
    playSound("声しびれ", -1);
    _14 = 0x3c;
    _12 = 0;
    return true;
}

bool MarioStun::update() {
    if (_14 != 0) {
        _14--;
    }
    if (_14 == 0) {
        if (_12 != 0) {
            return false;
        }
        _12 = 1;
        if (!getPlayer()->mMovementStates._1) {
            _14 = 0xa;
        } else {
            _14 = 0x1e;
        }
        if (getPlayer()->mMovementStates._1) {
            changeAnimation("しびれ回復", (char*)nullptr);
        }
    }
    if (_12 != 0 && (mActor->isRequestRush() || checkTrgA())) {
        stopAnimation(nullptr, (char*)nullptr);
        if (checkTrgA()) {
            getPlayer()->tryJump();
        }
        return false;
    }
    return true;
}
