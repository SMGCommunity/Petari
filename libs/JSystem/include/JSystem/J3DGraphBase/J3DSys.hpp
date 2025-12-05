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
    u32 _54;
    u32 _58;

    u32 _5C;
    u32 _60, _64, _68, _6C;
    u32 _70, _74, _78, _7C;
    u32 _80, _84, _88, _8C;
    u32 _90, _94, _98, _9C;
    u32 _A0, _A4, _A8, _AC;
    u32 _B0, _B4, _B8, _BC;
    u32 _C0, _C4, _C8, _CC;
    u32 _D0, _D4, _D8, _DC;
    u32 _E0, _E4, _E8, _EC;
    u32 _F0, _F4, _F8, _FC;
    u32 _100;

    u32 _104;
    u32 _108;
    u32 _10C;
    u32 _110;
    u32 _114;

    void drawInit();
    void draw() const;
    void reinitGX();
    void setTexCacheRegion(_GXTexCacheSize);

    static Mtx mCurrentMtx;
};

static J3DSys j3dSys;
