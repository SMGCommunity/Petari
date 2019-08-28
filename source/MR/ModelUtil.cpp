#include "MR/ModelUtil.h"

namespace MR
{
    void updateModelManager(LiveActor *actor)
    {
        actor->mModelManager->update();
    }

    void calcAnimModelManager(LiveActor *actor)
    {
        actor->mModelManager->calcAnim();
    }

    J3DModel* getJ3DModel(const LiveActor *actor)
    {
        if (actor->mModelManager == 0)
        {
            return 0;
        }

        return actor->mModelManager->getJ3DModel();
    }

    J3DModelData* getJ3DModelData(const LiveActor* actor)
    {
        if (actor->mModelManager == 0)
        {
            return 0;
        }

        return actor->mModelManager->getJ3DModelData();
    }

    ResourceHolder* getResourceHolder(const LiveActor *actor)
    {
        if (actor->mModelManager != 0)
        {
            return actor->mModelManager->getResourceHolder();
        }

        return 0;
    }

    ResourceHolder* getModelResourceHolder(const LiveActor *actor)
    {
        if (actor->mModelManager != 0)
        {
            return actor->mModelManager->getModelResourceHolder();
        }

        return 0;
    }

    const char* getModelResName(const LiveActor *actor)
    {
        ResourceHolder* holder = MR::getModelResourceHolder(actor);
        return holder->getResName(0);
    }
};