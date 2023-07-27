#ifndef OSRTC_H
#define OSRTC_H

#include <revolution/types.h>

typedef struct OSSram {
    u16 checkSum;
    u16 checkSumInv;
    u32 ead0;
    u32 ead1;
    u32 counterBias;
    s8 displayOffsetH;
    u8 ntd;
    u8 language;
    u8 flags;
} OSSram;

typedef struct OSSramEx {
    u8 flashID[2][12];
    u32 wirelessKeyboardID;
    u16 wirelessPadID[4];
    u8 dvdErrorCode;
    u8 _pad;
    u8 flashIDCheckSum[2];
    u16 gbs;
    u8 _pad1[2];
} OSSramEx;

u16 OSGetGbsMode(void);
void OSSetGbsMode(u16);

u16 OSGetWirelessID(s32);
void OSSetWirelessID(s32, u16);

BOOL __OSReadROM(void *, s32, s32);
BOOL __OSGetRTCFlags(u32 *);
BOOL __OSClearRTCFlags(void);

void __OSInitSram(void);

#endif // OSRTC_H