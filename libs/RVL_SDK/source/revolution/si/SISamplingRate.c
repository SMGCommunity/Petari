#include <revolution/si.h>
#include <revolution/vi.h>
#include <private/flipper.h>

static u32 SamplingRate;

typedef struct XY {
    u16 line;
    u8 count;
} XY;

static struct XY XYNTSC[12] = {
    246,        2,
    14,         19,
    30,         9,
    44,         6,
    52,         5,
    65,         4,
    87,         3,
    87,         3,
    87,         3,
    131,        2,
    131,        2,
    131,        2,
};

static struct XY XYPAL[12] = {
    296,        2,
    15,         21,
    29,         11,
    45,         7,
    52,         6,
    63,         5,
    78,         4,
    104,        3,
    104,        3,
    104,        3,
    104,        3,
    156,        2,
};

void SISetSamplingRate(u32 msec) {
    XY* xy;
    BOOL progressive;
    BOOL enabled;

    if (11 < msec) {
        msec = 11;
    }

    enabled = OSDisableInterrupts();
    SamplingRate = msec;

    switch (VIGetTvFormat()) {
        case VI_NTSC:
        case VI_MPAL:
        case VI_EURGB60:
            xy = XYNTSC;
            break;
        case VI_PAL:
            xy = XYPAL;
            break;
        default:
            OSReport("SISetSamplingRate: unknown TV format. Use default.");
            msec = 0;
            xy = XYNTSC;
            break;
    }

    progressive = __VIRegs[0x36] & 1;
    SISetXY((progressive ? 2 : 1) * xy[msec].line, xy[msec].count);
    OSRestoreInterrupts(enabled);
}

void SIRefreshSamplingRate(void) {
    SISetSamplingRate(SamplingRate);
}