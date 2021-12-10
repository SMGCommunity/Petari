#pragma once

#include "JSystem/J3DGraphBase/J3DDrawPacket.h"

class J3DShape;
class J3DMtxBuffer;
class J3DModel;

class J3DShapePacket : public J3DDrawPacket {
public:
    J3DShapePacket();

    J3DShape* _28;
    J3DMtxBuffer* _2C;
    Mtx* _30;
    u32 _34;
    J3DModel* _38;
};