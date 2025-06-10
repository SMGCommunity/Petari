#include "Game/Util/ActorStateUtil.hpp"
#include "Game/LiveActor/ActorStateKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"

void MR::initActorStateKeeper(NerveExecutor* pNerveExecutor, long capacity) {
    pNerveExecutor->mSpine->initStateKeeper(capacity);
}

void MR::initActorState(
        NerveExecutor* pNerveExecutor, 
        ActorStateBaseInterface* pActorState, 
        const Nerve* pNerve, 
        const char* pStateName) 
{
    pActorState->init();
    ActorStateKeeper* stateKeeper = pNerveExecutor->mSpine->mStateKeeper; 
    stateKeeper->addState(pActorState, pNerve, pStateName);
}

bool MR::updateActorState(LiveActor* pActor, ActorStateBaseInterface* pActorState) {
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

bool MR::updateActorState(NerveExecutor* pExecutor) {
    return pExecutor->mSpine->mStateKeeper->updateCurrentState();
}

bool MR::updateActorState(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState) {
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

bool MR::updateActorStateAndNextNerve(
        LiveActor* pActor, 
        ActorStateBaseInterface* pActorState, 
        const Nerve* pNextNerve) {
    
    bool stateUpdated = updateActorState(pActor, pActorState);
    
    if (stateUpdated) {
        pActor->setNerve(pNextNerve);
        return true;
    }
    
    return false;
}

bool MR::updateActorStateAndNextNerve(
        NerveExecutor* pNerveExecutor, 
        ActorStateBaseInterface* pActorState, 
        const Nerve* pNextNerve) {

    bool stateUpdated = updateActorState(pNerveExecutor, pActorState);
    
    if (stateUpdated) {
        pNerveExecutor->setNerve(pNextNerve);
        return true;
    }
    
    return false;
}
bool MR::updateActorStateAndNextNerve(NerveExecutor* pExecutor, const Nerve* pNextNerve) {
    bool stateUpdated = pExecutor->mSpine->mStateKeeper->updateCurrentState();
    
    if (stateUpdated) {
        pExecutor->setNerve(pNextNerve);
        return true;
    }
    
    return false;
}
