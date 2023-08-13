#include "Game/System/WPadHolder.h"
#include "Game/Util.h"

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