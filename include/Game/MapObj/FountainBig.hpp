#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FountainBig : public LiveActor {
public:
    FountainBig(const char*);

    /* 0x08 */ virtual ~FountainBig();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeSign();
    void exeSignStop();
    void exeSpout();
    void exeSpoutEnd();

    TVec3f mClippingRadius;  // 0x8C
    s32 mSpoutTimer;         // 0x98
};
