#include <revolution/base/PPCArch.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSException.h>
#include <revolution/db.h>

volatile OSContext* __OSCurrentContext: (0x8000 << 16 | 0xD4);
volatile OSContext* __OSpContext : (0x8000 << 16 | 0xD8);

/* these two do not match yet, I just have them defined to avoid the issues with OSSwitchFPUContext */
static asm void __OSLoadFPUContext(register u32, register OSContext *pContext) {
    nofralloc
    lhz r5, pContext->state
    clrlwi. r5, r5, 31
    beq _ret

    lfd f0, 0x190(pContext)
    mtfsf 0xFF, f0
    mfspr r5, HID2
    extrwi. r5, r5, 1, 2
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
    mfspr r3, HID2
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

inline void SwitchFPUContext(__OSException ex, OSContext *pContext) {
    PPCMtmsr(PPCMfmsr() | 0x2000);
    pContext->srr1 |= 0x2000;

    if (__OSpContext == pContext) {
        OSLoadContext(pContext);
    }

    if (__OSpContext) {
        OSSavepContext((OSContext*)__OSpContext);
    }

    __OSpContext = pContext;
    OSLoadFPUContext((OSContext*)__OSpContext);
    OSLoadContext(pContext);
}

asm void OSLoadFPUContext(register OSContext *pContext) {
    nofralloc
    addi r4, pContext, 0
    b __OSLoadFPUContext
}

asm void OSSaveFPUContext(register OSContext *pContext) {
    nofralloc
    addi r4, pContext, 0
    b __OSSaveFPUContext
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
    __OSpContext = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}