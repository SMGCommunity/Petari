#pragma once

#include "Game/NameObj/NameObjCategoryList.hpp"
#include "Game/System/DrawBufferHolder.hpp"

class LiveActor;

/// @brief Class that executes NameObjCategoryList instances.
class NameObjListExecutor {
public:
    NameObjListExecutor();

    virtual ~NameObjListExecutor();
    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();

    void init();
    s32 registerDrawBuffer(LiveActor* pActor, int drawBufferType);
    void allocateDrawBufferActorList();
    void registerPreDrawFunction(const MR::FunctorBase& rFunc, int drawType);
    void findLightInfo(LiveActor* pActor, int drawBufferType, int executorIndex) const;
    void incrementCheckMovement(NameObj* pObj, int category);
    void incrementCheckCalcAnim(NameObj* pObj, int category);
    void incrementCheckDraw(NameObj* pObj, int category);
    void addToMovement(NameObj* pObj, int category);
    void addToCalcAnim(NameObj* pObj, int category);
    void addToDrawBuffer(LiveActor* pActor, int drawBufferType, int executorIndex);
    void addToDraw(NameObj* pObj, int category);
    void removeToMovement(NameObj* pObj, int category);
    void removeToCalcAnim(NameObj* pObj, int category);
    void removeToDrawBuffer(LiveActor* pActor, int drawBufferType, int executorIndex);
    void removeToDraw(NameObj* pObj, int category);
    void executeMovement(int category);
    void executeCalcAnim(int category);
    void entryDrawBuffer2D();
    void entryDrawBuffer3D();
    void entryDrawBufferMirror();
    void drawOpa(int drawBufferType);
    void drawXlu(int drawBufferType);
    void executeDraw(int category);

    /* 0x04 */ DrawBufferHolder* mBufferHolder;
    /* 0x08 */ NameObjCategoryList* mMovementList;
    /* 0x0C */ NameObjCategoryList* mCalcAnimList;
    /* 0x10 */ NameObjCategoryList* mDrawList;
};
