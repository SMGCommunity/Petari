#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class NameObj;
class Coin;
class CoinHostInfo;

class CoinHolder : public DeriveActorGroup< Coin > {
public:
    CoinHolder(const char* pName);

    virtual ~CoinHolder() {
    }

    virtual void init(const JMapInfoIter& rIter);

    bool hopCoin(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity);
    bool appearCoinFix(const NameObj* pObj, const TVec3f& rPosition, s32 count);
    bool appearCoinPop(const NameObj* pObj, const TVec3f& rPosition, s32 count);
    bool appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity, s32 count);
    bool appearCoinToVelocity(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity, s32 count);
    bool appearCoinCircle(const NameObj* pObj, const TVec3f& rPosition, s32 count);
    CoinHostInfo* declare(const NameObj* pObj, s32 count);
    s32 getDeclareRemnantCoinCount(const NameObj* pObj) const;
    CoinHostInfo* findHostInfo(const NameObj* pObj) const;
    bool appearCoin(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity, s32 count, s32 life, s32 cannotTime, f32 randomize);

    /* 0x18 */ CoinHostInfo* mHostInfoArr;
    /* 0x1C */ s32 mHostInfoCount;
};

namespace MR {
    void createCoinHolder();
    void createCoinRotater();

    void addToCoinHolder(const NameObj* pObj, Coin* pCoin);
    CoinHolder* getCoinHolder();
};  // namespace MR
