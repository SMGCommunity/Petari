#include <revolution/os.h>
#include <revolution/os/OSReset.h>
#include <private/flipper.h>

extern OSErrorHandler __OSErrorTable[];
static BOOL OnShutdown( BOOL final, u32 event );

static OSShutdownFunctionInfo ShutdownFunctionInfo = {
    OnShutdown,
    127,
    0,
    0
};

#define TRUNC(n, a) (((u32) (n)) & ~((a) - 1))
#define ROUND(n, a) (((u32) (n) + (a) - 1) & ~((a) - 1))

u32 OSGetPhysicalMem1Size(void) {
    return *(u32*)OSPhysicalToCached(0x3100);
}

u32 OSGetPhysicalMem2Size(void) {
    return *(u32*)OSPhysicalToCached(0x3118);
}

u32 OSGetConsoleSimulatedMem1Size(void) {
    return *(u32*)OSPhysicalToCached(0x3104);
}

u32 OSGetConsoleSimulatedMem2Size(void) {
    return *(u32*)OSPhysicalToCached(0x311C);
}

static BOOL OnShutdown(BOOL final, u32 event) {
    if (final != FALSE) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(0xF0000000);
    }

    return TRUE;
}

void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext *context) {
    u32 addr, cause;
    cause = __MEMRegs[0xF];
    addr = (((u32)(__MEMRegs[0x12] & 0x3FF) << 16) | __MEMRegs[0x11]);
    __MEMRegs[0x10] = 0;

    if (__OSErrorTable[15]) {
        __OSErrorTable[15](15, context, cause, addr);
        return;
    }

    __OSUnhandledException(15, context, cause, addr);
}

void OSProtectRange(u32 chan, void *addr, u32 nBytes, u32 control) {
    BOOL enabled;
    u32 start, end;
    u16 reg;

    if (4 <= chan) {
        return;
    }

    control &= 3;
    end = (u32)addr + nBytes;
    start = TRUNC(addr, 1 << 10);
    end = ROUND(end, 1 << 10);
    DCFlushRange((void*)start, end - start);
    enabled = OSDisableInterrupts();
    __OSMaskInterrupts(0x80000000u >> (chan));
    __MEMRegs[0 + 2 * chan] = (u16)(start >> 10);
    __MEMRegs[1 + 2 * chan] = (u16)(end >> 10);
    reg = __MEMRegs[8];
    reg &= ~(3 << 2 * chan);
    reg |= control << 2 * chan;
    __MEMRegs[8] = reg;

    if (control != 3) {
        __OSUnmaskInterrupts(0x80000000u >> (chan));
    }

    OSRestoreInterrupts(enabled);
}

static asm void ConfigMEM1_24MB(void) {
    nofralloc
    addi r7, r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x800001ff@ha
    addi    r3,r3,0x800001ff@l

    addis   r6,r0,0x01000002@ha
    addi    r6,r6,0x01000002@l
    addis   r5,r0,0x810000ff@ha
    addi    r5,r5,0x810000ff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mtspr   dbat2u,r7
    mtspr   dbat2l,r6
    mtspr   dbat2u,r5
    isync

    mtspr   ibat2u,r7
    mtspr   ibat2l,r6
    mtspr   ibat2u,r5
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr r3
    mtsrr0 r3
    rfi
}

