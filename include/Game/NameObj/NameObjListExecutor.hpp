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
    s16 registerDrawBuffer(LiveActor*, int);
    void allocateDrawBufferActorList();
    void registerPreDrawFunction(const MR::FunctorBase&, int drawType);
    void findLightInfo(LiveActor*, int, int) const;
    void incrementCheckMovement(NameObj*, int);
    void incrementCheckCalcAnim(NameObj*, int);
    void incrementCheckDraw(NameObj*, int);
    void addToMovement(NameObj*, int);
    void addToCalcAnim(NameObj*, int);
    void addToDrawBuffer(LiveActor*, int, int);
    void addToDraw(NameObj*, int);
    void removeToMovement(NameObj*, int);
    void removeToCalcAnim(NameObj*, int);
    void removeToDrawBuffer(LiveActor*, int, int);
    void removeToDraw(NameObj*, int);
    void executeMovement(int);
    void executeCalcAnim(int);
    void entryDrawBuffer2D();
    void entryDrawBuffer3D();
    void entryDrawBufferMirror();
    void drawOpa(int);
    void drawXlu(int);
    void executeDraw(int);

    DrawBufferHolder* mBufferHolder;     // 0x4
    NameObjCategoryList* mMovementList;  // 0x8
    NameObjCategoryList* mCalcAnimList;  // 0xC
    NameObjCategoryList* mDrawList;      // 0x10
};
