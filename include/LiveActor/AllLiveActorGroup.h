#pragma once

#include "LiveActor/LiveActorGroup.h"

class AllLiveActorGroup : public LiveActorGroup
{
public:
    AllLiveActorGroup();

    virtual ~AllLiveActorGroup();

    void initActorSystemInfo(const JMapInfoIter &);

    u32 _18;
};

namespace MR
{
    AllLiveActorGroup* getAllLiveActorGroup();
};