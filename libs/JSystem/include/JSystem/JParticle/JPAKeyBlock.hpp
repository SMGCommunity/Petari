#pragma once

#include <revolution/types.h>

struct JPAKeyBlock {
    JPAKeyBlock(u8 const*);
    f32 calc(f32);

    u8 getID() { return mDataStart[8]; }

    const u8* mDataStart;
    const f32* field_0x4;
};
