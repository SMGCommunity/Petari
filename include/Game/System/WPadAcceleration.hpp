#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class WPad;

class WPadAcceleration {
public:
    WPadAcceleration(const WPad*, u32);

    bool getAcceleration(TVec3f*) const;
    bool getPastAcceleration(TVec3f*, s32) const;
    bool isStationary() const;
    bool isBalanced() const;
    void update();
    void updateRotate();
    void updateAccAverage();
    void updateIsStable();

    // TODO document members
    u8 _0[0x650];
};
