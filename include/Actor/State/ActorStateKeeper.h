#ifndef ACTORSTATEKEEPER_H
#define ACTORSTATEKEEPER_H

#include "Actor/Nerve/Nerve.h"
#include "Actor/State/ActorStateBaseInterface.h"

class ActorStateKeeper
{
public:
    ActorStateKeeper(s32);

    void addState(ActorStateBaseInterface *, const Nerve *, const char *);
    bool updateCurrentState();
    void startState(const Nerve *);
    void endState(const Nerve *);

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
};

#endif // ACTORSTATEKEEPER_H