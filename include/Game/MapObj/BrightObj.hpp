#pragma once

#include "JSystem/JGeometry.hpp"

class BrightInfo {
public:
    BrightInfo();

    void endRead();
    void reset();

    u32 _0;
    u32 _4;
    TVec2f mBrightnessCenter[3];    // 0x8
    TVec2f mRealCenter[3];          // 0x20
    f32 mBright[3];                 // 0x38
};

class BrightDrawInfo {
public:
    BrightDrawInfo();

    TVec3f mPosition[2];    // 0x0
    f32 mRadius[2];         // 0x18
};

class BrightCamInfo {
public:
    BrightCamInfo();

    TMtx34f mViewMtx[2];            // 0x0
    Mtx44 mProjectionMtx[2];        // 0x60
    TVec3f mCameraDir[2];           // 0xE0
    TVec3f mCameraPos[2];           // 0xF8
};