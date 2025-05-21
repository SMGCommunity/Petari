#include "Game/Screen/BatteryInfo.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace {
    NEW_NERVE(BatteryInfoEnought, BatteryInfo, Enought);
    NEW_NERVE(BatteryInfoLowAppear, BatteryInfo, LowAppear);
    NEW_NERVE(BatteryInfoLow, BatteryInfo, Low);
    NEW_NERVE(BatteryInfoLowDisappear, BatteryInfo, LowDisappear);
    NEW_NERVE(BatteryInfoLowHide, BatteryInfo, LowHide);
    NEW_NERVE(BatteryInfoDisappear, BatteryInfo, Disappear);
    NEW_NERVE(BatteryInfoCriticalAppear, BatteryInfo, CriticalAppear);
    NEW_NERVE(BatteryInfoCritical, BatteryInfo, Critical);
    NEW_NERVE(BatteryInfoNoneAppear, BatteryInfo, NoneAppear);
    NEW_NERVE(BatteryInfoNone, BatteryInfo, None);
    NEW_NERVE(BatteryInfoNoneDisappear, BatteryInfo, NoneDisappear);
    NEW_NERVE(BatteryInfoNoneHide, BatteryInfo, NoneHide);
};

BatteryInfo::BatteryInfo(LayoutActor *pLayoutActor, const char *pPaneName, s32 channel) : NerveExecutor("BatteryInfo") {
    mLayoutActor = pLayoutActor;
    mPaneName = pPaneName;
    mChannel = channel;

    initNerve(&::BatteryInfoNoneHide::sInstance);
    MR::createAndAddPaneCtrl(mLayoutActor, mPaneName, 1);
    MR::hidePane(mLayoutActor, mPaneName);
}

BatteryInfo::~BatteryInfo() {
    
}

void BatteryInfo::exeEnought() {
    if (MR::isFirstStep(this))
        MR::hidePane(mLayoutActor, mPaneName);

    if (MR::getWPad(mChannel)->getBattery() == -1) {
        setNerve(&::BatteryInfoNoneHide::sInstance);
    }
    else {
        if (!MR::getWPad(mChannel)->getBattery()) {
            setNerve(&::BatteryInfoCriticalAppear::sInstance);
        }
        else if (MR::getWPad(mChannel)->getBattery() == 1) {
            setNerve(&::BatteryInfoLowAppear::sInstance);
        }
    }
}

void BatteryInfo::exeLowAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mLayoutActor, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_LOW", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearLow", &::BatteryInfoLow::sInstance);
}

void BatteryInfo::exeLow() {
    if (MR::isFirstStep(this))
        MR::startPaneAnim(mLayoutActor, mPaneName, "WaitLow", 0);

    tryChangeNerveWithBatteryLeftAlreadyAppear();

    if (MR::isGreaterStep(this, 120))
        setNerve(&::BatteryInfoLowDisappear::sInstance);
}

void BatteryInfo::exeLowDisappear() {
    exeStartAnimAndSetNerveIfStopped(
        "EndLow",
        &::BatteryInfoLowHide::sInstance
    );
}

void BatteryInfo::exeLowHide() {
    if (MR::isFirstStep(this))
        MR::hidePane(mLayoutActor, mPaneName);

    if (MR::getWPad(mChannel)->getBattery() == -1) {
        if (mChannel == 1) {
            setNerve(&::BatteryInfoNoneAppear::sInstance);
        }
        else {
            setNerve(&::BatteryInfoNoneHide::sInstance);
        }
    }
    else {
        if (!MR::getWPad(mChannel)->getBattery()) {
            setNerve(&::BatteryInfoCriticalAppear::sInstance);
        }
        else {
            if (MR::getWPad(mChannel)->getBattery() > 1) {
                setNerve(&::BatteryInfoEnought::sInstance);                
            }
            else if (MR::isGreaterStep(this, 18000)) {
                setNerve(&::BatteryInfoLowAppear::sInstance);
            }
        }
    }
}

void BatteryInfo::exeCriticalAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mLayoutActor, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_CRITICAL", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearCritical", &::BatteryInfoCritical::sInstance);
}

void BatteryInfo::exeCritical() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mLayoutActor, mPaneName, "WaitCritical", 0);
    }

    tryChangeNerveWithBatteryLeftAlreadyAppear();
}

void BatteryInfo::exeDisappear() {
    exeStartAnimAndSetNerveIfStopped(
        "EndCritical",
        &::BatteryInfoEnought::sInstance
    );
}

void BatteryInfo::exeNoneAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mLayoutActor, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_CRITICAL", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearNone", &::BatteryInfoNone::sInstance);
}

void BatteryInfo::exeNone() {
    if (MR::isFirstStep(this))
        MR::startPaneAnim(mLayoutActor, mPaneName, "WaitNone", 0);

    if (MR::isGreaterStep(this, 450))
        setNerve(&::BatteryInfoNoneDisappear::sInstance);
}

void BatteryInfo::exeNoneDisappear() {
    exeStartAnimAndSetNerveIfStopped(
        "EndNone",
        &::BatteryInfoNoneHide::sInstance
    );
}

void BatteryInfo::exeNoneHide() {
    if (MR::isFirstStep(this))
        MR::hidePane(mLayoutActor, mPaneName);

    if (MR::getWPad(mChannel)->getBattery() != -1)
        setNerve(&::BatteryInfoEnought::sInstance);
}

bool BatteryInfo::exeStartAnimAndSetNerveIfStopped(const char *pAnimName, const Nerve *pNerve) {
    if (MR::isFirstStep(this))
        MR::startPaneAnim(mLayoutActor, mPaneName, pAnimName, 0);

    if (MR::isPaneAnimStopped(mLayoutActor, mPaneName, 0)) {
        setNerve(pNerve);
        return true;
    }

    return false;
}

bool BatteryInfo::tryChangeNerveWithBatteryLeftAlreadyAppear() {
    const Nerve *pNerve;

    switch (MR::getWPad(mChannel)->getBattery()) {
        case -1:
            if (mChannel == 1) {
                pNerve = &::BatteryInfoNoneAppear::sInstance;
            }
            else {
                pNerve = &::BatteryInfoDisappear::sInstance;
            }
            break;
        case 0:
            pNerve = &::BatteryInfoCritical::sInstance;
            break;
        case 1:
            pNerve = &::BatteryInfoLow::sInstance;
            break;
        default:
            pNerve = &::BatteryInfoDisappear::sInstance;
            break;
    }

    if (!isNerve(pNerve)) {
        setNerve(pNerve);
        return true;
    }

    return false;    
}

BatteryLayout::BatteryLayout() : LayoutActor("BatteryLayout", true) {
    mBatteryInfo1 = nullptr;
    mBatteryInfo2 = nullptr;
}

BatteryLayout::~BatteryLayout() {

}

void BatteryLayout::init(const JMapInfoIter &rIter) {
    initLayoutManager("BatteryInfo", 1);

    mBatteryInfo1 = new BatteryInfo(this, "WinBattery1", 0);
    mBatteryInfo2 = new BatteryInfo(this, "WinBattery2", 1);

    appear();
}

void BatteryLayout::appear() {
    LayoutActor::appear();
    mBatteryInfo1->setNerve(&::BatteryInfoNoneHide::sInstance);
    mBatteryInfo2->setNerve(&::BatteryInfoNoneHide::sInstance);
}

void BatteryLayout::control() {
    mBatteryInfo1->updateNerve();
    mBatteryInfo2->updateNerve();
}
