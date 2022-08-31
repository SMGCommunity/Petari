#pragma once

#include "JSystem/JGeometry.h"

class BrightInfo {
public:
    BrightInfo();

    void endRead();
    void reset();

    u32 _0;
    u32 _4;
    TVec2f mBrightnessCenter[3];    // _8
    TVec2f mRealCenter[3];          // _20
    f32 mBright[3];                 // _38
};

class BrightDrawInfo {
public:
    BrightDrawInfo();

    TVec3f mPosition[2];    // _0
    f32 mRadius[2];         // _18
};

class BrightCamInfo {
public:
    BrightCamInfo();

    TMtx34f mViewMtx[2];            // _0
    Mtx44 mProjectionMtx[2];        // _60
    TVec3f mCameraDir[2];           // _E0
    TVec3f mCameraPos[2];           // _F8
};