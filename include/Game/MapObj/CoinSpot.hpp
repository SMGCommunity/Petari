#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoinSpot : public LiveActor {
public:
    CoinSpot(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initSensor();
    void exeWait();
    void exeIsInTornado();

    /* 0x8C */ s32 _8C;
    /* 0x8C */ bool mIsInTornado;
    /* 0x8C */ s32 _94;
    /* 0x8C */ s32 _98;
};
