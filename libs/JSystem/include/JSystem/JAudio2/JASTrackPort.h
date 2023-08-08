#pragma once

#include <revolution/types.h>

class JASTrackPort {
public:
    s32 checkImport(u32) const;
    s32 checkExport(u32) const;

    u16 _0;
    u16 _2;
    u16 _4[0x10];
};
