#pragma once

#include "Game/MapObj/CoinGroup.hpp"

class CircleCoinGroup : public CoinGroup {
public:
    CircleCoinGroup(const char*);

    /* 0x08 */ virtual ~CircleCoinGroup();
    /* 0x78 */ virtual void initCoinArray(const JMapInfoIter&);
    /* 0x7C */ virtual void placementCoin();
    /* 0x80 */ virtual const char* getCoinName() const;

    f32 mCoinRadius;  // 0xA0
};

namespace MR {
    NameObj* createCircleCoinGroup(const char*);
    NameObj* createCirclePurpleCoinGroup(const char*);
};  // namespace MR
