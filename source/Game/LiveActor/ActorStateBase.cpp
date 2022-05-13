#include "Game/LiveActor/ActorStateBase.h"

bool ActorStateBaseInterface::update() {
    updateNerve();

    if (mIsDead) {
        return true;
    }

    control();
    return false;
}