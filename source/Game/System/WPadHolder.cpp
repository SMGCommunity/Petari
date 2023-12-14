#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadPointer.hpp"
#include <revolution/wpad.h>
#include "Game/Util/MemoryUtil.hpp"

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