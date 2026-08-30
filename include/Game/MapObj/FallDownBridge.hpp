#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FallDownBridge : public LiveActor {
public:
    FallDownBridge(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeQuake();

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ f32 _98;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ f32 _A4;
    /* 0xA8 */ bool _A8;
};
