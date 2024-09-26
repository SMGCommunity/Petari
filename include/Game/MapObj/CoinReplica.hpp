#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/Coin.hpp"

class CoinReplica : public NameObj {
public:
    CoinReplica(const char *);

    virtual ~CoinReplica();
    virtual void init(const JMapInfoIter &);

    void activeCoin();
    void activeCoinWithGravity();
    void deactiveCoin();
    void removeCoin();

    Coin* mCoin;    // 0xC
};