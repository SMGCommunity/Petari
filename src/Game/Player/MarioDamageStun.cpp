#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioStun.hpp"

MarioStun::MarioStun(MarioActor* pActor) : MarioState(pActor, 0xe), _12(0), _14(0) {}

bool MarioStun::close() {
    stopAnimation("しびれ", static_cast< const char* >(nullptr));  // "hesitation"
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
            changeAnimation("しびれ回復", static_cast< const char* >(nullptr));
        }
    }
    if (_12 != 0 && (mActor->isRequestRush() || checkTrgA())) {
        stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
        if (checkTrgA()) {
            getPlayer()->tryJump();
        }
        return false;
    }
    return true;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
