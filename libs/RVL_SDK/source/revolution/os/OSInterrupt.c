#include <revolution/os.h>
#include <revolution/os/OSInterrupt.h>
#include <private/flipper.h>
#include <mem.h>

static __OSInterruptHandler* InterruptHandlerTable;

static OSInterruptMask InterruptPrioTable[] =
{
    0x100,
    0x40,
    0xF800000000,
    0x200,
    0x80,
    0x10,
    0x3000,
    0x20,
    0x3FF8C00,
    0x4000000,
    0x4000,
    0xFFFFFFFF
};

// forward decl for init
static asm void ExternalInterruptHandler( register __OSException, register OSContext *);

static OSInterruptMask SetInterruptMask(OSInterruptMask, OSInterruptMask);
 
asm BOOL OSDisableInterrupts(void) {
    nofralloc
entry __RAS_OSDisableInterrupts_begin
    mfmsr r3
    rlwinm r4, r3, 0, 17, 15
    mtmsr r4
entry    __RAS_OSDisableInterrupts_end
    extrwi r3, r3, 1, 16
    blr
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

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler) {
    __OSInterruptHandler oldHandler;
    oldHandler = InterruptHandlerTable[interrupt];
    InterruptHandlerTable[interrupt] = handler;
    return oldHandler;
}

