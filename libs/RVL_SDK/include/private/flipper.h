#ifndef FLIPPER_H
#define FLIPPER_H

#include <revolution/types.h>

#ifdef __MWERKS__
vu16 __VIRegs[0x3B] : 0xCC002000;
vu32 __PIRegs[0xC] : 0xCC003000;
vu16 __DSPRegs[0x20] : 0xCC005000;
vu32 __AIRegs[0x8] : 0xCD006C00;
vu32 __EXIRegs[0x10] : 0xCD006800;
vu16 __MEMRegs[0x40] : 0xCC004000;
vu32 __DIRegs[16] : 0xCD006000;
vu32 __SIRegs[64] : 0xCD006400;
vu32 __ACRRegs[89] : 0xCD000000;
vu32 __IPCRegs[4] : (0xC000 << 16) + 0x0d000000;
#else
vu16 __VIRegs[0x3B];
vu32 __PIRegs[0xC];
vu16 __DSPRegs[0x20];
vu32 __AIRegs[0x8];
vu32 __EXIRegs[0x10];
vu16 __MEMRegs[0x40];
vu32 __DIRegs[16];
vu32 __SIRegs[64];
vu32 __ACRRegs[89];
vu32 __IPCRegs[4];
#endif

inline void ACRWriteReg(u32 offset, u32 val) {
    __ACRRegs[offset >> 2] = val;
}

inline u32 ACRReadReg(u32 offset) {
    return __ACRRegs[offset >> 2];
}

#endif  // FLIPPER_H
