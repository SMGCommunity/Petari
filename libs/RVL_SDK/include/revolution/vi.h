#ifndef VI_H
#define VI_H

#include <revolution/vi/vifuncs.h>
#include <revolution/vi/vitypes.h>

typedef struct {
    u8 equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8 bs1;
    u8 bs2;
    u8 bs3;
    u8 bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8 hsy;
    u8 hcs;
    u8 hce;
    u8 hbe640;
    u8 hbs640;
    u8 hbeCCIR656;
    u16 hbsCCIR656;
} timing_s;

void VIResetDimmingCount();

u32 VIGetTvFormat();

u32 VIGetCurrentLine();

#endif // VI_H