__OSInterruptHandler __OSGetInterruptHandler(s32 interrupt) {
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

static OSInterruptMask SetInterruptMask(OSInterruptMask mask, OSInterruptMask current) {
    u32 reg;

    switch(__cntlzw(mask)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            reg = 0;

            if (!(current & 0x80000000)) {
                reg |= 1;
            }

            if (!(current & 0x40000000)) {
                reg |= 2;
            }

            if (!(current & 0x20000000)) {
                reg |= 4;
            }

            if (!(current & 0x10000000)) {
                reg |= 8;
            }

            if (!(current & 0x8000000)) {
                reg |= 16;
            }

            __MEMRegs[0xE] = (u16)reg;
            mask &= 0x7FFFFFF;
            break;

        case 5:
        case 6:
        case 7:
            reg = __DSPRegs[5];
            reg &= 0xFFFFFE07;

            if (!(current & 0x4000000)) {
                reg |= 0x10;
            }

            if (!(current & 0x2000000)) {
                reg |= 0x40;
            }

            if (!(current & 0x1000000)) {
                reg |= 0x100;
            }
            
            __DSPRegs[5] = (u16)reg;
            mask &= 0xF8FFFFFF;
            break;

        case 8:
            reg = __AIRegs[0];
            reg &= 0xFFFFFFD3;

            if (!(current & 0x800000)) {
                reg |= 4;
            }

            __AIRegs[0] = reg;
            mask &= 0xFF7FFFFF;
            break;

        case 9:
        case 10:
        case 11:
            reg = __EXIRegs[0];
            reg &= 0xFFFFD3F0;

            if (!(current & 0x400000)) {
                reg |= 1;
            }

            if (!(current & 0x200000)) {
                reg |= 4;
            }

            if (!(current & 0x100000)) {
                reg |= 0x400;
            }

            __EXIRegs[0] = reg;
            mask &= 0xFF8FFFFF;
            break;

        case 12:
        case 13:
        case 14:
            reg = __EXIRegs[5];
            reg &= 0xFFFFF3F0;

            if (!(current & 0x80000)) {
                reg |= 1;
            }

            if (!(current & 0x40000)) {
                reg |= 4;
            }

            if (!(current & 0x20000)) {
                reg |= 0x400;
            }

            __EXIRegs[5] = reg;
            mask &= 0xFFF1FFFF;
            break;

        case 15:
        case 16:
            reg = __EXIRegs[0xA];
            reg &= 0xFFFFFFF0;

            if (!(current & 0x10000)) {
                reg |= 1;
            }

            if (!(current & 0x8000)) {
                reg |= 4;
            }

            __EXIRegs[0xA] = reg;
            mask &= 0xFFFE7FFF;

            break;

        case 17:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 18:
        case 19:
        case 26:
        case 27:
            reg = 0xF0;

            if (!(current & 0x4000)) {
                reg |= 0x800;
            }

            if (!(current & 0x800)) {
                reg |= 8;
            }

            if (!(current & 0x400)) {
                reg |= 4;
            }

            if (!(current & 0x200)) {
                reg |= 2;
            }

            if (!(current & 0x100)) {
                reg |= 1;
            }

            if (!(current & 0x80)) {
                reg |= 0x100;
            }

            if (!(current & 0x40)) {
                reg |= 0x1000;
            }

            if (!(current & 0x2000)) {
                reg |= 0x200;
            }

            if (!(current & 0x1000)) {
                reg |= 0x400;
            }

            if (!(current & 0x20)) {
                reg |= 0x2000;
            }

            if (!(current & 0x10)) {
                reg |= 0x4000;
            }

            __PIRegs[1] = reg;
            mask &= 0xFFFF800F;
            break;
        
        default:
            break;
    }

    return mask;
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

volatile OSTime __OSLastInterruptTime;
volatile __OSInterrupt __OSLastInterrupt;
volatile u32 __OSLastInterruptSrr0;

void __OSDispatchInterrupt(__OSException exception, OSContext* context) {
    u32 instr, mask, reg;
    OSInterruptMask cause, unmasked;
    OSInterruptMask* prio;
    __OSInterrupt interrupt;
    __OSInterruptHandler handler;

    instr = __PIRegs[0];
    instr &= 0xFFFEFFFF;
    mask = __PIRegs[1];

    if (instr == 0 || (instr & mask) == 0) {
        OSLoadContext(context);
    }

    cause = 0;

    if (instr & 0x80) {
        reg = __MEMRegs[0xF];

        if (reg & 0x1) {
            cause |= 0x80000000;
        }

        if (reg & 0x2) {
            cause |= 0x40000000;
        }

        if (reg & 0x4) {
            cause |= 0x20000000;
        }

        if (reg & 0x8) {
            cause |= 0x10000000;
        }

        if (reg & 0x10) {
            cause |= 0x8000000;
        }
    }
    
    if (instr & 0x40) {
        reg = __DSPRegs[5];

        if (reg & 0x8) {
            cause |= 0x4000000;
        }

        if (reg & 0x20) {
            cause |= 0x2000000;
        }

        if (reg & 0x80) {
            cause |= 0x1000000;
        }
    }

    if (instr & 0x20) {
        reg = __AIRegs[0];

        if (reg & 0x8) {
            cause |= 0x800000;
        }
    }
    
    if (instr & 0x10) {
        reg = __EXIRegs[0];

        if (reg & 0x2) {
            cause |= 0x400000;
        }

        if (reg & 0x8) {
            cause |= 0x200000;
        }

        if (reg & 0x800) {
            cause |= 0x100000;
        }

        reg = __EXIRegs[5];

        if (reg & 0x2) {
            cause |= 0x80000;
        }

        if (reg & 0x8) {
            cause |= 0x40000;
        }

        if (reg & 0x800) {
            cause |= 0x20000;
        }

        reg = __EXIRegs[0xA];

        if (reg & 0x2) {
            cause |= 0x10000;
        }

        if (reg & 0x8) {
            cause |= 0x8000;
        }
    }

    if ((instr & 0x2000) != 0) {
	    cause |= 0x20;
	}


	if ((instr & 0x1000) != 0) {
	    cause |= 0x40;
	}

	if ((instr & 0x400) != 0) {
	    cause |= 0x1000;
	}

	if ((instr & 0x200) != 0) {
	    cause |= 0x2000;
	}

	if ((instr & 0x100) != 0) {
	    cause |= 0x80;
	}

	if ((instr & 8) != 0) {
	    cause |= 0x800;
	}

	if ((instr & 4) != 0) {
	    cause |= 0x400;
	}

	if ((instr & 2) != 0) {
	    cause |= 0x200;
	}

	if ((instr & 0x800) != 0) {
	    cause |= 0x4000;
	}

	if ((instr & 1) != 0) {
	    cause |= 0x100;
	}

	if ((instr & 0x4000) != 0) {
	    cause |= 0x10;
	}

    unmasked = cause & ~(*(OSInterruptMask*)OSPhysicalToCached(0xC4) | *(OSInterruptMask*)OSPhysicalToCached(0xC8));

    if (unmasked) {
        for (prio = InterruptPrioTable; ; ++prio) {
            if (unmasked & *prio) {
                interrupt = (__OSInterrupt)__cntlzw(unmasked & *prio);
                break;
            }
        }

        handler = __OSGetInterruptHandler(interrupt);

        if (handler) {
            if (4 < interrupt) {
                __OSLastInterrupt = interrupt;
                __OSLastInterruptTime = OSGetTime();
                __OSLastInterruptSrr0 = context->srr0;
            }

            OSDisableScheduler();
            handler(interrupt, context);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(context);
        }
    }

    OSLoadContext(context);
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