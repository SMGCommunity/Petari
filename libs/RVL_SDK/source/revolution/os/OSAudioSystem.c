#include "private/flipper.h"
#include <revolution/os.h>

// __AIClockInit
// __OSInitAudioSystem

void __OSStopAudioSystem(void) {
    u16 reg16;
    OSTick start_tick;

    __DSPRegs[5] = 0x804;
    reg16 = __DSPRegs[0x1B];
    __DSPRegs[0x1B] = (u16)(reg16 & ~0x8000);

    reg16 = __DSPRegs[5];
    while (reg16 & 0x400) {
        reg16 = __DSPRegs[5];
    }

    reg16 = __DSPRegs[5];
    while (reg16 & 0x200) {
        reg16 = __DSPRegs[5];
    }

    __DSPRegs[5] = 0x8AC;
    __DSPRegs[0] = 0;

    while ((__DSPRegs[2] << 16 | __DSPRegs[3]) & 0x80000000);

    start_tick = OSGetTick();
    while (OSDiffTick(OSGetTick(), start_tick) < 44);

    reg16 = __DSPRegs[5];
    __DSPRegs[5] = (u16)(reg16 | 1);
    reg16 = __DSPRegs[5];

    while (reg16 & 1) {
        reg16 = __DSPRegs[5];
    }
}