#pragma once

#include <revolution.h>

class J3DSys {
public:
    Mtx mViewMtx;   // 0x0
    static Mtx mCurrentMtx;
};

static J3DSys j3dSys;