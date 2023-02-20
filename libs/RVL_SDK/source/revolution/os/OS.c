#include <revolution/os.h>

static __OSExceptionHandler* OSExceptionTable;

static f64 ZeroF;
static f32 ZeroPS[2];

asm void __OSFPRInit(void) {
    nofralloc
    mfmsr r3
    ori r3, r3, 0x2000
    mtmsr r3

    mfspr r3, 0x398
    rlwinm. r3, r3, 3, 31, 31
    beq skipPairedSingleInit

    lis r3, ZeroPS@ha
    addi r3, r3, ZeroPS@l
    psq_l fp0, 0(r3), 0, 0
    ps_mr fp1, fp0
    ps_mr fp2, fp0
    ps_mr fp3, fp0
    ps_mr fp4, fp0
    ps_mr fp5, fp0
    ps_mr fp6, fp0
    ps_mr fp7, fp0
    ps_mr fp8, fp0
    ps_mr fp9, fp0
    ps_mr fp10, fp0
    ps_mr fp11, fp0
    ps_mr fp12, fp0
    ps_mr fp13, fp0
    ps_mr fp14, fp0
    ps_mr fp15, fp0
    ps_mr fp16, fp0
    ps_mr fp17, fp0
    ps_mr fp18, fp0
    ps_mr fp19, fp0
    ps_mr fp20, fp0
    ps_mr fp21, fp0
    ps_mr fp22, fp0
    ps_mr fp23, fp0
    ps_mr fp24, fp0
    ps_mr fp25, fp0
    ps_mr fp26, fp0
    ps_mr fp27, fp0
    ps_mr fp28, fp0
    ps_mr fp29, fp0
    ps_mr fp30, fp0
    ps_mr fp31, fp0

skipPairedSingleInit:
    lfd fp0, ZeroF
    fmr fp1, fp0
    fmr fp2, fp0
    fmr fp3, fp0
    fmr fp4, fp0
    fmr fp5, fp0
    fmr fp6, fp0
    fmr fp7, fp0
    fmr fp8, fp0
    fmr fp9, fp0
    fmr fp10, fp0
    fmr fp11, fp0
    fmr fp12, fp0
    fmr fp13, fp0
    fmr fp14, fp0
    fmr fp15, fp0
    fmr fp16, fp0
    fmr fp17, fp0
    fmr fp18, fp0
    fmr fp19, fp0
    fmr fp20, fp0
    fmr fp21, fp0
    fmr fp22, fp0
    fmr fp23, fp0
    fmr fp24, fp0
    fmr fp25, fp0
    fmr fp26, fp0
    fmr fp27, fp0
    fmr fp28, fp0
    fmr fp29, fp0
    fmr fp30, fp0
    fmr fp31, fp0

    mtfsf 0xFF, fp0
    blr
}

__OSExceptionHandler __OSSetExceptionHandler(__OSException ex, __OSExceptionHandler handler) {
    __OSExceptionHandler cur;
    cur = OSExceptionTable[ex];
    OSExceptionTable[ex] = handler;
    return cur;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException ex) {
    return OSExceptionTable[ex];
}

void __OSPSInit(void) {
    PPCMthid2(PPCMfhid2() | 0xA0000000);
    ICFlashInvalidate();
    __sync();

    asm {
        li r3, 0
        mtspr 0x390, r3
        mtspr 0x391, r3
        mtspr 0x392, r3
        mtspr 0x393, r3
        mtspr 0x394, r3
        mtspr 0x395, r3
        mtspr 0x396, r3
        mtspr 0x397, r3
    }
}