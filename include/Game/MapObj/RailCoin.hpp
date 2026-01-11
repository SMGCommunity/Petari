#pragma once

#include "Game/MapObj/CoinGroup.hpp"

class RailCoin : public CoinGroup {
public:
    RailCoin(const char*);

    virtual ~RailCoin();
    virtual void initCoinArray(const JMapInfoIter&);
    virtual void placementCoin();
    virtual const char* getCoinName() const;

    void placementNormalRail();
    void placementMercatorRail();
};

namespace MR {
    NameObj* createRailCoin(const char*);
    NameObj* createRailPurpleCoin(const char*);
};  // namespace MR
