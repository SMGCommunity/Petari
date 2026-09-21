#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class WPad;

class WPadAcceleration {
public:
    WPadAcceleration(const WPad* pPad, u32 device);

    bool getAcceleration(TVec3f* pOut) const;
    bool getPastAcceleration(TVec3f* pOut, s32 index) const;
    bool isStationary() const;
    bool isBalanced() const;
    void update();
    void updateRotate();
    void updateAccAverage();
    void updateIsStable();

    /* 0x00 */ const WPad* mPad;
    /* 0x04 */ u32 _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ f32 _C;
    /* 0x10 */ TVec3f _10;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ bool _20;
    /* 0x24 */ TVec3f mHistory[128];
    /* 0x624 */ s32 _624;
    /* 0x628 */ s32 _628;
    /* 0x62C */ TVec3f _62C;
    /* 0x638 */ TVec3f _638;
    /* 0x644 */ s32 _644;
    /* 0x648 */ s32 _648;
    /* 0x64C */ s32 _64C;
};
