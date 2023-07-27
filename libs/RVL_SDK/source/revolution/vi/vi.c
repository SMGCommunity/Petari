#include <revolution/os.h>
#include <revolution/vi.h>
#include "private/flipper.h"

static volatile u32 retraceCount;
static volatile u32 flushFlag;
static volatile u32 flushFlag3in1;
static OSThreadQueue retraceQueue;

static vu32 __VIDimmingFlag_DEV_IDLE[10];
static volatile BOOL __VIDimmingState = FALSE;

#define ToPhysical(fb)  (u32)(((u32)(fb)) & 0x3FFFFFFF)
#define IS_LOWER_16MB(x) ((x) < 16*1024*1024)
#define ONES(x) ((1 << (x)) - 1)

#define VI_NUMREGS (sizeof(__VIRegs)/sizeof(__VIRegs[0]))

static vu32 changeMode = 0;
static vu64 changed = 0;
static u16 regs[VI_NUMREGS];

static vu32 shdwChangeMode = 0;
static vu64 shdwChanged = 0;
static u16 shdwRegs[VI_NUMREGS];

static HorVer_s HorVer;
static timing_s* CurrTiming;
static u32 CurrTvMode;

static u32 NextBufAddr;
static u32 CurrBufAddr;

static VIRetraceCallback PreCB;
static VIRetraceCallback PostCB;

static timing_s timing[] = {
    {
        6,
        240,
        24,
        25,
        3,
        2,
        12,
        13,
        12,
        13,
        520,
        519,
        520,
        519,
        525,
        429,
        64,
        71,
        105,
        162,
        373,
        122,
        412,
    },
    {
        6,
        240,
        24,
        24,
        4,
        4,
        12,
        12,
        12,
        12,
        520,
        520,
        520,
        520,
        526,
        429,
        64,
        71,
        105,
        162,
        373,
        122,
        412,
    },

    {
        5,
        287,
        35,
        36,
        1,
        0,
        13,
        12,
        11,
        10,
        619,
        618,
        617,
        620,
        625,
        432,
        64,
        75,
        106,
        172,
        380,
        133,
        420,
    },

    {
        5,
        287,
        33,
        33,
        2,
        2,
        13,
        11,
        13,
        11,
        619,
        621,
        619,
        621,
        624,
        432,
        64,
        75,
        106,
        172,
        380,
        133,
        420,
    },

    {
        6,
        240,
        24,
        25,
        3,
        2,
        16,
        15,
        14,
        13,
        518,
        517,
        516,
        519,
        525,
        429,
        64,
        78,
        112,
        162,
        373,
        122,
        412,
    },

    {
        6,
        240,
        24,
        24,
        4,
        4,
        16,
        14,
        16,
        14,
        518,
        520,
        518,
        520,
        526,
        429,
        64,
        78,
        112,
        162,
        373,
        122,
        412,
    },

    {
        12,
        480,
        48,
        48,
        6,
        6,
        24,
        24,
        24,
        24,
        1038,
        1038,
        1038,
        1038,
        1050,
        429,
        64,
        71,
        105,
        162,
        373,
        122,
        412,
    },

    {
        12,
        480,
        44,
        44,
        10,
        10,
        24,
        24,
        24,
        24,
        1038,
        1038,
        1038,
        1038,
        1050,
        429,
        64,
        71,
        105,
        168,
        379,
        122,
        412,
    },

    {
        6,
        241,
        24,
        25,
        1,
        0,
        12,
        13,
        12,
        13,
        520,
        519,
        520,
        519,
        525,
        429,
        64,
        71,
        105,
        159,
        370,
        122,
        412,
    },

    {
        12,
        480,
        48,
        48,
        6,
        6,
        24,
        24,
        24,
        24,
        1038,
        1038,
        1038,
        1038,
        1050,
        429,
        64,
        71,
        105,
        180,
        391,
        122,
        412,
    },

    {
        10,
        576,
        62,
        62,
        6,
        6,
        20,
        20,
        20,
        20,
        1240,
        1240,
        1240,
        1240,
        1250,
        432,
        64,
        75,
        106,
        172,
        380,
        122,
        412,
    }
};

static timing_s* timingExtra = NULL;

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

static void calcFbbs(u32 bufAddr, u16 panPosX, u16 panPosY, u8 wordPerLine, VIXFBMode xfbMode, u16 dispPosY, u32* tfbb, u32* bfbb) {
    u32 bytesPerLine, xoffInWords;
    xoffInWords = (u32)panPosX / 16;
    bytesPerLine = (u32)wordPerLine * 32;

    *tfbb = bufAddr + xoffInWords * 32 + bytesPerLine * panPosY;
    *bfbb = (xfbMode == VI_XFBMODE_SF) ? *tfbb : (*tfbb + bytesPerLine);

    if (dispPosY % 2 == 1) {
        u32 tmp = *tfbb;
        *tfbb = *bfbb;
        *bfbb = tmp;
    }

    *tfbb = ToPhysical(*tfbb);
    *bfbb = ToPhysical(*bfbb);
}

