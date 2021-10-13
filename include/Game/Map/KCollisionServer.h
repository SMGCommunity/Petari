#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

struct KC_PrismData {
    f32 mHeight; // _0
    u16 _4;
    u16 _6;
    u16 _8;
    u16 _A;
    u16 _C;
    u16 _E;
};

struct Fxyz {
    f32 x, y, z;
};

struct KCLFile {
    TVec3f* mVertOffs; // _0
    TVec3f* mNormOffs; // _4
    KC_PrismData* mTriOffs; // _8
    u32 mSpacOffs; // _C
    f32 mThickness; // _10
    TVec3f mFirstSpacCoord; // _14
    u32 mXMask; // _20
    u32 mYMask; // _24
    u32 mZMask; // _28
    u32 mCordShift; // _2C
    u32 mYShift; // _30
    u32 mZShift; // _34
    f32 mRadCap; // _38
};

class KCollisionServer {
public:
    KCollisionServer(void *);

    void setData(void *);

    static bool isBinaryInitialized(const void *);

    KCLFile* mFile; // _0
};