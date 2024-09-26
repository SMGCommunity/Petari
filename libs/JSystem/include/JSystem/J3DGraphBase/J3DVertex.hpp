#pragma once

#include <revolution.h>

class J3DDrawMtxData {
public:
    J3DDrawMtxData();

    ~J3DDrawMtxData();

    u16 mDrawMatrixCount;   // 0x0
    u16 mRigidMtxCount;     // 0x2
    u32 _4;
    u16* mDrawMtxArray;     // 0x8
};

class J3DVertexData {
public:
    J3DVertexData();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u8 _30[0x4C-0x30];
    u8 _4C;
    u32 _50;
    u8 _54;
    u32 _58;
};

class J3DVertexBuffer {
public:
    ~J3DVertexBuffer();

    void init();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};