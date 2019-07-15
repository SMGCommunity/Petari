#ifndef ALLLIVEACTORGROUP_H
#define ALLLIVEACTORGROUP_H

#include "Actor/LiveActor/LiveActorGroup.h"

class AllLiveActorGroup : public LiveActorGroup
{
public:
    AllLiveActorGroup();

    virtual ~AllLiveActorGroup();

    void initActorSystemInfo(const JMapInfoIter &);

    u32 _18;
};

#endif // ALLLIVEACTORGROUP_H