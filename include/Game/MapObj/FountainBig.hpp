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

    TVec3f mClippingRadius;  // 0x8C
    s32 mSpoutTimer;         // 0x98
};
