#include "MR/actor/ActorStateUtil.h"
#include "Actor/State/ActorStateKeeper.h"

namespace MR
{
    void initActorStateKeeper(NerveExecutor *pExecutor, s32 stateCount)
    {
        pExecutor->mSpine->initStateKeeper(stateCount);
    }

    bool updateActorState(NerveExecutor *pExecutor)
    {
        return pExecutor->mSpine->mStateKeeper->updateCurrentState();
    }
};