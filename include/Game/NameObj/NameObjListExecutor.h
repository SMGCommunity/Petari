#pragma once

#include "Game/NameObj/NameObjCategoryList.h"
#include "Game/System/DrawBufferHolder.h"

class LiveActor;

class NameObjListExecutor {
public:
    NameObjListExecutor();

    virtual ~NameObjListExecutor();
    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();

    void init();
    void registerDrawBuffer(LiveActor *, int);
    void allocateDrawBufferActorList();
    void registerPreDrawFunction(const MR::FunctorBase &, int);
    // findLightInfo
    void incrementCheckMovement(NameObj *, int);
    void incrementCheckCalcAnim(NameObj *, int);
    void incrementCheckDraw(NameObj *, int);

    DrawBufferHolder* mBufferHolder;    // _4
    NameObjCategoryList* mMovementList; // _8
    NameObjCategoryList* mCalcAnimList; // _C
    NameObjCategoryList* mDrawList;     // _10
};