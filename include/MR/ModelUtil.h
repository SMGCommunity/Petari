#ifndef MODELUTIL_H
#define MODELUTIL_H

#include "Actor/LiveActor/LiveActor.h"
#include "Model/J3D/J3DModel.h"

namespace MR
{
    void updateModelManager(LiveActor *);
    void calcAnimModelManager(LiveActor *);
    J3DModel* getJ3DModel(const LiveActor *);
    void calcJ3DModel(LiveActor *);
    ResourceHolder* getResourceHolder(const LiveActor *);
    ResourceHolder* getModelResourceHolder(const LiveActor *);

    const char* getModelResName(const LiveActor *);
}

#endif // MODELUTIL_H