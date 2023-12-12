#pragma once

#include "Game/LiveActor/LiveActor.h"

class CoinBox : public LiveActor {
public:
    CoinBox(const char *);

    virtual ~CoinBox();
    virtual void init(const JMapInfoIter &);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeHit();
};

namespace NrvCoinBox {
    NERVE(CoinBoxNrvWait);
    NERVE(CoinBoxNrvHit);
};