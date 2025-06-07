#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class Nerve;

namespace MR {
    
    void initActorStateKeeper(NerveExecutor* nerveExecutor, s32 p1);
    void initActorState(NerveExecutor* nerveExecutor, ActorStateBaseInterface* actorStateBaseInterface, const Nerve* nerve, const char* a1);

    bool updateActorStateAndNextNerve(LiveActor *, ActorStateBaseInterface *, const Nerve *);
    bool updateActorStateAndNextNerve(NerveExecutor *, ActorStateBaseInterface *, const Nerve *);
    bool updateActorStateAndNextNerve(NerveExecutor* nerveExecutor, const Nerve* nextNerve);

    bool updateActorState(NerveExecutor *, ActorStateBaseInterface *);
    bool updateActorState(NerveExecutor* nerveExecutor);
    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
};
