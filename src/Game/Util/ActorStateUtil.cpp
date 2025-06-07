#include "Game/Util/ActorStateUtil.hpp"
#include "Game/LiveActor/ActorStateKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"

void MR::initActorStateKeeper(NerveExecutor* nerveExecutor, s32 capacity) {
    nerveExecutor->mSpine->initStateKeeper(capacity);
}

void MR::initActorState(
        NerveExecutor* nerveExecutor, 
        ActorStateBaseInterface* actorStateBaseInterface, 
        const Nerve* nerve, 
        const char* stateName) {
    actorStateBaseInterface->init();
    ActorStateKeeper* stateKeeper = nerveExecutor->mSpine->mStateKeeper; 
    stateKeeper->addState(actorStateBaseInterface, nerve, stateName);
}

bool MR::updateActorState(LiveActor* liveActor, ActorStateBaseInterface* actorStateBaseInterface) {
    if (isFirstStep(liveActor)) {
        actorStateBaseInterface->appear(); 
    }
    
    if (actorStateBaseInterface->update()) { 
        
        if (!actorStateBaseInterface->mIsDead) { 
            actorStateBaseInterface->kill(); 
        }
        
        return true; 
    }
    
    return false; 
}

bool MR::updateActorState(NerveExecutor* nerveExecutor) {
    return nerveExecutor->mSpine->mStateKeeper->updateCurrentState();
}

bool MR::updateActorState(NerveExecutor* nerveExecutor, ActorStateBaseInterface* actorStateBaseInterface) {
    if (isFirstStep(nerveExecutor)) {
        actorStateBaseInterface->appear(); 
    }
    
    if (actorStateBaseInterface->update()) { 
        
        if (!actorStateBaseInterface->mIsDead) { 
            actorStateBaseInterface->kill(); 
        }
        
        return true; 
    }
    
    return false; 
}

bool MR::updateActorStateAndNextNerve(
        LiveActor* liveActor, 
        ActorStateBaseInterface* actorStateBaseInterface, 
        const Nerve* nextNerve) {
    
    bool stateUpdated = updateActorState(liveActor, actorStateBaseInterface);
    
    if (stateUpdated) {
        liveActor->setNerve(nextNerve);
        return true;
    }
    
    return false;
}

bool MR::updateActorStateAndNextNerve(
        NerveExecutor* nerveExecutor, 
        ActorStateBaseInterface* actorStateBaseInterface, 
        const Nerve* nextNerve) {

    bool stateUpdated = updateActorState(nerveExecutor, actorStateBaseInterface);
    
    if (stateUpdated) {
        nerveExecutor->setNerve(nextNerve);
        return true;
    }
    
    return false;
}
bool MR::updateActorStateAndNextNerve(NerveExecutor* nerveExecutor, const Nerve* nextNerve) {
    bool stateUpdated = nerveExecutor->mSpine->mStateKeeper->updateCurrentState();
    
    if (stateUpdated) {
        nerveExecutor->setNerve(nextNerve);
        return true;
    }
    
    return false;
}
