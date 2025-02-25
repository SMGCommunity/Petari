#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/Coin.hpp"

class TripodBossCoin : public NameObj {
public:
    TripodBossCoin(const char *);

    virtual ~TripodBossCoin();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    void exeNonActive();
    void exeActive();
    void exeEnd() { }

    Coin* mCoin;        // 0xC
    Spine* mSpine;      // 0x10
    TPos3f _14;
    s32 _44;
};
