#include "Game/MapObj/PurpleCoinStarter.h"
#include "Game/Util.h"

PurpleCoinStarter::PurpleCoinStarter(const char *pName) : NameObj(pName) {
    mCoinHolder = nullptr;
    mSwitchCtrl = nullptr;
}

void PurpleCoinStarter::init(const JMapInfoIter &rIter) {
    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
    MR::listenNameObjStageSwitchOnAppear(this, mSwitchCtrl, MR::FunctorV0M<PurpleCoinStarter *, void (PurpleCoinStarter::*)(void)>(this, &PurpleCoinStarter::start));
    MR::createPurpleCoinHolder();
    MR::registPurpleCoinStarter(this);
}

void PurpleCoinStarter::setHost(PurpleCoinHolder *pHolder) {
    mCoinHolder = pHolder;
}

void PurpleCoinStarter::start() {
    mCoinHolder->start();
}

PurpleCoinStarter::~PurpleCoinStarter() {
    
}