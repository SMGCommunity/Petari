#include "Game/LiveActor/ActorStateBase.hpp"

bool ActorStateBaseInterface::update() {
    updateNerve();

    if (mIsDead) {
        return true;
    }

    control();
    return false;
}