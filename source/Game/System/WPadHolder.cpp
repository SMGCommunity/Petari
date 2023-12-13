#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPad.hpp"
#include "Game/Util.hpp"

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

#ifdef NON_MATCHING
void WPadHolder::updateReadDataOnly() {
    for (s32 i = 0; i < 4u; i++) {
        WPadReadDataInfo* info = &mDataInfoArray[i];
        info->mValidStatusCount = KPADRead(i, info->mStatusArray, 0x78);
    }
}
#endif

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

    switch (_C) {
        case 1:
            updateInGame();
            break;
        case 0:
        default:
            break; 
    }
}

// WPadHolder::initSensorBarPosition

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