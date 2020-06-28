#include "MR/actor/ActorStateUtil.h"
#include "MR/actor/LiveActorUtil.h"
#include "Actor/LiveActor/LiveActor.h"

namespace MR
{
    void initActorStateKeeper(NerveExecutor *pExecutor, s32 stateCount)
    {
        pExecutor->mSpine->initStateKeeper(stateCount);
    }

    void initActorState(NerveExecutor *pExecutor, ActorStateBaseInterface *pStateBase, const Nerve *pNerve, const char *pStateName)
    {
        pStateBase->init();
        pExecutor->mSpine->mStateKeeper->addState(pStateBase, pNerve, pStateName);
    }

    bool updateActorState(LiveActor *pActor, ActorStateBaseInterface *pStateBase)
    {
        bool ret;

        if (MR::isFirstStep(pActor))
        {
            pStateBase->appear();
        }

        bool res = pStateBase->update();

        if (res)
        {
            if (!pStateBase->mIsDead)
            {
                pStateBase->kill();
            }

            ret = true;
        }
        else
        {
            ret = false;
        }
        
        return ret;
    }

    bool updateActorStateAndNextNerve(LiveActor *pActor, ActorStateBaseInterface *pStateBase, const Nerve *pNerve)
    {
        bool ret;
        bool needsNerveUpdate = updateActorState(pActor, pStateBase);

        if (needsNerveUpdate)
        {
            pActor->setNerve(pNerve);
            ret = true;
        }
        else
        {
            ret = false;
        }
        
        return ret;
    }

    bool updateActorState(NerveExecutor *pExecutor)
    {
        return pExecutor->mSpine->mStateKeeper->updateCurrentState();
    }
};