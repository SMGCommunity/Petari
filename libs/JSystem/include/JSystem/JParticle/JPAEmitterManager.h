#pragma once

#include <revolution.h>

enum JPAEmitterFlag {
    JPA_EMITTER_PAUSE_FLAG =    1 << 1,
    JPA_EMITTER_INIT_FLAG =     1 << 6
};

class JPAEmitterManager {
public:
    u32 _0[0x3D];   // _0
    s32 flags;      // _F4
};