#pragma once

#include "Actor/LiveActor/LiveActor.h"
#include "revolution/os/OSMutex.h"

namespace MR
{
    void updateModelManager(LiveActor *);
    void calcAnimModelManager(LiveActor *);
    J3DModel* getJ3DModel(const LiveActor *);
    void calcJ3DModel(LiveActor *);
    s16 getBckFrameMax(const LiveActor *, const char *);

    ResourceHolder* getResourceHolder(const LiveActor *);
    ResourceHolder* getModelResourceHolder(const LiveActor *);

    const char* getModelResName(const LiveActor *);

    void syncJointAnimation(LiveActor *, const LiveActor *);

    void syncMaterialAnimation(LiveActor *, const LiveActor *);

    template<int T>
    class MutexHolder
    {
    public:
        static OSMutex sMutex;
    };
};