#ifndef AI_H
#define AI_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AIDCallback)(void);

#define SET_FLAG(reg, value, mask, shift) reg = reg & ~mask | value << shift;

#define GET_FLAG(reg, mask, shift) ((reg & mask) >> shift)

AIDCallback AIRegisterDMACallback(AIDCallback);
void AIInitDMA(u32, u32);
void AIStartDMA(void);
void AIStopDMA(void);
u32 AIGetDMAStartAddr(void);
u32 AIGetDMALength(void);
void AISetDSPSampleRate(u32);
void AIInit(u8*);

#ifdef __cplusplus
}
#endif

#endif  // AI_H
