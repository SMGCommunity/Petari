#include "Game/MapObj/CoinReplica.h"

CoinReplica::CoinReplica(const char *pName) : NameObj(pName) {
    mCoin = NULL;
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

void CoinReplica::init(const JMapInfoIter &rIter) {
    mCoin = reinterpret_cast<Coin*>(MR::createCoin(this, "コイン(レプリカ用)"));
    MR::initDefaultPos(mCoin, rIter);
    mCoin->initWithoutIter();
    mCoin->appearNonActive();
    s32 arg0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (MR::isExistStageSwitchA(rIter) || MR::isExistStageSwitchB(rIter)) {
        StageSwitchCtrl* switchCtrl = MR::createStageSwitchCtrl(this, rIter);

        if (switchCtrl->isValidSwitchA()) {
            if (arg0 == -1) {
                void (CoinReplica::*d)(void) = &CoinReplica::deactiveCoin;
                void (CoinReplica::*a)(void) = &CoinReplica::activeCoin;
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, d), MR::Functor(this, a));
            }
            else {
                void (CoinReplica::*d)(void) = &CoinReplica::deactiveCoin;
                void (CoinReplica::*a)(void) = &CoinReplica::activeCoinWithGravity;
                MR::listenNameObjStageSwitchOnOffA(this, switchCtrl, MR::Functor(this, d), MR::Functor(this, a));
            }
        }

        if (switchCtrl->isValidSwitchB()) {
            void (CoinReplica::*d)(void) = &CoinReplica::removeCoin;
            MR::listenNameObjStageSwitchOnB(this, switchCtrl, MR::Functor(this, d));
        }
    }
}

CoinReplica::~CoinReplica() {

}