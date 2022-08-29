#include <revolution/os.h>
#include <revolution/os/OSInterrupt.h>
#include <private/flipper.h>
#include <mem.h>

static __OSInterruptHandler* InterruptHandlerTable;

// forward decl for init
static asm void ExternalInterruptHandler( register __OSException, register OSContext *);
 
BOOL OSDisableInterrupts(void) {
    asm {
        mfmsr r3
        rlwinm r4, r3, 0, 17, 15
        mtmsr r4
        extrwi r3, r3, 1, 16
    }
}

BOOL OSEnableInterrupts(void) {
    asm {
        mfmsr r3
        ori r4, r3, 0x8000
        mtmsr r4
        extrwi r3, r3, 1, 16
    }
}
 
BOOL OSRestoreInterrupts(register BOOL level) {
    asm {
        cmpwi level, 0
        mfmsr r4
        beq disable
        ori r5, r4, 0x00008000
        b restore

    disable:
        rlwinm r5, r4, 0, 17, 15

    restore:
        mtmsr r5
        extrwi r3, r4, 1, 16
    }
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt) {
    return InterruptHandlerTable[interrupt];
}

OSInterruptMask __OSMaskInterrupts(OSInterruptMask new_mask) {
    BOOL v2;
    OSInterruptMask user_interrupt;
    OSInterruptMask os_interrupt;
    OSInterruptMask mask;

    v2 = OSDisableInterrupts();
    // load the user interrupt mask
    user_interrupt = *(OSInterruptMask*)0x800000C4;
    os_interrupt = *(OSInterruptMask*)0x800000C8;
    mask = ~(user_interrupt | os_interrupt) & new_mask;
    // set the new user interrupt mask
    new_mask |= user_interrupt;
    *(OSInterruptMask*)0x800000C4 = new_mask;

    while (mask) {
        mask = SetInterruptMask(mask, new_mask | os_interrupt);
    }

    OSRestoreInterrupts(v2);
    return user_interrupt;
} 

void __OSInterruptInit(void) {
    // set our interrupt handler table to the __OSInterruptTable
    InterruptHandlerTable = (void*)0x80003040;
    memset(InterruptHandlerTable, 0, 32 * sizeof(__OSInterruptHandler));
    *(OSInterruptMask*)0x800000C4 = 0;
    *(OSInterruptMask*)0x800000C8 = 0;
    *(int*)0xCC003004 = 240;
    *(int*)0xCD000034 = 0x40000000;
    
    // this function call is inlined
    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM | OS_INTERRUPTMASK_DSP | OS_INTERRUPTMASK_AI | OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI);
    __OSSetExceptionHandler(4, ExternalInterruptHandler);
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask new_mask) {
    BOOL v2;
    OSInterruptMask user_interrupt;
    OSInterruptMask os_interrupt;
    OSInterruptMask mask;

    v2 = OSDisableInterrupts();
    user_interrupt = *(OSInterruptMask*)0x800000C4;
    os_interrupt = *(OSInterruptMask*)0x800000C8;
    mask = (user_interrupt | os_interrupt) & new_mask;
    new_mask = user_interrupt & ~new_mask;
    *(OSInterruptMask*)0x800000C4 = new_mask;

    while (mask) {
        mask = SetInterruptMask(mask, new_mask | os_interrupt);
    }

    OSRestoreInterrupts(v2);
    return user_interrupt;
}

static asm void ExternalInterruptHandler(register __OSException, register OSContext *pContext) {
    nofralloc
    stw r0, OS_CONTEXT_R0(pContext)
    stw r1, OS_CONTEXT_R1(pContext)
    stw r2, OS_CONTEXT_R2(pContext)
    stmw r6, OS_CONTEXT_R6(pContext)
    mfspr   r0, GQR1
    stw r0, OS_CONTEXT_GQR1(pContext)
    mfspr   r0, GQR2
    stw r0, OS_CONTEXT_GQR2(pContext)
    mfspr   r0, GQR3
    stw r0, OS_CONTEXT_GQR3(pContext)
    mfspr   r0, GQR4
    stw r0, OS_CONTEXT_GQR4(pContext)
    mfspr   r0, GQR5
    stw r0, OS_CONTEXT_GQR5(pContext)
    mfspr   r0, GQR6
    stw r0, OS_CONTEXT_GQR6(pContext)
    mfspr   r0, GQR7
    stw r0, OS_CONTEXT_GQR7(pContext)
    stwu r1, -8(r1)
    b __OSDispatchInterrupt
}