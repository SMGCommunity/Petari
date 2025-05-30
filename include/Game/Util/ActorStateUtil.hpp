#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class Nerve;

namespace MR {
    void initActorState(NerveExecutor *, ActorStateBaseInterface *, const Nerve *, const char *);
    void initActorStateKeeper(NerveExecutor *, s32);

    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
    bool updateActorState(NerveExecutor *);
    bool updateActorState(NerveExecutor *, ActorStateBaseInterface *);
    bool updateActorStateAndNextNerve(LiveActor *, ActorStateBaseInterface *, const Nerve *);
    bool updateActorStateAndNextNerve(NerveExecutor *, const Nerve *);
    bool updateActorStateAndNextNerve(NerveExecutor *, ActorStateBaseInterface *, const Nerve *);  
};
