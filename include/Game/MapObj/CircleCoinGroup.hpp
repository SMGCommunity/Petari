#pragma once

#include "Game/MapObj/CoinGroup.hpp"

class CircleCoinGroup : public CoinGroup {
public:
    CircleCoinGroup(const char*);

    virtual ~CircleCoinGroup();
    virtual void initCoinArray(const JMapInfoIter&);
    virtual void placementCoin() const;
    virtual const char* getCoinName() const;

    f32 mCoinRadius;  // 0xA0
};

namespace MR {
    NameObj* createCircleCoinGroup(const char*);
    NameObj* createCirclePurpleCoinGroup(const char*);
};  // namespace MR
