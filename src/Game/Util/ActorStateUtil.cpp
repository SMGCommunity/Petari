#include "Game/LiveActor/ActorStateKeeper.hpp"
#include "Game/Util/ActorStateUtil.hpp"

namespace MR {
    // Fails only because mSpine is private - does it really need to be private????
    /* void initActorState(NerveExecutor *pExecutor, ActorStateBaseInterface *pStateBase, const Nerve *pNerve, const char *pName) {
        pStateBase->init();
        pExecutor->mSpine->mStateKeeper->addState(pStateBase, pNerve, pName);
    }

    void initActorStateKeeper(NerveExecutor *pExecutor, s32 a4) {
        pExecutor->mSpine->initStateKeeper(a4);
    }
        
    bool updateActorState(NerveExecutor *pExecutor) {
        return pExecutor->mSpine->mStateKeeper->updateCurrentState();
    }

    bool updateActorStateAndNextNerve(NerveExecutor *pExecutor, const Nerve *pNerve) {
        if (pExecutor->mSpine->mStateKeeper->updateCurrentState()) {
            pExecutor->setNerve(pNerve);
            return true;
        }
        return false;
    }
    */

    bool updateActorState(LiveActor *pActor, ActorStateBaseInterface *pStateBase) {
        if (isFirstStep(pActor)) {
            pStateBase->appear();
        }
        if (pStateBase->update()) {
            if (pStateBase->mIsDead == false) {
                pStateBase->kill();
            }
            return true;
        }
        return false;
    }

    

    bool updateActorState(NerveExecutor *pExecutor, ActorStateBaseInterface *pStateBase) {
        if (isFirstStep(pExecutor)) {
            pStateBase->appear();
        }
        if (pStateBase->update()) {
            if (pStateBase->mIsDead == false) {
                pStateBase->kill();
            }
            return true;
        }
        return false;
    }
    
    bool updateActorStateAndNextNerve(LiveActor *pActor, ActorStateBaseInterface *pStateBase, const Nerve *pNerve) {
        if (updateActorState(pActor, pStateBase)) {
            pActor->setNerve(pNerve);
            return true;
        }
        return false;
    }

    bool updateActorStateAndNextNerve(NerveExecutor *pExecutor, ActorStateBaseInterface *pStateBase, const Nerve *pNerve) {
        if (updateActorState(pExecutor, pStateBase)) {
            pExecutor->setNerve(pNerve);
            return true;
        }
        return false;
    }
};
