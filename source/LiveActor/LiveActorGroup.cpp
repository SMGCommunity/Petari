#include "LiveActor/LiveActorGroup.h"

LiveActorGroup::LiveActorGroup(const char *pName, s32 numObjs) : NameObjGroup(pName, numObjs) { }

void LiveActorGroup::registerActor(LiveActor *pActor)
{
    registerObj(pActor);
}

LiveActor* LiveActorGroup::getActor(s32 idx) const
{
    return (LiveActor*)mObjs[idx];
}

LiveActor* LiveActorGroup::getDeadActor() const
{
    u32 curObjIdx = 0;
    LiveActor *pDeadActors;

    while(true)
    {
        if (curObjIdx < mNumObjs)
        {
            if (!MR::isDead((LiveActor*)mObjs[curObjIdx]))
            {
                curObjIdx++;
            }
            else
            {
                pDeadActors = (LiveActor*)mObjs[curObjIdx];
                break;
            }
        }
        else
        {
            pDeadActors = 0;
            break;
        }
    }

    return pDeadActors;
}

s32 LiveActorGroup::getLivingActorNum() const
{
    s32 actorNum = 0;
    u32 curObjIdx = 0;

    while(curObjIdx < mNumObjs)
    {
        if (MR::isDead((LiveActor*)mObjs[curObjIdx]) == 0)
        {
            actorNum++;
        }

        curObjIdx++;
    }

    return actorNum;
}

void LiveActorGroup::appearAll()
{
    u32 curObjIdx = 0;

    while(curObjIdx < mNumObjs)
    {
        if (MR::isDead((LiveActor*)mObjs[curObjIdx]) != 0)
        {
            LiveActor *pActor = (LiveActor*)mObjs[curObjIdx];
            pActor->appear();
        }

        curObjIdx++;
    }
}

void LiveActorGroup::killAll()
{
    u32 curObjIdx = 0;

    while(curObjIdx < mNumObjs)
    {
        LiveActor *pActor = (LiveActor*)mObjs[curObjIdx];
        pActor->makeActorDead();

        curObjIdx++;
    }
}