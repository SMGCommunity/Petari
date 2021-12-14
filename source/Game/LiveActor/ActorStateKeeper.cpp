#include "Game/LiveActor/ActorStateKeeper.h"
#include "Game/LiveActor/Nerve.h"

bool ActorStateKeeper::updateCurrentState() {
    return (!mCurrentState) ? false : ((ActorStateBaseInterface*)mCurrentState->mExecutor)->update();
}

void ActorStateKeeper::startState(const Nerve *pNerve) {
    mCurrentState = findStateInfo(pNerve);

    if (mCurrentState) {
        ActorStateBaseInterface* interface = (ActorStateBaseInterface*)mCurrentState->mExecutor;
        interface->appear();
    }
}

void ActorStateKeeper::endState(const Nerve *pNerve) {
    mCurrentState = findStateInfo(pNerve);

    if (mCurrentState) {
        ActorStateBaseInterface* interface = (ActorStateBaseInterface*)mCurrentState->mExecutor;
        if (!interface->mIsDead) {
            interface->kill();
        }
    }
}