void setFbbRegs(HorVer_s* HorVer, u32* tfbb, u32* bfbb, u32* rtfbb, u32* rbfbb) {
    u32 shifted;
    calcFbbs(HorVer->bufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY, HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY, tfbb, bfbb);

    if (HorVer->threeD) {
        calcFbbs(HorVer->rbufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY, HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY, rtfbb, rbfbb);
    }

    if (IS_LOWER_16MB(*tfbb) && IS_LOWER_16MB(*bfbb) && IS_LOWER_16MB(*rtfbb) && IS_LOWER_16MB(*rbfbb)) {
        shifted = 0;
    }
    else {
        shifted = 1;
    }

    if (shifted) {
        *tfbb >>= 5;
        *bfbb >>= 5;
        *rtfbb >>= 5;
        *rbfbb >>= 5;
    }

    regs[0xF] = (u16)(*tfbb & 0xFFFF);
    changed |= (1ull << (63 - (0xF)));

    regs[0xE] = (u16)((((*tfbb >> 16))) | HorVer->xof << 8 | shifted << 12);
    changed |= (1ull << (63 - (0xE)));

    regs[0x13] = (u16)(*bfbb & 0xFFFF);
    changed |= (1ull << (63 - (0x13)));

    regs[0x12] = (u16)(*bfbb >> 16);
    changed |= (1ull << (63 - (0x12)));

    if (HorVer->threeD) {
        regs[0x11] = *rtfbb & 0xffff;
        changed |= (1ull << (63 - (0x11)));

        regs[0x10] = *rtfbb >> 16;
        changed |= (1ull << (63 - (0x10)));

        regs[0x15] = *rbfbb & 0xFFFF;
        changed |= (1ull << (63 - (0x15)));

        regs[0x16] = *rbfbb >> 16;
        changed |= (1ull << (63 - (0x14)));
    }
}

void setHorizontalRegs(timing_s* tm, u16 dispPosX, u16 dispSizeX) {
    u32 hbe, hbs, hbeLo, hbeHi;

    regs[0x3] = (u16)tm->hlw;
    changed |= (1ull << (63 - (0x3)));

    regs[2] = (u16)(tm->hce | tm->hcs << 8);
    changed |= (1ull << (63 - (0x2)));

    if (HorVer.tv == 8) {
        hbe = (u32)(tm->hbe640 + 172);
        hbs = tm->hbs640;
    }
    else {
        hbe = (u32)(tm->hbe640 - 40 + dispPosX);
        hbs = (u32)(tm->hbs640 + 40 + dispPosX - (720 - dispSizeX));
    }

    hbeLo = hbe & ONES(9);
    hbeHi = hbe >> 9;

    regs[5] = (u16)(tm->hsy | hbeLo << 7);
    changed |= (1ull << (63 - (0x05)));

    regs[4] = (u16)(hbeHi| hbs << 1);
    changed |= (1ull << (63 - (0x04)));
}

void setVerticalRegs(u16 dispPosY, u16 dispSizeY, u8 equ, u16 acv, u16 prbOdd, u16 prbEven, u16 psbOdd, u16 psbEven, BOOL black) {
    u16 actualPrbOdd, actualPrbEven, actualPsbOdd, actualPsbEven, actualAcv, c, d;

    if ((HorVer.nonInter == 2) || (HorVer.nonInter == 3)) {
        c = 1;
        d = 2;
    }
    else {
        c = 2;
        d = 1;
    }

    if (dispPosY % 2 == 0) {
        actualPrbOdd = (u16)(prbOdd + d * dispPosY);
        actualPsbOdd = (u16)(psbOdd + d * ((c * acv - dispSizeY) - dispPosY));
        actualPrbEven = (u16)(prbEven + d * dispPosY);
        actualPsbEven = (u16)(psbEven + d * ((c * acv - dispSizeY) - dispPosY));
    }
    else {
        actualPrbOdd = (u16)(prbEven + d * dispPosY);
        actualPsbOdd = (u16)(psbEven + d * ((c * acv - dispSizeY) - dispPosY));
        actualPrbEven = (u16)(prbOdd + d * dispPosY);
        actualPsbEven = (u16)(psbOdd + d * ((c * acv - dispSizeY) - dispPosY));
    }

    actualAcv = (u16)(dispSizeY / c);

    if (black) {
        actualPrbOdd += 2 * actualAcv - 2;
        actualPsbOdd += 2;
        actualPrbEven += 2 * actualAcv - 2;
        actualPsbEven += 2;
        actualAcv = 0;
    }

    regs[0] = (u16)(equ | actualAcv << 4);
    changed |= (1ull << (63 - (0x00)));

    regs[7] = (u16)actualPrbOdd;
    changed |= (1ull << (63 - (0x07)));

    regs[6] = (u16)actualPsbOdd;
    changed |= (1ull << (63 - (0x06)));

    regs[9] = (u16)actualPrbEven;
    changed |= (1ull << (63 - (0x09)));

    regs[8] = (u16)actualPsbEven;
    changed |= (1ull << (63 - (0x08)));
}

