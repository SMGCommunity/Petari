#include <revolution/base/PPCArch.h>
#include <revolution/os.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSException.h>
#include <revolution/db.h>

static asm void __OSLoadFPUContext(register u32 dummy, register OSContext *pContext) {
    nofralloc
    lhz r5, pContext->state
    clrlwi. r5, r5, 31
    beq _ret

    lfd f0, 0x190(pContext)
    mtfsf 0xFF, f0
    mfspr r5, 0x398
    rlwinm. r5, r5, 3, 31, 31
    bc 12, 2, norm_fpr

    psq_l   fp0, OS_CONTEXT_PSF0(pContext), 0, 0
    psq_l   fp1, OS_CONTEXT_PSF1(pContext), 0, 0
    psq_l   fp2, OS_CONTEXT_PSF2(pContext), 0, 0
    psq_l   fp3, OS_CONTEXT_PSF3(pContext), 0, 0
    psq_l   fp4, OS_CONTEXT_PSF4(pContext), 0, 0
    psq_l   fp5, OS_CONTEXT_PSF5(pContext), 0, 0
    psq_l   fp6, OS_CONTEXT_PSF6(pContext), 0, 0
    psq_l   fp7, OS_CONTEXT_PSF7(pContext), 0, 0
    psq_l   fp8, OS_CONTEXT_PSF8(pContext), 0, 0
    psq_l   fp9, OS_CONTEXT_PSF9(pContext), 0, 0
    psq_l   fp10, OS_CONTEXT_PSF10(pContext), 0, 0
    psq_l   fp11, OS_CONTEXT_PSF11(pContext), 0, 0
    psq_l   fp12, OS_CONTEXT_PSF12(pContext), 0, 0
    psq_l   fp13, OS_CONTEXT_PSF13(pContext), 0, 0
    psq_l   fp14, OS_CONTEXT_PSF14(pContext), 0, 0
    psq_l   fp15, OS_CONTEXT_PSF15(pContext), 0, 0
    psq_l   fp16, OS_CONTEXT_PSF16(pContext), 0, 0
    psq_l   fp17, OS_CONTEXT_PSF17(pContext), 0, 0
    psq_l   fp18, OS_CONTEXT_PSF18(pContext), 0, 0
    psq_l   fp19, OS_CONTEXT_PSF19(pContext), 0, 0
    psq_l   fp20, OS_CONTEXT_PSF20(pContext), 0, 0
    psq_l   fp21, OS_CONTEXT_PSF21(pContext), 0, 0
    psq_l   fp22, OS_CONTEXT_PSF22(pContext), 0, 0
    psq_l   fp23, OS_CONTEXT_PSF23(pContext), 0, 0
    psq_l   fp24, OS_CONTEXT_PSF24(pContext), 0, 0
    psq_l   fp25, OS_CONTEXT_PSF25(pContext), 0, 0
    psq_l   fp26, OS_CONTEXT_PSF26(pContext), 0, 0
    psq_l   fp27, OS_CONTEXT_PSF27(pContext), 0, 0
    psq_l   fp28, OS_CONTEXT_PSF28(pContext), 0, 0
    psq_l   fp29, OS_CONTEXT_PSF29(pContext), 0, 0
    psq_l   fp30, OS_CONTEXT_PSF30(pContext), 0, 0
    psq_l   fp31, OS_CONTEXT_PSF31(pContext), 0, 0

norm_fpr:
    lfd     fp0,  pContext->fpr[0]
    lfd     fp1,  pContext->fpr[1]
    lfd     fp2,  pContext->fpr[2]
    lfd     fp3,  pContext->fpr[3]
    lfd     fp4,  pContext->fpr[4]
    lfd     fp5,  pContext->fpr[5]
    lfd     fp6,  pContext->fpr[6]
    lfd     fp7,  pContext->fpr[7]
    lfd     fp8,  pContext->fpr[8]
    lfd     fp9,  pContext->fpr[9]
    lfd     fp10, pContext->fpr[10]
    lfd     fp11, pContext->fpr[11]
    lfd     fp12, pContext->fpr[12]
    lfd     fp13, pContext->fpr[13]
    lfd     fp14, pContext->fpr[14]
    lfd     fp15, pContext->fpr[15]
    lfd     fp16, pContext->fpr[16]
    lfd     fp17, pContext->fpr[17]
    lfd     fp18, pContext->fpr[18]
    lfd     fp19, pContext->fpr[19]
    lfd     fp20, pContext->fpr[20]
    lfd     fp21, pContext->fpr[21]
    lfd     fp22, pContext->fpr[22]
    lfd     fp23, pContext->fpr[23]
    lfd     fp24, pContext->fpr[24]
    lfd     fp25, pContext->fpr[25]
    lfd     fp26, pContext->fpr[26]
    lfd     fp27, pContext->fpr[27]
    lfd     fp28, pContext->fpr[28]
    lfd     fp29, pContext->fpr[29]
    lfd     fp30, pContext->fpr[30]
    lfd     fp31, pContext->fpr[31]

_ret:
    blr
}

