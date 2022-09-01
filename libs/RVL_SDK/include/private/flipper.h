#ifndef FLIPPER_H
#define FLIPPER_H

#include <revolution/types.h>

vu32 __PIRegs[0xC] : (0xC000 << 16) + 0x3000;

vu16 __DSPRegs[0x20] : (0xC000 << 16) + 0x5000;

vu32 __AIRegs [0x8] : (0xC000 << 16) + 0x06C00;

#endif // FLIPPER_H