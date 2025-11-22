#pragma once

#include <revolution.h>

class J3DDrawBuffer;

class J3DSys {
public:
    Mtx mViewMtx;  // 0x0
    u32 _30;
    u32 _34;
    u32 _38;
    u32 _3C;
    u32 _40;
    u32 _44;
    J3DDrawBuffer* _48;
    J3DDrawBuffer* _4C;
    u32 _50;

    void drawInit();
    void draw() const;
    void reinitGX();
    void setTexCacheRegion(_GXTexCacheSize);

    static Mtx mCurrentMtx;
};

static J3DSys j3dSys;
