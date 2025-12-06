#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PhantomShipBridge : public LiveActor {
public:
    PhantomShipBridge(const char*);

    virtual ~PhantomShipBridge();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();

    void startMoveA();
    void startMoveB();
    void setStateMoveA();
    void exeMoveA();
    void exeMoveB();
    void exeWait();

    CollisionParts* _8C;
    s32 mIsNutShipBridge;  // 0x90
};