static asm void __OSSaveFPUContext(register u32 v0, register u32 v1, register OSContext *pContext) {
    nofralloc
    lhz r3, pContext->state
    ori r3, r3, 1
    sth r3, pContext->state 

    stfd    fp0,  pContext->fpr[0]
    stfd    fp1,  pContext->fpr[1]
    stfd    fp2,  pContext->fpr[2]
    stfd    fp3,  pContext->fpr[3]
    stfd    fp4,  pContext->fpr[4]
    stfd    fp5,  pContext->fpr[5]
    stfd    fp6,  pContext->fpr[6]
    stfd    fp7,  pContext->fpr[7]
    stfd    fp8,  pContext->fpr[8]
    stfd    fp9,  pContext->fpr[9]
    stfd    fp10, pContext->fpr[10]
    stfd    fp11, pContext->fpr[11]
    stfd    fp12, pContext->fpr[12]
    stfd    fp13, pContext->fpr[13]
    stfd    fp14, pContext->fpr[14]
    stfd    fp15, pContext->fpr[15]
    stfd    fp16, pContext->fpr[16]
    stfd    fp17, pContext->fpr[17]
    stfd    fp18, pContext->fpr[18]
    stfd    fp19, pContext->fpr[19]
    stfd    fp20, pContext->fpr[20]
    stfd    fp21, pContext->fpr[21]
    stfd    fp22, pContext->fpr[22]
    stfd    fp23, pContext->fpr[23]
    stfd    fp24, pContext->fpr[24]
    stfd    fp25, pContext->fpr[25]
    stfd    fp26, pContext->fpr[26]
    stfd    fp27, pContext->fpr[27]
    stfd    fp28, pContext->fpr[28]
    stfd    fp29, pContext->fpr[29]
    stfd    fp30, pContext->fpr[30]
    stfd    fp31, pContext->fpr[31]

    mffs fp0
    stfd fp0, 400(pContext)
    lfd fp0, pContext->fpr[0]
    mfspr r3, 0x398
    extrwi. r3, r3, 1, 2
    beq _ret

    psq_st  fp0, OS_CONTEXT_PSF0(pContext), 0, 0
    psq_st  fp1, OS_CONTEXT_PSF1(pContext), 0, 0
    psq_st  fp2, OS_CONTEXT_PSF2(pContext), 0, 0
    psq_st  fp3, OS_CONTEXT_PSF3(pContext), 0, 0
    psq_st  fp4, OS_CONTEXT_PSF4(pContext), 0, 0
    psq_st  fp5, OS_CONTEXT_PSF5(pContext), 0, 0
    psq_st  fp6, OS_CONTEXT_PSF6(pContext), 0, 0
    psq_st  fp7, OS_CONTEXT_PSF7(pContext), 0, 0
    psq_st  fp8, OS_CONTEXT_PSF8(pContext), 0, 0
    psq_st  fp9, OS_CONTEXT_PSF9(pContext), 0, 0
    psq_st  fp10, OS_CONTEXT_PSF10(pContext), 0, 0
    psq_st  fp11, OS_CONTEXT_PSF11(pContext), 0, 0
    psq_st  fp12, OS_CONTEXT_PSF12(pContext), 0, 0
    psq_st  fp13, OS_CONTEXT_PSF13(pContext), 0, 0
    psq_st  fp14, OS_CONTEXT_PSF14(pContext), 0, 0
    psq_st  fp15, OS_CONTEXT_PSF15(pContext), 0, 0
    psq_st  fp16, OS_CONTEXT_PSF16(pContext), 0, 0
    psq_st  fp17, OS_CONTEXT_PSF17(pContext), 0, 0
    psq_st  fp18, OS_CONTEXT_PSF18(pContext), 0, 0
    psq_st  fp19, OS_CONTEXT_PSF19(pContext), 0, 0
    psq_st  fp20, OS_CONTEXT_PSF20(pContext), 0, 0
    psq_st  fp21, OS_CONTEXT_PSF21(pContext), 0, 0
    psq_st  fp22, OS_CONTEXT_PSF22(pContext), 0, 0
    psq_st  fp23, OS_CONTEXT_PSF23(pContext), 0, 0
    psq_st  fp24, OS_CONTEXT_PSF24(pContext), 0, 0
    psq_st  fp25, OS_CONTEXT_PSF25(pContext), 0, 0
    psq_st  fp26, OS_CONTEXT_PSF26(pContext), 0, 0
    psq_st  fp27, OS_CONTEXT_PSF27(pContext), 0, 0
    psq_st  fp28, OS_CONTEXT_PSF28(pContext), 0, 0
    psq_st  fp29, OS_CONTEXT_PSF29(pContext), 0, 0
    psq_st  fp30, OS_CONTEXT_PSF30(pContext), 0, 0
    psq_st  fp31, OS_CONTEXT_PSF31(pContext), 0, 0

_ret:
    blr    
}

