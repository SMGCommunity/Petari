#include "Actor/NameObj/NameObjListExecutor.h"

void NameObjListExecutor::init()
{
    initMovementList();
    initCalcAnimList();
    initCalcViewAndEntryList();
    initDrawList();
}

void NameObjListExecutor::registerDrawBuffer(LiveActor *pActor, s32 idx)
{
    mBufferHolder->registerDrawBuffer(pActor, idx);
}

void NameObjListExecutor::allocateDrawBufferActorList()
{
    mMovementList->allocateBuffer();
    mCalcAnimList->allocateBuffer();
    mDrawList->allocateBuffer();
    mBufferHolder->allocateActorListBuffer();
}