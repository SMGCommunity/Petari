#ifndef OSINTERRUPT_H
#define OSINTERRUPT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

u32 OSDisableInterrupts();
u32 OSEnableInterrupts();
u32 OSRestoreInterrupts(u32);
u32 OSIsEnableInterrupts();

#ifdef __cplusplus
}
#endif

#endif // OSINERRUPT_H