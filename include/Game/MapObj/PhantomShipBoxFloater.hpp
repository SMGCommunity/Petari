#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FloaterFloatingForceTypeSpring;

class PhantomShipBoxFloater : public LiveActor {
public:
    PhantomShipBoxFloater(const char*);

    virtual ~PhantomShipBoxFloater();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    f32 getCurrentSinkDepth() const;

    void exeWait();
    void exeMove();
    void exeMoveUnderWater();
    void playRippleSe();

    /* 0x8C */ FloaterFloatingForceTypeSpring* mFloatingForce;
    /* 0x90 */ const char* mObjectName;
    /* 0x94 */ TVec3f mFloaterOffset;
    /* 0xA0 */ s32 mRippleSeCounter;
};