asm void OSSaveFPUContext(register OSContext *pContext) {
    nofralloc
    addi r4, pContext, 0
    b __OSLoadFPUContext
}

asm void OSSetCurrentContext(register OSContext *pContext) {
    nofralloc
    addis r4, r0, -0x8000
    stw pContext, 0xD4(r4)
    clrlwi r5, pContext, 2
    stw r5, 0xC0(r4)
    lwz r5, 0xD8(r4)
    cmpw r5, pContext
    beq disableFPU

    lwz r6, pContext->srr1
    ori r6, r6, 0x2000
    stw r6, pContext->srr1

    mfmsr r6
    ori r6, r6, 2
    mtmsr r6
    blr

disableFPU:
    lwz r6, pContext->srr1
    rlwinm r6, r6, 0, 19, 17
    stw r6, pContext->srr1
    mfmsr r6
    rlwinm r6, r6, 0, 19, 17
    ori r6, r6, 2
    mtmsr r6
    isync
    blr
}

OSContext* OSGetCurrentContext(void) {
    return (OSContext*)__OSCurrentContext;
}

asm u32 OSSaveContext(register OSContext* context) {
    nofralloc
    stmw r13, context->gpr[13]

    mfspr   r0, 0x391
    stw     r0, context->gqr[1]
    mfspr   r0, 0x392
    stw     r0, context->gqr[2]
    mfspr   r0, 0x393
    stw     r0, context->gqr[3]
    mfspr   r0, 0x394
    stw     r0, context->gqr[4]
    mfspr   r0, 0x395
    stw     r0, context->gqr[5]
    mfspr   r0, 0x396
    stw     r0, context->gqr[6]
    mfspr   r0, 0x397
    stw     r0, context->gqr[7]

    mfcr    r0
    stw     r0, context->cr

    mflr    r0
    stw     r0, context->lr
    stw     r0, context->srr0

    mfmsr   r0
    stw     r0, context->srr1

    mfctr   r0
    stw     r0, context->ctr

    mfxer   r0
    stw     r0, context->xer

    stw     r1, context->gpr[1]
    stw     r2, context->gpr[2]

    li      r0, 1
    stw     r0, context->gpr[3]
    li      r3, 0
    blr
}

asm void OSLoadContext(register OSContext* context) {
    nofralloc

    lis r4, __RAS_OSDisableInterrupts_begin@ha
    lwz r6, context->srr0
    addi r5, r4, __RAS_OSDisableInterrupts_begin@l
    cmplw r6, r5
    ble notInRAS
    lis r4, __RAS_OSDisableInterrupts_end@ha
    addi r0, r4, __RAS_OSDisableInterrupts_end@l
    cmplw r6, r0
    bge notInRAS
    stw r5, context->srr0

notInRAS:
    lwz r0, context->gpr[0]
    lwz r1, context->gpr[1]
    lwz r2, context->gpr[2]
    lhz r4, context->state
    rlwinm. r5, r4, 0, 30, 30
    beq notexc
    rlwinm  r4, r4, 0, 31, 29
    sth r4, context->state
    lmw r5, context->gpr[5]
    b misc

notexc:
    lmw r13, context->gpr[13]

misc:   
    lwz r4, context->gqr[1]
    mtspr 0x391, r4
    lwz r4, context->gqr[2]
    mtspr 0x392, r4
    lwz r4, context->gqr[3]
    mtspr 0x393, r4
    lwz r4, context->gqr[4]
    mtspr 0x394, r4
    lwz r4, context->gqr[5]
    mtspr 0x395, r4
    lwz r4, context->gqr[6]
    mtspr 0x396, r4
    lwz r4, context->gqr[7]
    mtspr 0x397, r4

    lwz r4, context->cr
    mtcr r4
    lwz r4, context->lr
    mtlr r4
    lwz r4, context->ctr
    mtctr r4
    lwz r4, context->xer
    mtxer r4

    mfmsr r4
    rlwinm r4, r4, 0, 17, 15
    rlwinm r4, r4, 0, 31, 29
    mtmsr r4

    lwz r4, context->srr0
    mtsrr0 r4
    lwz r4, context->srr1
    mtsrr1 r4

    lwz r4, context->gpr[4]
    lwz r3, context->gpr[3]
    rfi
}

