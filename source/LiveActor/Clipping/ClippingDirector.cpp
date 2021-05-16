#include "LiveActor/Clipping/ClippingDirector.h"
#include "Util/ObjUtil.h"
#include "Util/JMapUtil.h"

ClippingDirector::ClippingDirector() : NameObj("クリッピング指揮") 
{
    mJudge = 0;
    mActorHolder = 0;
    mGroupHolder = 0;

    ClippingJudge* pJudge = new ClippingJudge("クリッピング判定者");
    mJudge = pJudge;
    pJudge->initWithoutIter();

    ClippingActorHolder* pActorHolder = new ClippingActorHolder();
    mActorHolder = pActorHolder;
    pActorHolder->initWithoutIter();

    ClippingGroupHolder* pGroupHolder = new ClippingGroupHolder();
    mGroupHolder = pGroupHolder;
    pGroupHolder->initWithoutIter();

    MR::connectToScene(this, 4, -1, -1, -1);
}

void ClippingDirector::endInitActorSystemInfo() 
{
    mActorHolder->initViewGroupTable();
}

void ClippingDirector::movement() 
{
    mJudge->movement();
    mActorHolder->movement();
    mGroupHolder->movement();
}

void ClippingDirector::registerActor(LiveActor *pActor) 
{
    mActorHolder->registerActor(pActor);
}

void ClippingDirector::initActorSystemInfo(LiveActor *pActor, const JMapInfoIter &iter) 
{
    mActorHolder->initSystemInfo(pActor, iter);
}

void ClippingDirector::joinToGroupClipping(LiveActor *pActor, const JMapInfoIter &iter, s32 unk1) 
{
    s32 groupID = -1;

    MR::getJMapInfoClippingGroupID(iter, &groupID);

    // for some reason this assembles to some weird algorithm
    if (groupID < 0) 
    {
        return;
    }

    ClippingActorInfo* pInfo = mActorHolder->startGroupClipping(pActor, iter);
    mGroupHolder->createAndAdd(pInfo, iter, unk1);
}

void ClippingDirector::entryLodCtrl(LodCtrl *pLodCtrl, const JMapInfoIter &iter) 
{
    mActorHolder->entryLodCtrl(pLodCtrl, iter);
}