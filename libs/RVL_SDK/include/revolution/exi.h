#ifndef EXI_H
#define EXI_H

#include "revolution/os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*EXICallback)(s32 chan, OSContext* context);

BOOL EXISync(s32);
BOOL EXIImm(s32, void *, s32, u32, EXICallback);
BOOL EXIDma(s32, void *, s32, u32, EXICallback);
BOOL EXIAttach(s32, EXICallback);
BOOL EXIDetach(s32);
BOOL EXISelect(s32, u32, u32);
BOOL EXIDeselect(s32);
BOOL EXILock(s32, u32, EXICallback);
BOOL EXIUnlock(s32);
BOOL EXIImmEx(s32, void *, s32, u32);

s32 EXIGetID(s32, u32, u32 *);

u32 EXIClearInterrupts(s32, BOOL, BOOL, BOOL);

void __OSEnableBarnacle(s32, u32);

#ifdef __cplusplus
}
#endif

#endif // EXI_H