#pragma once

#include "Game/MapObj/CoinGroup.hpp"

class RailCoin : public CoinGroup {
public:
    RailCoin(const char*);

    /* 0x08 */ virtual ~RailCoin();
    /* 0x78 */ virtual void initCoinArray(const JMapInfoIter&);
    /* 0x7C */ virtual void placementCoin();
    /* 0x80 */ virtual const char* getCoinName() const;

    void placementNormalRail();
    void placementMercatorRail();
};

namespace MR {
    NameObj* createRailCoin(const char*);
    NameObj* createRailPurpleCoin(const char*);
};  // namespace MR
