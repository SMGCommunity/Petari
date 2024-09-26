#pragma once

#include <revolution.h>

enum JPAEmitterFlag {
    JPA_EMITTER_PAUSE_FLAG =    1 << 1,
    JPA_EMITTER_INIT_FLAG =     1 << 6
};

class JPAEmitterManager {
public:

    void calc(u8);

    u32 _0[0x3D];   // 0x0
    s32 flags;      // 0xF4
};