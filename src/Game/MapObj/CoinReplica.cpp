#include "Game/MapObj/CoinReplica.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/Coin.hpp"
#include "Game/Util.hpp"

CoinReplica::CoinReplica(const char* pName) : NameObj(pName) {
    mCoin = nullptr;
}

void CoinReplica::activeCoin() {
    if (mCoin) {
        mCoin->requestActive();
    }
}

void CoinReplica::activeCoinWithGravity() {
    if (mCoin) {
        mCoin->requestActiveWithGravity();
    }
}

void CoinReplica::deactiveCoin() {
    if (mCoin) {
        mCoin->requestDeactive();
    }
}

void CoinReplica::removeCoin() {
    if (!MR::isDead(mCoin)) {
        mCoin->kill();
    }
}

void CoinReplica::init(const JMapInfoIter& rIter) {
    mCoin = static_cast< Coin* >(MR::createCoin(this, "コイン(レプリカ用)"));
    MR::initDefaultPos(mCoin, rIter);
    mCoin->initWithoutIter();
    mCoin->appearNonActive();

    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    if (MR::isExistStageSwitchA(rIter) || MR::isExistStageSwitchB(rIter)) {
        StageSwitchCtrl* switchCtrl = MR::createStageSwitchCtrl(this, rIter);

        if (switchCtrl->isValidSwitchA()) {
            if (arg0 == -1) {
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, &CoinReplica::deactiveCoin),
                                                   MR::Functor(this, &CoinReplica::activeCoin));
            } else {
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, &CoinReplica::deactiveCoin),
                                                   MR::Functor(this, &CoinReplica::activeCoinWithGravity));
            }
        }

        if (switchCtrl->isValidSwitchB()) {
            MR::listenNameObjStageSwitchOnB(this, switchCtrl, MR::Functor(this, &CoinReplica::removeCoin));
        }
    }
}

CoinReplica::~CoinReplica() {
}
