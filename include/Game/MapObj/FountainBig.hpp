#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FountainBig : public LiveActor {
public:
    FountainBig(const char*);

    virtual ~FountainBig();
    virtual void init(const JMapInfoIter&);
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeSign();
    void exeSignStop();
    void exeSpout();
    void exeSpoutEnd();

    /* 0x8C */ TVec3f mClippingRadius;
    /* 0x98 */ s32 mSpoutTimer;
};
