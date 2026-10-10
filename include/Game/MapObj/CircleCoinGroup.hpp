#pragma once

#include "Game/MapObj/CoinGroup.hpp"

class CircleCoinGroup : public CoinGroup {
public:
    CircleCoinGroup(const char*);

    virtual ~CircleCoinGroup();
    virtual void initCoinArray(const JMapInfoIter&);
    virtual void placementCoin();
    virtual const char* getCoinName() const;

    /* 0xA0 */ f32 mCoinRadius;
};

namespace MR {
    NameObj* createCircleCoinGroup(const char*);
    NameObj* createCirclePurpleCoinGroup(const char*);
};  // namespace MR
