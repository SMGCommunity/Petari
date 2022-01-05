#pragma once

#include "JSystem/JGeometry.h"

class TDDraw {
public:
    static void sendPoint(const TVec3f &);

    static void setup(u32, u32, u8);

    static void drawFillFan(const TVec3f &, const TVec3f &, const TVec3f &, unsigned long, float, float, unsigned long);
};