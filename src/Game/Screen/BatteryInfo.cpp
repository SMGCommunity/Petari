#include "Game/Screen/BatteryInfo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

#define WPAD_BATTERY_LEVEL_EMPTY -1

namespace {
    static const s32 sDisplayFrameBatteryLow = 120;
    static const s32 sLowIntervalFrame = 18000;
    static const s32 sDisplayFrameBatteryNone = 450;
};  // namespace

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
};  // namespace

BatteryInfo::BatteryInfo(LayoutActor* pHost, const char* pPaneName, int channel)
    : NerveExecutor("BatteryInfo"), mHost(pHost), mPaneName(pPaneName), mChannel(channel) {
    initNerve(&BatteryInfoNoneHide::sInstance);
    MR::createAndAddPaneCtrl(mHost, mPaneName, 1);
    MR::hidePane(mHost, mPaneName);
}

void BatteryInfo::exeEnought() {
    if (MR::isFirstStep(this)) {
        MR::hidePane(mHost, mPaneName);
    }

    if (MR::getWPad(mChannel)->getBattery() == WPAD_BATTERY_LEVEL_EMPTY) {
        setNerve(&BatteryInfoNoneHide::sInstance);
    } else if (MR::getWPad(mChannel)->getBattery() == WPAD_BATTERY_LEVEL_CRITICAL) {
        setNerve(&BatteryInfoCriticalAppear::sInstance);
    } else if (MR::getWPad(mChannel)->getBattery() == WPAD_BATTERY_LEVEL_LOW) {
        setNerve(&BatteryInfoLowAppear::sInstance);
    }
}

void BatteryInfo::exeLowAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mHost, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_LOW", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearLow", &BatteryInfoLow::sInstance);
}

void BatteryInfo::exeLow() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, "WaitLow", 0);
    }

    tryChangeNerveWithBatteryLeftAlreadyAppear();

    if (MR::isGreaterStep(this, sDisplayFrameBatteryLow)) {
        setNerve(&BatteryInfoLowDisappear::sInstance);
    }
}

void BatteryInfo::exeLowDisappear() {
    exeStartAnimAndSetNerveIfStopped("EndLow", &BatteryInfoLowHide::sInstance);
}

void BatteryInfo::exeLowHide() {
    if (MR::isFirstStep(this)) {
        MR::hidePane(mHost, mPaneName);
    }

    if (MR::getWPad(mChannel)->getBattery() == WPAD_BATTERY_LEVEL_EMPTY) {
        if (mChannel == WPAD_CHAN1) {
            setNerve(&BatteryInfoNoneAppear::sInstance);
        } else {
            setNerve(&BatteryInfoNoneHide::sInstance);
        }
    } else if (MR::getWPad(mChannel)->getBattery() == WPAD_BATTERY_LEVEL_CRITICAL) {
        setNerve(&BatteryInfoCriticalAppear::sInstance);
    } else if (MR::getWPad(mChannel)->getBattery() > WPAD_BATTERY_LEVEL_LOW) {
        setNerve(&BatteryInfoEnought::sInstance);
    } else if (MR::isGreaterStep(this, sLowIntervalFrame)) {
        setNerve(&BatteryInfoLowAppear::sInstance);
    }
}

void BatteryInfo::exeDisappear() {
    exeStartAnimAndSetNerveIfStopped("EndCritical", &BatteryInfoEnought::sInstance);
}

void BatteryInfo::exeCriticalAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mHost, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_CRITICAL", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearCritical", &BatteryInfoCritical::sInstance);
}

void BatteryInfo::exeCritical() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, "WaitCritical", 0);
    }

    tryChangeNerveWithBatteryLeftAlreadyAppear();
}

void BatteryInfo::exeNoneAppear() {
    if (MR::isFirstStep(this)) {
        MR::showPane(mHost, mPaneName);
        MR::startSystemSE("SE_SY_BATTERY_CRITICAL", -1, -1);
    }

    exeStartAnimAndSetNerveIfStopped("AppearNone", &BatteryInfoNone::sInstance);
}

void BatteryInfo::exeNone() {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, "WaitNone", 0);
    }

    if (MR::isGreaterStep(this, sDisplayFrameBatteryNone)) {
        setNerve(&BatteryInfoNoneDisappear::sInstance);
    }
}

void BatteryInfo::exeNoneDisappear() {
    exeStartAnimAndSetNerveIfStopped("EndNone", &BatteryInfoNoneHide::sInstance);
}

void BatteryInfo::exeNoneHide() {
    if (MR::isFirstStep(this)) {
        MR::hidePane(mHost, mPaneName);
    }

    if (MR::getWPad(mChannel)->getBattery() != -1) {
        setNerve(&BatteryInfoEnought::sInstance);
    }
}

bool BatteryInfo::exeStartAnimAndSetNerveIfStopped(const char* pAnimName, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, pAnimName, 0);
    }

    if (MR::isPaneAnimStopped(mHost, mPaneName, 0)) {
        setNerve(pNerve);

        return true;
    }

    return false;
}

bool BatteryInfo::tryChangeNerveWithBatteryLeftAlreadyAppear() {
    const Nerve* pNerve;

    switch (MR::getWPad(mChannel)->getBattery()) {
    case WPAD_BATTERY_LEVEL_EMPTY:
        if (mChannel == WPAD_CHAN1) {
            pNerve = &BatteryInfoNoneAppear::sInstance;
        } else {
            pNerve = &BatteryInfoDisappear::sInstance;
        }
        break;
    case WPAD_BATTERY_LEVEL_CRITICAL:
        pNerve = &BatteryInfoCritical::sInstance;
        break;
    case WPAD_BATTERY_LEVEL_LOW:
        pNerve = &BatteryInfoLow::sInstance;
        break;
    default:
        pNerve = &BatteryInfoDisappear::sInstance;
        break;
    }

    if (!isNerve(pNerve)) {
        setNerve(pNerve);

        return true;
    }

    return false;
}

BatteryLayout::BatteryLayout() : LayoutActor("BatteryLayout", true), mInfo1P(nullptr), mInfo2P(nullptr) {}

void BatteryLayout::init(const JMapInfoIter& rIter) {
    initLayoutManager("BatteryInfo", 1);

    mInfo1P = new BatteryInfo(this, "WinBattery1", 0);
    mInfo2P = new BatteryInfo(this, "WinBattery2", 1);

    appear();
}

void BatteryLayout::appear() {
    LayoutActor::appear();
    mInfo1P->setNerve(&BatteryInfoNoneHide::sInstance);
    mInfo2P->setNerve(&BatteryInfoNoneHide::sInstance);
}

void BatteryLayout::control() {
    mInfo1P->updateNerve();
    mInfo2P->updateNerve();
}
