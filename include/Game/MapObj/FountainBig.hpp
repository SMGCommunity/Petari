#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FountainBig : public LiveActor {
public:
    FountainBig(const char *);

    virtual ~FountainBig();
    virtual void init(const JMapInfoIter &);
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);

    void exeWait();
    void exeSign();
    void exeSignStop();
    void exeSpout();
    void exeSpoutEnd();

    TVec3f mClippingRadius;     // 0x8C
    s32 mSpoutTimer;            // 0x98
};

namespace NrvFountainBig {
    NERVE_DECL(FountainBigNrvWait, FountainBig, FountainBig::exeWait);
    NERVE_DECL(FountainBigNrvSign, FountainBig, FountainBig::exeSign);
    NERVE_DECL(FountainBigNrvSignStop, FountainBig, FountainBig::exeSignStop);
    NERVE_DECL(FountainBigNrvSpout, FountainBig, FountainBig::exeSpout);
    NERVE_DECL(FountainBigNrvSpoutEnd, FountainBig, FountainBig::exeSpoutEnd);
};