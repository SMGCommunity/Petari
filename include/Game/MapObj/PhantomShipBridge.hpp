#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PhantomShipBridge : public LiveActor {
public:
    PhantomShipBridge(const char*);

    /* 0x08 */ virtual ~PhantomShipBridge();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();

    void startMoveA();
    void startMoveB();
    void setStateMoveA();
    void exeMoveA();
    void exeMoveB();
    void exeWait();

    CollisionParts* _8C;
    s32 mIsNutShipBridge;  // 0x90
};