asm u32 OSGetStackPointer(void) {
    nofralloc
    mr r3, r1
    blr
}

asm int OSSwitchFiber(register u32 pc, register u32 newsp) {
    nofralloc
    mflr r0
    mr r5, r1
    stwu r5, -8(newsp)
    mr r1, newsp
    stw r0, 4(r5)
    mtlr pc
    blrl

    lwz r5, 0(r1)
    lwz r0, 4(r5)
    mtlr r0
    mr r1, r5
    blr
}

asm int OSSwitchFiberEx(register u32 arg0, register u32 arg1, register u32 arg2, register u32 arg3, register u32 pc, register u32 newsp) {
    nofralloc
    mflr r0
    mr r9, r1
    stwu r9, -8(newsp)
    mr r1, newsp
    stw r0, 4(r9)
    mtlr pc
    blrl

    lwz r5, 0(r1)
    lwz r0, 4(r5)
    mtlr r0

    mr r1, r5
    blr
}

void OSClearContext(OSContext *pContext) {
    pContext->mode = 0;
    pContext->state = 0;

    if (pContext == __OSFPUContext) {
        __OSFPUContext = 0;
    }
}

asm void OSInitContext(register OSContext* context, register u32 pc, register u32 newsp) {
    nofralloc
    stw     pc,  0x198(context)
    stw     newsp,  OS_CONTEXT_R1(context)
    li      r11, 0
    ori     r11, r11, 0x9032
    stw     r11, 0x19C(context)
    li      r0,  0x0
    stw     r0,  0x80(context)
    stw     r0,  0x8C(context)
    stw     r2,  OS_CONTEXT_R2(context)
    stw     r13, OS_CONTEXT_R13(context)

    stw     r0,  OS_CONTEXT_R3(context)
    stw     r0,  OS_CONTEXT_R4(context)
    stw     r0,  OS_CONTEXT_R5(context)
    stw     r0,  OS_CONTEXT_R6(context)
    stw     r0,  OS_CONTEXT_R7(context)
    stw     r0,  OS_CONTEXT_R8(context)
    stw     r0,  OS_CONTEXT_R9(context)
    stw     r0,  OS_CONTEXT_R10(context)
    stw     r0,  OS_CONTEXT_R11(context)
    stw     r0,  OS_CONTEXT_R12(context)

    stw     r0,  OS_CONTEXT_R14(context)
    stw     r0,  OS_CONTEXT_R15(context)
    stw     r0,  OS_CONTEXT_R16(context)
    stw     r0,  OS_CONTEXT_R17(context)
    stw     r0,  OS_CONTEXT_R18(context)
    stw     r0,  OS_CONTEXT_R19(context)
    stw     r0,  OS_CONTEXT_R20(context)
    stw     r0,  OS_CONTEXT_R21(context)
    stw     r0,  OS_CONTEXT_R22(context)
    stw     r0,  OS_CONTEXT_R23(context)
    stw     r0,  OS_CONTEXT_R24(context)
    stw     r0,  OS_CONTEXT_R25(context)
    stw     r0,  OS_CONTEXT_R26(context)
    stw     r0,  OS_CONTEXT_R27(context)
    stw     r0,  OS_CONTEXT_R28(context)
    stw     r0,  OS_CONTEXT_R29(context)
    stw     r0,  OS_CONTEXT_R30(context)
    stw     r0,  OS_CONTEXT_R31(context)
    stw     r0,  OS_CONTEXT_GQR0(context)
    stw     r0,  OS_CONTEXT_GQR1(context)
    stw     r0,  OS_CONTEXT_GQR2(context)
    stw     r0,  OS_CONTEXT_GQR3(context)
    stw     r0,  OS_CONTEXT_GQR4(context)
    stw     r0,  OS_CONTEXT_GQR5(context)
    stw     r0,  OS_CONTEXT_GQR6(context)
    stw     r0,  OS_CONTEXT_GQR7(context)
    b       OSClearContext
}

