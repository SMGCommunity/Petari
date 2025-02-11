#include "Game/MapObj/CoinHolder.hpp"
#include "Game/MapObj/Coin.hpp"

CoinHolder::CoinHolder(const char *pName) : DeriveActorGroup<Coin>(pName, 0x200), mHostInfoArr(nullptr), mHostInfoCount(0) {
    mHostInfoArr = new CoinHostInfo[0x200];
}

bool CoinHolder::hopCoin(const NameObj *pObj, const TVec3f &a2, const TVec3f &a3) {
    CoinHostInfo* hostInfo = findHostInfo(pObj);

    if (hostInfo->_8 >= hostInfo->_4) {
        return false;
    }

    Coin* coin = getDeadMember();

    if (coin) {
        coin->setHostInfo(hostInfo);
        coin->appearHop(a2, a3);
        return true; 
    }

    return false;
}
 
bool CoinHolder::appearCoinFix(const NameObj *pObj, const TVec3f &a2, s32 a3) {
    TVec3f stack_8(0.0f, 0.0f, 0.0f);
    return appearCoin(pObj, a2, stack_8, a3, -1, -1, a3 == 1 ? 0.0f : 4.0f);
}

/*
bool CoinHolder::appearCoinPop(const NameObj *pObj, const TVec3f &a2, s32 a3) {
    TVec3f stack_20;
    MR::calcGravityVector(this, a2, &stack_20, nullptr, nullptr);
    TVec3f stack_8 = -stack_20 % 25.0f;
    return appearCoin(pObj, a2, stack_8, a3, -1, -1, a3 == 1 ? 0.0f : 4.0f);
}
*/

bool CoinHolder::appearCoinToVelocity(const NameObj *pObj, const TVec3f &a2, const TVec3f &a3, s32 a4) {
    return appearCoin(pObj, a2, a3, a4, -1, -1, a4 == 1 ? 0.0f : 4.0f);
} 

// CoinHolder::appearCoinCircle

CoinHostInfo* CoinHolder::declare(const NameObj *pObj, s32 a2) {
    if (a2 < 0) {
        return nullptr;
    }

    CoinHostInfo* hostInfo = findHostInfo(pObj);
    if (!hostInfo) {
        hostInfo = &mHostInfoArr[mHostInfoCount];
        hostInfo->mHostActor = pObj;
        mHostInfoCount++;
    }

    hostInfo->_4 += a2;
    return hostInfo;
}

s32 CoinHolder::getDeclareRemnantCoinCount(const NameObj *pObj) const {
    CoinHostInfo* hostInfo = findHostInfo(pObj);

    if (MR::isGalaxyDarkCometAppearInCurrentStage()) {
        return 0;
    }

    return hostInfo->_4 - hostInfo->_8;
}

CoinHostInfo* CoinHolder::findHostInfo(const NameObj *pObj) const {
    for (s32 i = 0; i < mHostInfoCount; i++) {
        if (mHostInfoArr[i].mHostActor == pObj) {
            return &mHostInfoArr[i];
        }
    }

    return nullptr;
}

void CoinHolder::init(const JMapInfoIter &rIter) {
    for (int i = 0; i < 0x20; i++) {
        Coin* coin = new Coin("コイン(共用)");
        coin->initWithoutIter();
        registerActor(coin);
    }
}
