#pragma once

#include "Game/NameObj/NameObj.hpp"

class Coin;

class CoinReplica : public NameObj {
public:
    CoinReplica(const char*);

    /* 0x08 */ virtual ~CoinReplica();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void activeCoin();
    void activeCoinWithGravity();
    void deactiveCoin();
    void removeCoin();

    Coin* mCoin;  // 0xC
};