#pragma once

#include <revolution.h>

class JAIStream;

class AudBgmTempoAdjuster {
public:
    AudBgmTempoAdjuster();

    void init();
    void setInfo(f32, u32, u32, u32, u32);
    void setStream(JAIStream*);
    void reject();
    f32 beatProc(f32);
    f32 calcAdjustRate(f32, f32);

    JAIStream* mStream;  // 0x0
    f32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    f32 _18;
    u32 _1C;
    u32 _20;
    bool _24;
    bool _25;
};