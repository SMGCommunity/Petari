#pragma once

#include "JSystem/J3DGraphBase/J3DPacket.hpp"

class J3DDrawPacket : public J3DPacket {
public:
    J3DDrawPacket();

    virtual void draw();
    virtual ~J3DDrawPacket();

    u32 _C;
    u32 mFlags;             // 0x10
    u32 _14;
    u32 _18;
    u32 _1C;
    J3DDisplayListObj* mDL; // 0x20
    u32 _24;
};