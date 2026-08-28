#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoinBox : public LiveActor {
public:
    CoinBox(const char*);

    /* 0x08 */ virtual ~CoinBox();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeHit();
};
