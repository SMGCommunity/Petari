#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/ActorStateKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorStateUtil.hpp"

namespace MR {
    void initActorStateKeeper(NerveExecutor* pExecutor, s32 capacity) {
        pExecutor->mSpine->initStateKeeper(capacity);
    }

    void initActorState(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState, const Nerve* pNerve, const char* pStateName) {
        pActorState->init();
        pExecutor->mSpine->mStateKeeper->addState(pActorState, pNerve, pStateName);
    }

    bool updateActorState(LiveActor* pActor, ActorStateBaseInterface* pActorState) {
        if (isFirstStep(pActor)) {
            pActorState->appear();
        }

        if (pActorState->update()) {
            if (!pActorState->mIsDead) {
                pActorState->kill();
            }

            return true;
        }

        return false;
    }

    bool updateActorStateAndNextNerve(LiveActor* pActor, ActorStateBaseInterface* pActorState, const Nerve* pNextNerve) {
        bool isStateUpdated = updateActorState(pActor, pActorState);

        if (isStateUpdated) {
            pActor->setNerve(pNextNerve);
            return true;
        }

        return false;
    }

    bool updateActorState(NerveExecutor* pExecutor) {
        return pExecutor->mSpine->mStateKeeper->updateCurrentState();
    }

    bool updateActorState(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState) {
        if (isFirstStep(pExecutor)) {
            pActorState->appear();
        }

        if (pActorState->update()) {
            if (!pActorState->mIsDead) {
                pActorState->kill();
            }

            return true;
        }

        return false;
    }

    bool updateActorStateAndNextNerve(NerveExecutor* pExecutor, const Nerve* pNextNerve) {
        bool isStateUpdated = updateActorState(pExecutor);

        if (isStateUpdated) {
            pExecutor->setNerve(pNextNerve);
            return true;
        }

        return false;
    }

    bool updateActorStateAndNextNerve(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState, const Nerve* pNextNerve) {
        bool isStateUpdated = updateActorState(pExecutor, pActorState);

        if (isStateUpdated) {
            pExecutor->setNerve(pNextNerve);
            return true;
        }

        return false;
    }
}; // namespace MR
