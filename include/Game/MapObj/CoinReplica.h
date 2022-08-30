#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/MapObj/Coin.h"

class CoinReplica : public NameObj {
public:
    CoinReplica(const char *);

    virtual ~CoinReplica();
    virtual void init(const JMapInfoIter &);

    void activeCoin();
    void activeCoinWithGravity();
    void deactiveCoin();
    void removeCoin();

    Coin* mCoin;    // _C
};