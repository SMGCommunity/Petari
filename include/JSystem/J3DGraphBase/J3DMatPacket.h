#pragma once

#include "JSystem/J3DGraphBase/J3DDrawPacket.h"

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
