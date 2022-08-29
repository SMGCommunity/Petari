#include <revolution/db.h>
#include <revolution/os/OSCache.h>
#include <revolution/os.h>

asm void DCEnable(void) {
    nofralloc
    sync
    mfspr r3, HID0
    ori r3, r3, 0x4000
    mtspr HID0, r3
    blr
}

asm void DCInvalidateRange(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbi 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop
    
    blr
}

asm void DCFlushRange(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbf 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop
    
    sc
    blr
}

asm void DCStoreRange(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbst 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop
    
    sc
    blr
}

asm void DCFlushRangeNoSync(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbf 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop
    
    blr
}

asm void DCStoreRangeNoSync(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbst 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop

    blr
}

asm void DCZeroRange(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        dcbz 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop

    blr
}

asm void ICInvalidateRange(register void *pAddr, register u32 num) {
    nofralloc
    cmplwi num, 0
    blelr-
    clrlwi r5, pAddr, 27
    add num, num, r5
    addi num, num, 0x1F
    srwi num, num, 5
    mtctr num

    loop:
        icbi 0, pAddr
        addi pAddr, pAddr, 32
        bdnz loop

    sync
    isync
    blr
}

asm void ICFlashInvalidate(void) {
    nofralloc
    mfspr r3, HID0
    ori r3, r3, 0x800
    mtspr HID0, r3
    blr
}

asm void ICEnable(void) {
    nofralloc
    isync
    mfspr r3, HID0
    ori r3, r3, 0x8000
    mtspr HID0, r3
    blr
}

static asm void __LCEnable(void) {
    nofralloc
    mfmsr r5
    ori r5, r5, 0x1000
    mtmsr r5

    lis r3, -0x8000
    li r4, 0x400
    mtctr r4

    loop_1:
        dcbt 0, r3
        dcbst 0, r3
        addi r3, r3, 0x20
        bdnz loop_1

    mfspr r4, 0x398
    oris r4, r4, 0x100F
    mtspr 0x398, r4

    /* this is used to seperate the dcbz_l from mtspr HID2 */
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    lis r3, -0x2000
    ori r3, r3, 2
    mtspr DBAT3L, r3
    ori r3, r3, 0x1FE
    mtspr DBAT3U, r3
    isync

    lis r3, -0x2000
    li r6, 0x200
    mtctr r6
    li r6, 0

    loop_2:
        dcbz_l r6, r3
        addi r3, r3, 0x20
        bdnz+ loop_2

    // seperate the previous dcbz_l from the next icache load
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    blr
}

void LCEnable(void) {
    BOOL lvl;
    lvl = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(lvl);
}

asm void LCDisable(void) {
    lis r3, -0x2000
    li r4, 0x200
    mtctr r4

    loop:
        dcbi 0, r3
        addi r3, r3, 0x20
        bdnz loop

    mfspr r4, 0x398
    rlwinm r4, r4, 0, 4, 2
    mtspr 0x398, r4
    blr
}

asm void LCStoreBlocks(register void *pDest, register void *pSrc, register u32 num) {
    nofralloc
    /* set the upper bits of the DMA length (num) */
    rlwinm r6, num, 30, 27, 31
    /* set the destination address in the locked cache (DMA_U) */
    rlwinm pDest, pDest, 0, 3, 31
    or r6, r6, pDest
    mtspr DMA_U, r6
    /* set the lower bits of the DMA length */
    rlwinm r6, num, 2, 28, 29
    /* now we set the source address inside of the locked cache (DMA_U) */
    or r6, r6, pSrc
    ori r6, r6, 2
    mtspr DMA_L, r6
    blr
}

u32 LCStoreData(void *pDest, void *pSrc, u32 num) {
    u32 block_count = (num + 31) / 32;
    u32 v1 = (block_count + LC_MAX_DMA_BLOCKS - 1) / LC_MAX_DMA_BLOCKS;

    while (block_count > 0) {
        if (block_count < LC_MAX_DMA_BLOCKS) {
            LCStoreBlocks(pDest, pSrc, block_count);
            block_count = 0;
        }
        else {
            LCStoreBlocks(pDest, pSrc, 0);
            block_count -= LC_MAX_DMA_BLOCKS;
            pDest = (void*)((u32)pDest + 0x1000);
            pSrc = (void*)((u32)pSrc + 0x1000);
        }
    }

    return v1;
}

asm void LCQueueWait(register u32 length) {
    nofralloc

    loop:
        mfspr r4, 0x398
        rlwinm r4, r4, 8, 28, 31
        cmpw r4, length
        bgt loop

    blr
}

static void L2Disable(void);
static void L2GlobalInvalidate(void);

static inline void L2Init(void) {
    u32 old;
    old = PPCMfmsr();
    __sync();
    PPCMtmsr(0x30);
    __sync();
    L2Disable();

    L2GlobalInvalidate();
    PPCMtmsr(old);
}

void inline L2Enable(void) {
    PPCMtl2cr((PPCMfl2cr() | 0x80000000) & ~0x200000);
}

void inline L2Disable(void) {
    __sync();
    PPCMtl2cr(PPCMfl2cr() & ~0x80000000);
    __sync();
}

void inline L2GlobalInvalidate(void) {
    L2Disable();

    PPCMtl2cr(PPCMfl2cr() | 0x200000);
    while (PPCMfl2cr() & 1);
    PPCMtl2cr(PPCMfl2cr() & ~0x200000);

    while (PPCMfl2cr() & 1) {
        DBPrintf(">>> L2 INVALIDATE : SHOULD NEVER HAPPEN\n");
    }
}

// I don't know what OSError is used for here
void DMAErrorHandler(OSError, OSContext *pContext, ...) {
    u32 hid = PPCMfhid2();
    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%x   SRR1 = 0x%x\n", hid, pContext->srr1);

    if (!(hid & 0xF00000) || !(pContext->srr1 & 0x00200000)) {
        OSReport("Machine check was not DMA/locked cache related\n");
        OSDumpContext(pContext);
        PPCHalt();
    }

    OSReport("DMAErrorHandler(): An error occurred while processing DMA.\n");
    OSReport("The following errors have been detected and cleared :\n");

    if (hid & 0x00800000) {
        OSReport("\t- Requested a locked cache tag that was already in the cache\n");
    }

    if (hid & 0x00400000) {
        OSReport("\t- DMA attempted to access normal cache\n");
    }

    if (hid & 0x00200000) {
        OSReport("\t- DMA missed in data cache\n");
    }

    if (hid & 0x00100000) {
        OSReport("\t- DMA queue overflowed\n");
    }

    PPCMthid2(hid);
}

void __OSCacheInit(void) {
    if (!(PPCMfhid0() & 0x8000)) {
        ICEnable();
        DBPrintf("L1 i-caches initialized\n");
    }

    if (!(PPCMfhid0() & 0x4000)) {
        DCEnable();
        DBPrintf("L1 d-caches initialized\n");
    }

    if (!(PPCMfl2cr() & 0x80000000)) {
        // these two functions are inlined
        L2Init();
        L2Enable();
        DBPrintf("L2 cache initialized\n");
    }

    OSSetErrorHandler(1, DMAErrorHandler);
    DBPrintf("Locked cache machine check handler installed\n");
}