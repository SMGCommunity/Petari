#pragma once

#include <revolution.h>

class MainLoopFramework {
public:

    void setTickRateFromFrame(u16);

    static MainLoopFramework* get() {
        return MainLoopFramework::sManager;
    }

    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    u32 _4;
    u16 _8;
    u16 _A;
    u32 _C;
    u16 _10;
    u32 _14;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    f32 _1C;
    OSTick mTick;          // 0x20
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u8 _34;
    u8 _35;
    u8 _36;
    u8 _37;
    u32 _38;
    u16 _3C;
    u8 _3E;

    static MainLoopFramework* sManager;
};
