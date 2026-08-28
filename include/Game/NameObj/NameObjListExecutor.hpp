#pragma once

#include "Game/NameObj/NameObjCategoryList.hpp"
#include "Game/System/DrawBufferHolder.hpp"

class LiveActor;

/// @brief Class that executes NameObjCategoryList instances.
class NameObjListExecutor {
public:
    NameObjListExecutor();

    /* 0x08 */ virtual ~NameObjListExecutor();
    /* 0x0C */ virtual void initMovementList() {
    }
    /* 0x10 */ virtual void initCalcAnimList() {
    }
    /* 0x14 */ virtual void initCalcViewAndEntryList() {
    }
    /* 0x18 */ virtual void initDrawList() {
    }

    void init();
    s32 registerDrawBuffer(LiveActor*, int);
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
