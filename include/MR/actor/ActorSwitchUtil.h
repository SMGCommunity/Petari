#ifndef ACTORSWITCHUTIL_H
#define ACTORSWITCHUTIL_H

#include "Actor/LiveActor/LiveActor.h"
#include "JMap/JMapInfoIter.h"

namespace MR
{
    bool useStageSwitchReadA(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchReadB(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchReadAppear(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteA(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteB(LiveActor *, const JMapInfoIter &);
    bool useStageSwitchWriteDead(LiveActor *, const JMapInfoIter &);
};

#endif // ACTORSWITCHUTIL_H