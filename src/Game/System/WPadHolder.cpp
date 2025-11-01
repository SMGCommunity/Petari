#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <revolution/kpad.h>
#include <revolution/wpad.h>

#define KPAD_STATUS_ARRAY_SIZE 120

WPadReadDataInfo::WPadReadDataInfo() :
    mStatusArray(nullptr),
    mValidStatusCount(0)
{
    mStatusArray = new KPADStatus[KPAD_STATUS_ARRAY_SIZE];

    MR::zeroMemory(mStatusArray, sizeof(KPADStatus) * KPAD_STATUS_ARRAY_SIZE);
}

KPADStatus* WPadReadDataInfo::getKPadStatus(u32 index) const {
    if (index >= mValidStatusCount) {
        return nullptr;
    }

    return &mStatusArray[index];
}

u32 WPadReadDataInfo::getValidStatusCount() const {
    return mValidStatusCount;
}

WPadHolder::WPadHolder() :
    mReadDataInfoArray(nullptr),
    mMode(WPAD_SENSOR_BAR_POS_TOP)
{
    WPADRegisterAllocator(MR::allocFromWPadHeap, MR::freeFromWPadHeap);
    KPADInit();

    mReadDataInfoArray = new WPadReadDataInfo[WPAD_MAX_CONTROLLERS];

    for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + sizeof(mPad) / sizeof(*mPad); chan++) {
        mPad[chan] = new WPad(chan);
        mPad[chan]->setReadInfo(&mReadDataInfoArray[chan]);
    }

    setConnectCallback();
    initSensorBarPosition();
}

void WPadHolder::updateReadDataOnly() {
    WPadReadDataInfo* pReadData;

    for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + WPAD_MAX_CONTROLLERS; chan++) {
        pReadData = &mReadDataInfoArray[chan];
        pReadData->mValidStatusCount = KPADRead(chan, pReadData->mStatusArray, KPAD_STATUS_ARRAY_SIZE);
    }
}

void WPadHolder::updateProjectPadData() {
    for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + sizeof(mPad) / sizeof(*mPad); chan++) {
        mPad[chan]->update();
    }
}

void WPadHolder::updateInGame() {
    for (s32 chan = WPAD_CHAN2; chan < WPAD_CHAN0 + WPAD_MAX_CONTROLLERS; chan++) {
        KPADStatus* pStatus = mReadDataInfoArray[chan].getKPadStatus(0);

        if (pStatus == nullptr) {
            continue;
        }

        if (pStatus->wpad_err != WPAD_ERR_NONE) {
            continue;
        }

        WPADDisconnect(chan);
    }
}

void WPadHolder::update() {
    updateReadDataOnly();
    updateProjectPadData();

    switch (mMode) {
    case WPAD_SENSOR_BAR_POS_TOP:
        updateInGame();
        break;
    case WPAD_SENSOR_BAR_POS_BOTTOM:
        break;
    }
}

void WPadHolder::initSensorBarPosition() {
    u8 sensorBarPosition = WPADGetSensorBarPosition();
    f32 level = 0.0f;

    switch (sensorBarPosition) {
    case WPAD_SENSOR_BAR_POS_TOP:
        level = 0.35;
        break;
    case WPAD_SENSOR_BAR_POS_BOTTOM:
        level = -0.15;
        break;
    }

    for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + sizeof(mPad) / sizeof(*mPad); chan++) {
        mPad[chan]->mPointer->setSensorBarLevel(level);
    }
}

void WPadHolder::resetPad() {
    KPADReset();

    for (s32 chan = WPAD_CHAN0; chan < WPAD_CHAN0 + sizeof(mPad) / sizeof(*mPad); chan++) {
        mPad[chan]->resetPad();
    }
}

WPad* WPadHolder::getWPad(s32 channel) {
    if (channel < WPAD_CHAN0 + sizeof(mPad) / sizeof(*mPad)) {
        return mPad[channel];
    }

    return nullptr;
}

namespace {
    WPadHolder* getWPadHolder() NO_INLINE {
        return SingletonHolder<GameSystem>::get()->mObjHolder->mWPadHolder;
    }
};

namespace MR {
    WPad* getWPad(s32 channel) {
        return ::getWPadHolder()->getWPad(channel);
    }

    void resetWPad() {
        WPadHolder* pWPadHolder;

        pWPadHolder = ::getWPadHolder();
        pWPadHolder->resetPad();
    }

    void setWPadHolderModeHomeButton() {
        WPadHolder* pWPadHolder;

        pWPadHolder = ::getWPadHolder();
        pWPadHolder->mMode = WPAD_SENSOR_BAR_POS_BOTTOM;
    }

    void setWPadHolderModeGame() {
        WPadHolder* pWPadHolder;

        pWPadHolder = ::getWPadHolder();
        pWPadHolder->mMode = WPAD_SENSOR_BAR_POS_TOP;
    }

    // getHBMKPadData

    void setAutoSleepTimeWiiRemote(bool isLongAutoSleep) {
        u8 minute = isLongAutoSleep ? 15 : 5;

        WPADSetAutoSleepTime(minute);
    }
};