void OSDumpContext(OSContext* context) {
    u32 i;
    u32* p;

    OSReport("------------------------- Context 0x%08x -------------------------\n", context);

    for (i = 0; i < 16; ++i) {
        OSReport("r%-2d  = 0x%08x (%14d)  r%-2d  = 0x%08x (%14d)\n", i, context->gpr[i], context->gpr[i], i + 16, context->gpr[i + 16], context->gpr[i + 16]);
    }

    OSReport("LR   = 0x%08x                   CR   = 0x%08x\n", context->lr, context->cr);
    OSReport("SRR0 = 0x%08x                   SRR1 = 0x%08x\n", context->srr0, context->srr1);
    OSReport("\nGQRs----------\n");

    for (i = 0; i < 4; ++i) {
        OSReport("gqr%d = 0x%08x \t gqr%d = 0x%08x\n", i, context->gqr[i], i + 4, context->gqr[i + 4]);
    }

    if (context->state & 1) {
        OSContext* currentContext;
        OSContext fpuContext;
        BOOL enabled;
        enabled = OSDisableInterrupts();
        currentContext = OSGetCurrentContext();
        OSClearContext(&fpuContext);
        OSSetCurrentContext(&fpuContext);

        OSReport("\n\nFPRs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (u32)context->psf[i], i + 1, (u32)context->psf[i + 1]);
        }

        OSReport("\n\nPSFs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (u32)context->psf[i], i + 1, (u32)context->psf[i + 1]);
        }

        OSClearContext(&fpuContext);
        OSSetCurrentContext(currentContext);
        OSRestoreInterrupts(enabled);
    }

    OSReport("\nAddress:      Back Chain    LR Save\n");
    for (i = 0, p = (u32*)context->gpr[1]; p && (u32)p != 0xffffffff && i++ < 16; p = (u32*)*p) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }
}

inline void SwitchFPUContext(__OSException ex, OSContext *pContext) {
    PPCMtmsr(PPCMfmsr() | 0x2000);
    pContext->srr1 |= 0x2000;

    if (__OSFPUContext == pContext) {
        OSLoadContext(pContext);
    }

    if (__OSFPUContext) {
        OSSavepContext((OSContext*)__OSFPUContext);
    }

    __OSFPUContext = pContext;
    OSLoadFPUContext((OSContext*)__OSFPUContext);
    OSLoadContext(pContext);
}

asm void OSLoadFPUContext(register OSContext *pContext) {
    nofralloc
    addi r4, pContext, 0
    b __OSLoadFPUContext
}

static asm void OSSwitchFPUContext(register __OSException ex, register OSContext *pContext) {
    nofralloc
    mfmsr r5 
    ori r5, r5, 0x2000
    mtmsr r5
    isync

    lwz r5, 412(pContext)
    ori r5, r5, 0x2000
    mtsrr1 r5

    lis r3, -0x8000
    lwz r5, 0xD8(r3)
    stw pContext, 0xD8(r3)
    cmpw r5, r4
    beq restore

    cmpwi r5, 0
    beq loadFPU
    bl __OSSaveFPUContext

    loadFPU:
        bl __OSLoadFPUContext

    restore:
        lwz     r3, 0x80(pContext)
        mtcr    r3
        lwz     r3, 0x84(pContext)
        mtlr    r3
        lwz     r3, 0x198(pContext)
        mtsrr0  r3
        lwz     r3, 0x88(pContext)
        mtctr   r3
        lwz     r3, 0x8C(pContext)
        mtxer   r3

        lhz r3, pContext->state
        rlwinm r3, r3,0, 31, 29
        sth r3, pContext->state

        lwz r5, 0x14(pContext)
        lwz r3, 0xC(pContext)
        lwz r4, 0x10(pContext)
        rfi
}

void __OSContextInit(void) {
    __OSSetExceptionHandler(7, OSSwitchFPUContext);
    __OSFPUContext = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}

