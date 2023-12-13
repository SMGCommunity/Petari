#include "Game/MapObj/CircleCoinGroup.hpp"


CircleCoinGroup::CircleCoinGroup(const char *pName) : CoinGroup(pName) {
    mCoinRadius = 200.0f;
}

void CircleCoinGroup::initCoinArray(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg2NoInit(rIter, &mCoinRadius);
    MR::initDefaultPos(this, rIter);
}

/* CircleCoinGroup::placementCoin */

namespace MR {
    NameObj* createCircleCoinGroup(const char *pName) {
        CircleCoinGroup* group = new CircleCoinGroup(pName);
        return group;
    }

    NameObj* createCirclePurpleCoinGroup(const char *pName) {
        CircleCoinGroup* group = new CircleCoinGroup(pName);
        group->mIsPurpleCoinGroup = true;
        return group;
    }
};

CircleCoinGroup::~CircleCoinGroup() {

}

const char* CircleCoinGroup::getCoinName() const {
    return mIsPurpleCoinGroup ? "パープルコイン(円形配置)" : "コイン(円形配置)";
}