static u32 getCurrentHalfLine(void) {
    u32 hcount;
    u32 vcount;

    GetCurrentDisplayPosition(&hcount, &vcount);

    return ((vcount - 1) << 1) + ((hcount - 1) / CurrTiming->hlw);
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
    BOOL enabled;
    VIRetraceCallback oldcb = PreCB;
    enabled = OSDisableInterrupts();
    PreCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}


VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) {
    BOOL enabled;
    VIRetraceCallback oldcb = PostCB;
    enabled = OSDisableInterrupts();
    PostCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static timing_s* getTiming(VITVMode mode) {
    switch (mode) {
      case VI_TVMODE_NTSC_INT:
        return &timing[0];
        break;
      case VI_TVMODE_NTSC_DS:
        return &timing[1];
        break;
      case VI_TVMODE_PAL_INT:
        return &timing[2];
        break;
      case VI_TVMODE_PAL_DS:
        return &timing[3];
        break;
      case VI_TVMODE_EURGB60_INT:
        return &timing[0];
        break;
      case VI_TVMODE_EURGB60_DS:
        return &timing[1];
        break;
      case VI_TVMODE_MPAL_INT:
        return &timing[4];
        break;
      case VI_TVMODE_MPAL_DS:
        return &timing[5];
        break;
      case VI_TVMODE_NTSC_PROG:
      case VI_TVMODE_MPAL_PROG:
      case VI_TVMODE_EURGB60_PROG:
        return &timing[6];
        break;
      case VI_TVMODE_NTSC_3D:
        return &timing[7];
        break;
      case VI_TVMODE_DEBUG_PAL_INT:
        return &timing[2];
        break;
      case VI_TVMODE_DEBUG_PAL_DS:
        return &timing[3];
        break;
      case VI_TVMODE_GCA_INT:
        return &timing[8];
        break;
      case VI_TVMODE_GCA_PROG:
        return &timing[9];
        break;
      case VI_TVMODE_PAL_PROG:
        return &timing[10];
        break;
      case VI_TVMODE_EXTRA_INT:
      case VI_TVMODE_EXTRA_DS:
      case VI_TVMODE_EXTRA_PROG:
      case VI_TVMODE_HD720_PROG:
        return timingExtra;
        break;
    }

    return NULL;
}


void VIWaitForRetrace(void) {
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = retraceCount;

    do {
        OSSleepThread(&retraceQueue);
    } while (count == retraceCount);

    OSRestoreInterrupts(enabled);
}

void VISetBlack(BOOL black) {
    BOOL enabled;
    timing_s* tm;

    enabled = OSDisableInterrupts();
    HorVer.black = black;
    tm = HorVer.timing;
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.DispSizeY, tm->equ, tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
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

static s32 cntlzd(u64 bit) {
    u32 hi, lo;
    s32 value;

    hi = (u32)(bit >> 32);
    lo = (u32)(bit & 0xFFFFFFFF);
    value = __cntlzw(hi);

    if (value < 32) {
        return value;
    }

    return (32 + __cntlzw(lo));
}

void VIFlush(void) {
    BOOL enabled;
    s32 regIndex;

    enabled = OSDisableInterrupts();
    shdwChangeMode |= changeMode;
    changeMode = 0;
    shdwChanged |= changed;

    while (changed) {
        regIndex = cntlzd(changed);
        shdwRegs[regIndex] = regs[regIndex];
        changed &= ~(1ull << (63 - (regIndex)));
    }

    flushFlag = 1;
    flushFlag3in1 = 1;
    NextBufAddr = HorVer.bufAddr;
    OSRestoreInterrupts(enabled);
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

BOOL __VIResetDev0Idle(void) {
    __VIDimmingFlag_DEV_IDLE[0] = 0;
    return TRUE;
}

BOOL VIResetDimmingCount(void) {
    return __VIResetDev0Idle();
}