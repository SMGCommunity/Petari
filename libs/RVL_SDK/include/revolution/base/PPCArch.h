#ifndef PPCARCH_H
#define PPCARCH_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void PPCHalt(void);

u32 PPCMfhid0(void);
u32 PPCMfhid2(void);

void PPCMthid2(u32);

u32 PPCMfl2cr(void);
u32 PPCMfmsr(void);
void PPCMtmsr(u32);

u32 PPCMfl2cr(void);
void PPCMtl2cr(u32);

#ifdef __cplusplus
}
#endif

#endif // PPCARCH_H