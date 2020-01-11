#include "MR/ModelUtil.h"
#include "Animation/BckCtrl.h"

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

    void calcJ3DModel(LiveActor *pActor)
    {
        OSLockMutex(&MR::MutexHolder<0>::sMutex);
        MR::getJ3DModel(pActor)->calc();
        OSUnlockMutex(&MR::MutexHolder<0>::sMutex);
    }

    J3DModelData* getJ3DModelData(const LiveActor *pActor)
    {
        if (pActor->mModelManager == 0)
        {
            return 0;
        }

        return pActor->mModelManager->getJ3DModelData();
    }

    s16 getBckFrameMax(const LiveActor *pActor, const char *pBck)
    {
        BckCtrlData* data = (BckCtrlData*)MR::getResourceHolder(pActor)->_4->getRes(pBck);
        return data->_6;
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