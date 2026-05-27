#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution/types.h>

class GhostPacket {
public:
    GhostPacket(void*, u32);

    void read(u8*, u32);
    void read(u32*);
    void read(s16*);
    void read(char**);
    void read(s8*);
    void read(TVec3Sc*);
    void read(TVec3s*);

    u32 _0;
    u8* mDataPtr;  // 0x04
    u32 mCurOffs;  // 0x08
    u32 _C;
};
