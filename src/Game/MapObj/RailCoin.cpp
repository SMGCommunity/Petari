#include "Game/Mapobj/RailCoin.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"

namespace {
    class RailCoinMercatorDivInfo : public DivideMercatorRailPosInfo {
    public:
        inline RailCoinMercatorDivInfo(CoinGroup* pGroup) { mGroup = pGroup; }

        virtual void setPosition(s32, const TVec3f&);

        CoinGroup* mGroup;  // 0x4
    };
};  // namespace

RailCoin::RailCoin(const char* pName) : CoinGroup(pName) {}

void RailCoin::initCoinArray(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
}

void RailCoin::placementNormalRail() {
    f32 speed;
    f32 length = MR::getRailTotalLength(this);
    u32 coinCount = mCoinCount;

    if (mCoinCount <= 1) {
        speed = 0.0f;
    } else {
        if (MR::isLoopRail(this)) {
            speed = length / coinCount;
        } else {
            speed = length / (coinCount - 1);
        }
    }

    MR::moveCoordToStartPos(this);
    MR::setRailCoordSpeed(this, speed);

    for (s32 i = 0; i < coinCount; i++) {
        setCoinTrans(i, MR::getRailPos(this));
        MR::moveRailRider(this);
    }
}

void RailCoin::placementMercatorRail() {
    RailCoinMercatorDivInfo info(this);
    MR::getDivideMercatorRailPosition(&info, this, mCoinCount, 10.0f, 10);
}

namespace MR {
    NameObj* createRailCoin(const char* pName) {
        return new RailCoin(pName);
    }

    NameObj* createRailPurpleCoin(const char* pName) {
        RailCoin* coin = new RailCoin(pName);
        coin->mIsPurpleCoinGroup = true;
        return coin;
    }
};  // namespace MR

namespace {
    void RailCoinMercatorDivInfo::setPosition(s32 idx, const TVec3f& rPos) {
        mGroup->setCoinTrans(idx, rPos);
    }
};  // namespace

RailCoin::~RailCoin() {}

void RailCoin::placementCoin() {
    if (MR::isInAreaObj("MercatorCube", mPosition)) {
        placementMercatorRail();
    } else {
        placementNormalRail();
    }
}

const char* RailCoin::getCoinName() const {
    return mIsPurpleCoinGroup ? "パープルコイン(レール配置)" : "コイン(レール配置)";
}
