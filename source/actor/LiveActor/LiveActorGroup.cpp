#include "actor/LiveActor/LiveActorGroup.h"
#include "MR/actor/LiveActorUtil.h"

LiveActorGroup::LiveActorGroup(const char *name, s32 numObjs) : NameObjGroup(name, numObjs) { }

void LiveActorGroup::registerActor(LiveActor *actor)
{
    this->registerObj(actor);
}

LiveActor* LiveActorGroup::getActor(s32 idx) const
{
    return (LiveActor*)this->mObjs[idx];
}

LiveActor* LiveActorGroup::getDeadActor() const
{
    u32 curObjIdx = 0;
    LiveActor* actor;

    while(true)
    {
        if (curObjIdx < this->mNumObjs)
        {
            if (!MR::isDead((LiveActor*)this->mObjs[curObjIdx]))
            {
                curObjIdx++;
            }
            else
            {
                actor = (LiveActor*)this->mObjs[curObjIdx];
                break;
            }
        }
        else
        {
            actor = 0;
            break;
        }
    }

    return actor;
}

s32 LiveActorGroup::getLivingActorNum() const
{
    s32 actorNum = 0;
    u32 curObjIdx = 0;

    while(curObjIdx < this->mNumObjs)
    {
        if (MR::isDead((LiveActor*)this->mObjs[curObjIdx]) == 0)
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

    while(curObjIdx < this->mNumObjs)
    {
        if (MR::isDead((LiveActor*)this->mObjs[curObjIdx]) != 0)
        {
            LiveActor* actor = (LiveActor*)this->mObjs[curObjIdx];
            actor->appear();
        }

        curObjIdx++;
    }
}

void LiveActorGroup::killAll()
{
    u32 curObjIdx = 0;

    while(curObjIdx < this->mNumObjs)
    {
        LiveActor* actor = (LiveActor*)this->mObjs[curObjIdx];
        actor->makeActorDead();

        curObjIdx++;
    }
}