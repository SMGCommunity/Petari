#pragma once

class NameObj;
class Coin;
class CoinHostInfo;

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CoinHolder : public DeriveActorGroup< Coin > {
public:
    CoinHolder(const char*);

    virtual ~CoinHolder() {}

    virtual void init(const JMapInfoIter&);

    bool hopCoin(const NameObj*, const TVec3f&, const TVec3f&);
    bool appearCoinFix(const NameObj*, const TVec3f&, s32);
    bool appearCoinPop(const NameObj*, const TVec3f&, s32);
    bool appearCoinPopToDirection(const NameObj*, const TVec3f&, const TVec3f&, s32);
    bool appearCoinToVelocity(const NameObj*, const TVec3f&, const TVec3f&, s32);
    bool appearCoinCircle(const NameObj*, const TVec3f&, s32);
    CoinHostInfo* declare(const NameObj*, s32);
    s32 getDeclareRemnantCoinCount(const NameObj*) const;
    CoinHostInfo* findHostInfo(const NameObj*) const;
    bool appearCoin(const NameObj*, const TVec3f&, const TVec3f&, s32, s32, s32, f32);

    CoinHostInfo* mHostInfoArr;  // 0x18
    s32 mHostInfoCount;          // 0x1C
};

namespace MR {
    void createCoinHolder();
    void createCoinRotater();

    void addToCoinHolder(const NameObj*, Coin*);
    CoinHolder* getCoinHolder();
};  // namespace MR
