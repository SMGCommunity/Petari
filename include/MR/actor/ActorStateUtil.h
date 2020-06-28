#ifndef ACTORSTATEUTIL_H
#define ACTORSTATEUTIL_H

#include <revolution.h>
#include "Actor/Nerve/NerveExecutor.h"

namespace MR
{
    void initActorStateKeeper(NerveExecutor *, s32);

    bool updateActorState(NerveExecutor *);
};

#endif // ACTORSTATEUTIL_H