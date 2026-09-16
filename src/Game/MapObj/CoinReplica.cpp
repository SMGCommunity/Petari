#include "Game/MapObj/CoinReplica.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/Coin.hpp"
#include "Game/Util.hpp"

CoinReplica::CoinReplica(const char* pName) : NameObj(pName), mCoin() {
}

void CoinReplica::activeCoin() {
    if (mCoin != nullptr) {
        mCoin->requestActive();
    }
}

void CoinReplica::activeCoinWithGravity() {
    if (mCoin != nullptr) {
        mCoin->requestActiveWithGravity();
    }
}

void CoinReplica::deactiveCoin() {
    if (mCoin != nullptr) {
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

    s32 calcGravity;
    MR::getJMapInfoArg0WithInit(rIter, &calcGravity);

    if (MR::isExistStageSwitchA(rIter) || MR::isExistStageSwitchB(rIter)) {
        StageSwitchCtrl* switchCtrl = MR::createStageSwitchCtrl(this, rIter);

        if (switchCtrl->isValidSwitchA()) {
            if (calcGravity == -1) {
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, &CoinReplica::activeCoin),
                                                   MR::Functor(this, &CoinReplica::deactiveCoin));
            } else {
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, &CoinReplica::activeCoinWithGravity),
                                                   MR::Functor(this, &CoinReplica::deactiveCoin));
            }
        }

        if (switchCtrl->isValidSwitchB()) {
            MR::listenNameObjStageSwitchOnB(this, switchCtrl, MR::Functor(this, &CoinReplica::removeCoin));
        }
    }
}
