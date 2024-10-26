#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadPointer.hpp"
#include <revolution/wpad.h>
#include "Game/Util/MemoryUtil.hpp"

namespace {
    WPadHolder* getWPadHolder() NO_INLINE {
        return SingletonHolder<GameSystem>::sInstance->mObjHolder->mWPadHolder;
    }
};

WPadReadDataInfo::WPadReadDataInfo() {
    mStatusArray = nullptr;
    mValidStatusCount = 0;
    mStatusArray = new KPADStatus[0x78];
    MR::zeroMemory(mStatusArray, sizeof(KPADStatus) * 0x78);
}

KPADStatus* WPadReadDataInfo::getKPadStatus(u32 idx) const {
    if (idx >= mValidStatusCount) {
        return nullptr;
    }

    return &mStatusArray[idx];
}

u32 WPadReadDataInfo::getValidStatusCount() const {
    return mValidStatusCount;
}

WPadHolder::WPadHolder() {
    mDataInfoArray = nullptr;
    mHolderMode = 1;
    WPADRegisterAllocator(MR::allocFromWPadHeap, MR::freeFromWPadHeap);
    KPADInit();
    mDataInfoArray = new WPadReadDataInfo[4];

    for (u32 i = 0; i < 2; i++) {
        WPad* pad = new WPad(i);
        mPads[i] = pad;
        pad->setReadInfo(&mDataInfoArray[i]);
    }

    setConnectCallback();
    initSensorBarPosition();
}

void WPadHolder::updateReadDataOnly() {
    WPadReadDataInfo* info;
    for (s32 i = 0; i < 4; i++) {
        info = &mDataInfoArray[i];
        info->mValidStatusCount = KPADRead(i, info->mStatusArray, 0x78);
    }
}

void WPadHolder::updateProjectPadData() {
    for (s32 i = 0; i < 2u; i++) {
        mPads[i]->update();
    }
}

void WPadHolder::updateInGame() { 
    for (s32 i = 2; i < 4; i++) {
        KPADStatus* status = (&mDataInfoArray[i])->getKPadStatus(0);

        if (status != nullptr && !status->wpad_err) {
            WPADDisconnect(i);
        }
    }
}

void WPadHolder::update() {
    updateReadDataOnly();
    updateProjectPadData();

    switch (mHolderMode) {
        case 1:
            updateInGame();
            break;
        case 0:
        default:
            break; 
    }
}

void WPadHolder::initSensorBarPosition() {
    u8 barPos = WPADGetSensorBarPosition();
    f32 lvl = 0.0f;

    switch (barPos) {
        case 1:
            lvl = 0.34999999;
            break;
         case 0:
            lvl = -0.15000001;
            break;
    }

    for (int i = 0; i < 2; i++) {
        mPads[i]->mPointer->setSensorBarLevel(lvl);
    }
}

void WPadHolder::resetPad() {
    KPADReset();

    for (s32 i = 0; i < 2u; i++) {
        mPads[i]->resetPad();
    }
}

WPad* WPadHolder::getWPad(s32 idx) {
    if (idx < 2u) {
        return mPads[idx];
    }

    return nullptr; 
}

namespace MR {
    WPad* getWPad(s32 idx) {
        return ::getWPadHolder()->getWPad(idx);
    }

    void resetWPad() {
        ::getWPadHolder()->resetPad();
    }

    void setWPadHolderModeHomeButton() {
        WPadHolder* holder = ::getWPadHolder();
        holder->mHolderMode = 0;
    }

    void setWPadHolderModeGame() {
        WPadHolder* holder = ::getWPadHolder();
        holder->mHolderMode = 1;
    }

    // getHBMKPadData

    void setAutoSleepTimeWiiRemote(bool a1) {
        WPADSetAutoSleepTime(a1 != false ? 15 : 5);
    }
};