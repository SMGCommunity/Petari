#include "Game/MapObj/ArrowSwitchMulti.h"
#include "Game/MapObj/ArrowSwitchMultiHolder.h"
#include "Game/Map/StageSwitch.h"

ArrowSwitchTarget::ArrowSwitchTarget(const char *pName) : NameObj(pName) {
    mJMapIDInfo = NULL;
    mStageSwitchCtrl = NULL;
    _14 = -1;
    MR::createArrowSwitchMultiHolder();
}

void ArrowSwitchTarget::init(const JMapInfoIter &rIter) {
    s32 arg;
    MR::getJMapInfoArg0WithInit(rIter, &arg);
    mJMapIDInfo = new JMapIdInfo(arg, rIter);
    MR::getJMapInfoArg1WithInit(rIter, &_14);
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void ArrowSwitchTarget::initAfterPlacement() {
    MR::registerArrowSwitchTarget(this);
}

void ArrowSwitchTarget::onTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->onSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->offSwitchB();
    }
}

void ArrowSwitchTarget::offTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->offSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->onSwitchB();
    }
}

ArrowSwitchTarget::~ArrowSwitchTarget() {
    
}