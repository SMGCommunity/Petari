#pragma once

#include "Game/MapObj/Coin.hpp"
#include "Game/NameObj/NameObj.hpp"

class CoinReplica : public NameObj {
public:
    CoinReplica(const char*);

    virtual ~CoinReplica();
    virtual void init(const JMapInfoIter&);

    void activeCoin();
    void activeCoinWithGravity();
    void deactiveCoin();
    void removeCoin();

    Coin* mCoin;  // 0xC
};