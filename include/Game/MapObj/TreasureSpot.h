#pragma once

#include "Game/MapObj/MapObjActor.h"

class TreasureSpot : public MapObjActor {
public:
    TreasureSpot(const char *);

    virtual ~TreasureSpot();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeSpout();
    void switchEmitGlow();

    bool mIsCoinFlower; // _C4
};

namespace NrvTreasureSpot {
    NERVE(TreasureSpotNrvWait);
    NERVE(TreasureSpotNrvEnd);
    NERVE(TreasureSpotNrvSpout);
};