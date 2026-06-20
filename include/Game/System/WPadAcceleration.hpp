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

    WPad* mPad;  // 0x00
    u32 _4;
    u32 _8;
    u32 _C;
    TVec3f _10;
    f32 _1C;
    bool _20;
    TVec3f mHistory[128];  // 0x24
    s32 _624;
    u32 _628;
    TVec3f _62C;
    TVec3f _638;
    u32 _644;
    s32 _648;
    s32 _64C;
};
