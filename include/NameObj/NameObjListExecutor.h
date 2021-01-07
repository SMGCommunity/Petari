#pragma once

#include "NameObj/NameObjCategoryList.h"
#include "System/DrawBuffer/DrawBufferHolder.h"

class LiveActor;

class NameObjListExecutor
{
public:
    NameObjListExecutor();

    virtual ~NameObjListExecutor();
    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();

    void init();
    void registerDrawBuffer(LiveActor *, s32);
    void allocateDrawBufferActorList();

    DrawBufferHolder* mBufferHolder; // _4
    NameObjCategoryList* mMovementList; // _8
    NameObjCategoryList* mCalcAnimList; //_C
    NameObjCategoryList* mDrawList; // _10
};