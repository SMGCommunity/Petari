#include "Game/MapObj/CircleCoinGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

CircleCoinGroup::CircleCoinGroup(const char* pName) : CoinGroup(pName) {
    mCoinRadius = 200.0f;
}

void CircleCoinGroup::initCoinArray(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg2NoInit(rIter, &mCoinRadius);
    MR::initDefaultPos(this, rIter);
}

void CircleCoinGroup::placementCoin() {
    TPos3f mtx;
    MR::makeMtxTR(mtx, this);

    TVec3f front, side;
    mtx.getXDir(side);
    mtx.getZDir(front);

    TVec3f center = mPosition;
    f32 angle = 0.0f;
    f32 interval = (TWO_PI / mCoinCount);

    for (s32 i = 0; i < mCoinCount; i++) {
        f32 c = mCoinRadius * MR::cos(angle);
        f32 s = mCoinRadius * MR::sin(angle);

        setCoinTrans(i, front * c + side * s + center);
        angle += interval;
    }
}

namespace MR {
    NameObj* createCircleCoinGroup(const char* pName) {
        CircleCoinGroup* group = new CircleCoinGroup(pName);
        return group;
    }

    NameObj* createCirclePurpleCoinGroup(const char* pName) {
        CircleCoinGroup* group = new CircleCoinGroup(pName);
        group->mIsPurpleCoinGroup = true;
        return group;
    }
};  // namespace MR

CircleCoinGroup::~CircleCoinGroup() {
}

const char* CircleCoinGroup::getCoinName() const {
    return mIsPurpleCoinGroup ? "パープルコイン(円形配置)" : "コイン(円形配置)";
}
