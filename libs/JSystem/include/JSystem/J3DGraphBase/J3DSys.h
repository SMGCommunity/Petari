#pragma once

#include <revolution.h>

class J3DSys {
public:
    Mtx mViewMtx;   // _0
    static Mtx mCurrentMtx;
};

static J3DSys j3dSys;