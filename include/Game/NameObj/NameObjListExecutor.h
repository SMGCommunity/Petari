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
    s16 registerDrawBuffer(LiveActor *, int);
    void allocateDrawBufferActorList();
    void registerPreDrawFunction(const MR::FunctorBase &, int);
    // findLightInfo
    void incrementCheckMovement(NameObj *, int);
    void incrementCheckCalcAnim(NameObj *, int);
    void incrementCheckDraw(NameObj *, int);

    void addToMovement(NameObj *, int);
    void addToCalcAnim(NameObj *, int);

    void removeToMovement(NameObj *, int);
    void removeToCalcAnim(NameObj *, int);

    void addToDraw(NameObj *, int);
    void addToDrawBuffer(LiveActor *, int, int);

    void removeToDraw(NameObj *, int);
    void removeToDrawBuffer(LiveActor *, int, int);

    DrawBufferHolder* mBufferHolder;    // _4
    NameObjCategoryList* mMovementList; // _8
    NameObjCategoryList* mCalcAnimList; // _C
    NameObjCategoryList* mDrawList;     // _10
};