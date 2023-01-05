#ifndef EXI_H
#define EXI_H

#include "revolution/os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*EXICallback)(s32 chan, OSContext* context);

BOOL EXIDma(s32, void *, s32, u32, EXICallback);

u32 EXIClearInterrupts(s32, BOOL, BOOL, BOOL);

#ifdef __cplusplus
}
#endif

#endif // EXI_H