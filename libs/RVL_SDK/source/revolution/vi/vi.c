#include <revolution/os.h>
#include <revolution/vi.h>
#include "private/flipper.h"

static timing_s* CurrTiming;
static u32 CurrTvMode;

static void GetCurrentDisplayPosition(u32 *hct, u32 *vct) {
    u32 hcount;
    u32 vcount0;
    u32 vcount;

    vcount = (u32)((__VIRegs[0x16]) & 0x7FF);

    do {
        vcount0 = vcount;
        hcount = (u32)(__VIRegs[0x17] & 0x7FF);
        vcount = (u32)(__VIRegs[0x16] & 0x7FF);
    } while (vcount0 != vcount);

    *hct = hcount;
    *vct = vcount;
}

static u32 getCurrentHalfLine(void) {
    u32 hcount;
    u32 vcount;

    GetCurrentDisplayPosition(&hcount, &vcount);

    return ((vcount - 1) << 1) + ((hcount - 1) / CurrTiming->hlw);
}

u32 VIGetCurrentLine(void) {
    u32 halfLine;
    timing_s* tm;
    BOOL enabled;

    tm = CurrTiming;
    enabled = OSDisableInterrupts();
    halfLine = getCurrentHalfLine();
    OSRestoreInterrupts(enabled);

    if (halfLine >= tm->nhlines) {
        halfLine -= tm->nhlines;
    }

    return (halfLine >> 1);
}

u32 VIGetTvFormat(void) {
    u32 format;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    switch(CurrTvMode) {
        case 0:
        case 3:
        case 6:
        case 7:
        case 8:
            format = 0;
            break;
        
        case 1:
        case 4:
            format = 1;
            break;

        case 5:
        case 2:
            format = CurrTvMode;
            break;

        default:
            break;
    }

    OSRestoreInterrupts(enabled);

    return format;
}