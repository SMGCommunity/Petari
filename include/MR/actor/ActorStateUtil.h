#pragma once

#include <revolution.h>
#include "Actor/Nerve/NerveExecutor.h"
#include "Actor/State/ActorStateKeeper.h"

class LiveActor;

namespace MR
{
    void initActorStateKeeper(NerveExecutor *, s32);
    void initActorState(NerveExecutor *, ActorStateBaseInterface *, const Nerve *, const char *);
    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
    bool updateActorStateAndNextNerve(LiveActor *, ActorStateBaseInterface *, const Nerve *);
    bool updateActorState(NerveExecutor *);
};