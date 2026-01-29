#pragma once

#include <revolution/types.h>

class JASProbe {
public:
    void start(char const*);
    void stop();
    static void start(s32, char const*);
    static void stop(s32);

    static JASProbe* sProbeTable[16];

    /* 0x000 */ char const* mName;
    /* 0x004 */ u32 mStartTime;
    /* 0x008 */ f32 _8;
    /* 0x00C */ f32 _C;
    /* 0x010 */ f32 _10;
    /* 0x014 */ f32 _14;
    /* 0x018 */ f32 _18[100];
    /* 0x1A8 */ u32 _1A8;
};
