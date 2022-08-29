#pragma once

#include "JSystem/J3DGraphBase/J3DPacket.h"

class J3DDrawPacket : public J3DPacket {
public:
    J3DDrawPacket();

    virtual void draw();
    virtual ~J3DDrawPacket();

    u32 _C;
    u32 mFlags;             // _10
    u32 _14;
    u32 _18;
    u32 _1C;
    J3DDisplayListObj* mDL; // _20
    u32 _24;
};