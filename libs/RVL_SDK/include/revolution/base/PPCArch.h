#ifndef PPCARCH_H
#define PPCARCH_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 PPCMfmsr(void);
void PPCMtmsr(u32);
u32 PPCMfhid0(void);
void PPCMthid0(u32);
u32 PPCMfl2cr(void);
void PPCMtl2cr(u32);
void PPCMtdec(u32);
void PPCSync();
void PPCHalt(void);
void PPCMtmmcr0(u32);
void PPCMtmmcr1(u32);
void PPCMtpmc1(u32);
void PPCMtpmc2(u32);
void PPCMtpmc3(u32);
void PPCMtpmc4(u32);
u32 PPCMffpscr(void);
void PPCMtfpscr(u32);
u32 PPCMfhid2(void);
void PPCMthid2(u32);
u32 PPCMfwpar(void);
void PPCMtwpar(u32);
void PPCDisableSpeculation(void);
void PPCSetFpNonIEEEMode(void);
void PPCMthid4(u32);

#ifdef __cplusplus
}
#endif

#endif // PPCARCH_H