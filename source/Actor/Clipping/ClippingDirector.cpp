#include "Actor/Clipping/ClippingDirector.h"
#include "MR/ObjUtil.h"
#include "MR/JMap/JMapUtil.h"

ClippingDirector::ClippingDirector() : NameObj("クリッピング指揮")
{
    this->mJudge = 0;
    this->mActorHolder = 0;
    this->mGroupHolder = 0;

    ClippingJudge* judge = new ClippingJudge("クリッピング判定者");
    this->mJudge = judge;
    judge->initWithoutIter();

    ClippingActorHolder* actorHolder = new ClippingActorHolder();
    this->mActorHolder = actorHolder;
    actorHolder->initWithoutIter();

    ClippingGroupHolder* groupHolder = new ClippingGroupHolder();
    this->mGroupHolder = groupHolder;
    groupHolder->initWithoutIter();

    MR::connectToScene(this, 4, -1, -1, -1);
}

void ClippingDirector::endInitActorSystemInfo()
{
    this->mActorHolder->initViewGroupTable();
}

void ClippingDirector::movement()
{
    this->mJudge->movement();
    this->mActorHolder->movement();
    this->mGroupHolder->movement();
}

void ClippingDirector::registerActor(LiveActor *actor)
{
    this->mActorHolder->registerActor(actor);
}

void ClippingDirector::initActorSystemInfo(LiveActor *actor, const JMapInfoIter &iter)
{
    this->mActorHolder->initSystemInfo(actor, iter);
}

void ClippingDirector::joinToGroupClipping(LiveActor *actor, const JMapInfoIter &iter, s32 unk1)
{
    s64 groupID = -1;

    MR::getJMapInfoClippingGroupID(iter, &groupID);

    // for some reason this assembles to some weird algorithm
    if (groupID < 0)
    {
        return;
    }

    ClippingActorInfo* info = this->mActorHolder->startGroupClipping(actor, iter);
    this->mGroupHolder->createAndAdd(info, iter, unk1);
}

void ClippingDirector::entryLodCtrl(LodCtrl* lodCtrl, const JMapInfoIter &iter)
{
    this->mActorHolder->entryLodCtrl(lodCtrl, iter);
}