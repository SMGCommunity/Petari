#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class Nerve;

namespace MR {
    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
    bool updateActorStateAndNextNerve(LiveActor *, ActorStateBaseInterface *, const Nerve *);

    bool updateActorStateAndNextNerve(NerveExecutor *, ActorStateBaseInterface *, const Nerve *);
    bool updateActorState(NerveExecutor *, ActorStateBaseInterface *);
};