static asm void ConfigMEM1_48MB(void)
{
    nofralloc

    addi    r7,r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x800003ff@ha
    addi    r3,r3,0x800003ff@l

    addis   r6,r0,0x02000002@ha
    addi    r6,r6,0x02000002@l
    addis   r5,r0,0x820001ff@ha
    addi    r5,r5,0x820001ff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mtspr   dbat2u,r7
    mtspr   dbat2l,r6
    mtspr   dbat2u,r5
    isync

    mtspr   ibat2u,r7
    mtspr   ibat2l,r6
    mtspr   ibat2u,r5
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM2_52MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900003ff@ha
    addi    r3,r3,0x900003ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r4
    mtspr   IBAT4U,r3

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x12000002@ha
    addi    r4,r4,0x12000002@l
    addis   r3,r0,0x920001ff@ha
    addi    r3,r3,0x920001ff@l

    addis   r6,r0,0x13000002@ha
    addi    r6,r6,0x13000002@l
    addis   r5,r0,0x9300007f@ha
    addi    r5,r5,0x9300007f@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r4
    mtspr   IBAT6U,r3

    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r6
    mtspr   IBAT7U,r5

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM2_56MB(void) {
    nofralloc

    addi    r7,r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900003ff@ha
    addi    r3,r3,0x900003ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r4
    mtspr   IBAT4U,r3

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x12000002@ha
    addi    r4,r4,0x12000002@l
    addis   r3,r0,0x920001ff@ha
    addi    r3,r3,0x920001ff@l

    addis   r6,r0,0x13000002@ha
    addi    r6,r6,0x13000002@l
    addis   r5,r0,0x930000ff@ha
    addi    r5,r5,0x930000ff@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r4
    mtspr   IBAT6U,r3

    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r6
    mtspr   IBAT7U,r5

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM2_64MB(void) {
    nofralloc

    addi    r7,r0,0x0000

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900007ff@ha
    addi    r3,r3,0x900007ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r4
    mtspr   IBAT4U,r3

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7
    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r7
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r7
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM2_112MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900007ff@ha
    addi    r3,r3,0x900007ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD0000fff@ha
    addi    r5,r5,0xD0000fff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r4
    mtspr   IBAT4U,r3

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x14000002@ha
    addi    r4,r4,0x14000002@l
    addis   r3,r0,0x940003ff@ha
    addi    r3,r3,0x940003ff@l

    addis   r6,r0,0x16000002@ha
    addi    r6,r6,0x16000002@l
    addis   r5,r0,0x960001ff@ha
    addi    r5,r5,0x960001ff@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r4
    mtspr   IBAT6U,r3
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r6
    mtspr   IBAT7U,r5

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM2_128MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x90000fff@ha
    addi    r3,r3,0x90000fff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD0000fff@ha
    addi    r5,r5,0xD0000fff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r4
    mtspr   IBAT4U,r3

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7
    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r7
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r7
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void ConfigMEM_ES1_0(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x80000fff@ha
    addi    r3,r3,0x80000fff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void RealMode(register u32 addr) {
    nofralloc

    clrlwi r3, r3, 2
    mtsrr0 addr
    mfmsr r3
    rlwinm r3, r3, 0, 28, 25
    mtsrr1 r3
    rfi
}

static void BATConfig(void) {
    u32 size1, size2, prot;

    if (*(u32*)OSPhysicalToCached(0x3138) == 0) {
        if (OSGetPhysicalMem1Size == 0) {
            RealMode((u32)ConfigMEM_ES1_0);
            return;
        }
    }

    size1 = OSGetConsoleSimulatedMem1Size();

    if (size1 < OSGetPhysicalMem1Size() && size1 == 25165824) {
        DCInvalidateRange((void*)0x81800000, 25165824);
        __MEMRegs[0x14] = 2;
    }

    if (size1 <= 25165824) {
        RealMode((u32)ConfigMEM1_24MB);
    }
    else if (size1 <= 50331648) {
        RealMode((u32)ConfigMEM1_48MB);
    }

    size2 = OSGetConsoleSimulatedMem2Size();
    prot = *(u32*)OSPhysicalToCached(0x3120);

    if (size2 <= 67108864) {
        if (prot <= 0x93400000) {
            RealMode((u32)ConfigMEM2_52MB);
        }
        else if (prot <= 0x93800000) {
            RealMode((u32)ConfigMEM2_56MB);
        }
        else {
            RealMode((u32)ConfigMEM2_64MB);
        }
    }
    else if (size2 <= 134217728) {
        if (prot <= 0x97000000) {
            RealMode((u32)ConfigMEM2_112MB);
        }
        else {
            RealMode((u32)ConfigMEM2_128MB);
        }
    }
}

void __OSInitMemoryProtection(void) {
    static BOOL initialized = FALSE;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __MEMRegs[0x10] = 0;
    __MEMRegs[0x8] = 0xFF;
    __OSMaskInterrupts(0xF0000000);
    __OSSetInterruptHandler(0, MEMIntrruptHandler);
    __OSSetInterruptHandler(1, MEMIntrruptHandler);
    __OSSetInterruptHandler(2, MEMIntrruptHandler);
    __OSSetInterruptHandler(3, MEMIntrruptHandler);
    __OSSetInterruptHandler(4, MEMIntrruptHandler);
    OSRegisterShutdownFunction(&ShutdownFunctionInfo);
    BATConfig();
    __OSUnmaskInterrupts(0x8000000);
    OSRestoreInterrupts(enabled);
}
