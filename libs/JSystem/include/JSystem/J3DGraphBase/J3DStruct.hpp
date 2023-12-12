#pragma once

#include <revolution.h>

class J3DGXColor {
public:
    J3DGXColor& operator=(_GXColor);

    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

class J3DFogInfo {
public:
    inline J3DFogInfo() {
        
    }

    J3DFogInfo operator=(const J3DFogInfo &);

    u8 mFogType;        // _0
    u8 _1;
    u16 _2;
    f32 mStartZ;        // _4
    f32 mEndZ;          // _8
    f32 mNearZ;         // _C
    f32 mFarZ;          // _10
    _GXColor mColor;    // _14
    u16 mTable[10];     //  _18
};