asm void OSFillFPUContext(register OSContext* context) {
    nofralloc
    mfmsr   r5
    ori     r5, r5, 0x2000
    mtmsr   r5
    isync

    stfd    fp0,  context->fpr[0]
    stfd    fp1,  context->fpr[1]
    stfd    fp2,  context->fpr[2]
    stfd    fp3,  context->fpr[3]
    stfd    fp4,  context->fpr[4]
    stfd    fp5,  context->fpr[5]
    stfd    fp6,  context->fpr[6]
    stfd    fp7,  context->fpr[7]
    stfd    fp8,  context->fpr[8]
    stfd    fp9,  context->fpr[9]
    stfd    fp10, context->fpr[10]
    stfd    fp11, context->fpr[11]
    stfd    fp12, context->fpr[12]
    stfd    fp13, context->fpr[13]
    stfd    fp14, context->fpr[14]
    stfd    fp15, context->fpr[15]
    stfd    fp16, context->fpr[16]
    stfd    fp17, context->fpr[17]
    stfd    fp18, context->fpr[18]
    stfd    fp19, context->fpr[19]
    stfd    fp20, context->fpr[20]
    stfd    fp21, context->fpr[21]
    stfd    fp22, context->fpr[22]
    stfd    fp23, context->fpr[23]
    stfd    fp24, context->fpr[24]
    stfd    fp25, context->fpr[25]
    stfd    fp26, context->fpr[26]
    stfd    fp27, context->fpr[27]
    stfd    fp28, context->fpr[28]
    stfd    fp29, context->fpr[29]
    stfd    fp30, context->fpr[30]
    stfd    fp31, context->fpr[31]
    mffs    fp0
    stfd    fp0,  0x190(context)
    lfd     fp0,  context->fpr[0]
    mfspr   r5, 0x398
    rlwinm. r5, r5, 3, 31, 31
    bc      12, 2, _return
    psq_st  fp0, OS_CONTEXT_PSF0(context), 0, 0
    psq_st  fp1, OS_CONTEXT_PSF1(context), 0, 0
    psq_st  fp2, OS_CONTEXT_PSF2(context), 0, 0
    psq_st  fp3, OS_CONTEXT_PSF3(context), 0, 0
    psq_st  fp4, OS_CONTEXT_PSF4(context), 0, 0
    psq_st  fp5, OS_CONTEXT_PSF5(context), 0, 0
    psq_st  fp6, OS_CONTEXT_PSF6(context), 0, 0
    psq_st  fp7, OS_CONTEXT_PSF7(context), 0, 0
    psq_st  fp8, OS_CONTEXT_PSF8(context), 0, 0
    psq_st  fp9, OS_CONTEXT_PSF9(context), 0, 0
    psq_st  fp10, OS_CONTEXT_PSF10(context), 0, 0
    psq_st  fp11, OS_CONTEXT_PSF11(context), 0, 0
    psq_st  fp12, OS_CONTEXT_PSF12(context), 0, 0
    psq_st  fp13, OS_CONTEXT_PSF13(context), 0, 0
    psq_st  fp14, OS_CONTEXT_PSF14(context), 0, 0
    psq_st  fp15, OS_CONTEXT_PSF15(context), 0, 0
    psq_st  fp16, OS_CONTEXT_PSF16(context), 0, 0
    psq_st  fp17, OS_CONTEXT_PSF17(context), 0, 0
    psq_st  fp18, OS_CONTEXT_PSF18(context), 0, 0
    psq_st  fp19, OS_CONTEXT_PSF19(context), 0, 0
    psq_st  fp20, OS_CONTEXT_PSF20(context), 0, 0
    psq_st  fp21, OS_CONTEXT_PSF21(context), 0, 0
    psq_st  fp22, OS_CONTEXT_PSF22(context), 0, 0
    psq_st  fp23, OS_CONTEXT_PSF23(context), 0, 0
    psq_st  fp24, OS_CONTEXT_PSF24(context), 0, 0
    psq_st  fp25, OS_CONTEXT_PSF25(context), 0, 0
    psq_st  fp26, OS_CONTEXT_PSF26(context), 0, 0
    psq_st  fp27, OS_CONTEXT_PSF27(context), 0, 0
    psq_st  fp28, OS_CONTEXT_PSF28(context), 0, 0
    psq_st  fp29, OS_CONTEXT_PSF29(context), 0, 0
    psq_st  fp30, OS_CONTEXT_PSF30(context), 0, 0
    psq_st  fp31, OS_CONTEXT_PSF31(context), 0, 0

_return:
    blr
}
