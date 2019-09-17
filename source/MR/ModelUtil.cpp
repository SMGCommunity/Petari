#include "MR/ModelUtil.h"

namespace MR
{
    void updateModelManager(LiveActor *pActor)
    {
        pActor->mModelManager->update();
    }

    void calcAnimModelManager(LiveActor *pActor)
    {
        pActor->mModelManager->calcAnim();
    }

    J3DModel* getJ3DModel(const LiveActor *pActor)
    {
        if (pActor->mModelManager == 0)
        {
            return 0;
        }

        return pActor->mModelManager->getJ3DModel();
    }

    J3DModelData* getJ3DModelData(const LiveActor *pActor)
    {
        if (pActor->mModelManager == 0)
        {
            return 0;
        }

        return pActor->mModelManager->getJ3DModelData();
    }

    ResourceHolder* getResourceHolder(const LiveActor *pActor)
    {
        if (pActor->mModelManager != 0)
        {
            return pActor->mModelManager->getResourceHolder();
        }

        return 0;
    }

    ResourceHolder* getModelResourceHolder(const LiveActor *pActor)
    {
        if (pActor->mModelManager != 0)
        {
            return pActor->mModelManager->getModelResourceHolder();
        }

        return 0;
    }

    const char* getModelResName(const LiveActor *pActor)
    {
        ResourceHolder* holder = MR::getModelResourceHolder(pActor);
        return holder->getResName(0);
    }
};