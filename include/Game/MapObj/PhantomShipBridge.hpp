#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PhantomShipBridge : public LiveActor {
public:
    PhantomShipBridge(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();

    void startMoveA();
    void startMoveB();
    void setStateMoveA();
    void exeMoveA();
    void exeMoveB();
    void exeWait();

    /* 0x8C */ CollisionParts* _8C;
    /* 0x90 */ s32 mIsNutShipBridge;
};
