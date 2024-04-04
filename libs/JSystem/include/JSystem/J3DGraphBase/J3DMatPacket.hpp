#pragma once

#include "JSystem/J3DGraphBase/J3DDrawPacket.hpp"

class J3DMaterial;
class J3DShapePacket;

class J3DMatPacket : public J3DDrawPacket {
public:
    J3DMatPacket();

    J3DShapePacket* _28;
    J3DShapePacket* _2C;
    J3DMaterial* mMaterial; // _30
    u32 mFlags;             // _34
    s32 _38;
    s32 _3C;
};

class J3DFog { 
public:
    void load() const;

    u8 _0;
    u8 _1;
    u16 _2;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    GXColor mColor;                     // _14
    GXFogAdjTable mFogTable;            // _18
};