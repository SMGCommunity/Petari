#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

namespace MR {
    void getCorePadAcceleration(TVec3f *, s32);
    bool testCorePadButtonUp(s32);
    bool testCorePadButtonDown(s32);

    bool testCorePadTriggerA(s32);
    bool testCorePadTriggerB(s32);
    bool testSystemPadTriggerDecide();
    bool testSystemTriggerA();

    f32 getSubPadStickX(s32);
    f32 getSubPadStickY(s32);

    bool testDPDMenuPadDecideTrigger();	

    bool isPadSwing(s32);
};