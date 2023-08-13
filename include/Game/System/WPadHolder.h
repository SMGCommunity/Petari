#pragma once

#include <revolution.h>

class WPadReadDataInfo {
public:
    WPadReadDataInfo();

    KPADStatus* getKPadStatus(u32) const;
    u32 getValidStatusCount() const;

    KPADStatus* mStatusArray;      // _0 
    u32 mValidStatusCount;          // _4
};