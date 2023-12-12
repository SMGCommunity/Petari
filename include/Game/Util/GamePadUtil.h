#pragma once

#include "JSystem/JGeometry.h"
#include <revolution.h>

namespace MR {
    void getCorePadAcceleration(TVec3f *, s32);
    bool testCorePadButtonUp(s32);
    bool testCorePadButtonDown(s32);

    bool testCorePadTriggerB(s32);

    bool testDPDMenuPadDecideTrigger();

    bool isPadSwing(s32);
};