#ifndef PPCARCH_H
#define PPCARCH_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

u32 PPCMfmsr();
void PPCMtmsr(u32);
u32 PPCMfhid0();
void PPCMthid0(u32);
u32 PPCMfl2cr();
void PPCMtl2cr(u32);
void PPCMtdec(u32);
void PPCSync();
void PPCHalt();
void PPCMtmmcr0(u32);
void PPCMtmmcr1(u32);
void PPCMtpmc1(u32);
void PPCMtpmc2(u32);
void PPCMtpmc3(u32);
void PPCMtpmc4(u32);
u32 PPCMffpscr();
void PPCMtfpscr(u32);
u32 PPCMfhid2();
void PPCMthid2(u32);
u32 PPCMfwpar();
void PPCMtwpar(u32);
void PPCDisableSpeculation();
void PPCSetFpNonIEEEMode();
void PPCMthid4(register u32);

#ifdef __cplusplus
}
#endif

#endif // PPCARCH_H