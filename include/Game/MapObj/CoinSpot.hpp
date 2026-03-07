#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoinSpot : public LiveActor {
public:
    CoinSpot(const char*);

    virtual ~CoinSpot();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initSensor();
    void exeWait();
    void exeIsInTornado();

    s32 _8C;
    u8 _90;
    s32 _94;
    s32 _98;
};
