#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class TreasureSpot : public MapObjActor {
public:
    TreasureSpot(const char*);

    virtual ~TreasureSpot();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeEnd();
    void exeSpout();
    void switchEmitGlow();

    bool mIsCoinFlower;  // 0xC4
};
