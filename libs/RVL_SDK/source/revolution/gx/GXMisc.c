#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>
#include <private/flipper.h>
#include <revolution/os.h>

void GXFlush(void) {
    if (gx->dirtyState) {
        __GXSetDirtyState();
    }

    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    PPCSync();
}

static void __GXAbortWait(u32 clocks) {
    OSTime time0, time1;
    time0 = OSGetTime();

    do {
        time1 = OSGetTime();
    }
    while (time1 - time0 <= clocks / 4);
}

static void __GXAbortWaitPECopyDone(void) {
    u32 peCnt0, peCnt1;
    
    peCnt0 = GX_MEM_COUNTER_READ_U32(MEM_PE_REQCOUNT);
    do {
        peCnt1 = peCnt0;
        __GXAbortWait(32);
    
        peCnt0 = GX_MEM_COUNTER_READ_U32(MEM_PE_REQCOUNT);
    } while ( peCnt0 != peCnt1 );
} 

void __GXAbort(void) {
    if (gx->abtWaitPECopy && __GXIsGPFifoReady()) {
        __GXAbortWaitPECopyDone();
    }

    __PIRegs[0x18/4] = 1;
    __GXAbortWait(200);
    __PIRegs[0x18/4] = 0;
    __GXAbortWait(20);
}

void GXAbortFrame(void) {
    __GXAbort();

    if (__GXIsGPFifoReady()) {
        __GXCleanGPFifo(); 
        __GXInitRevisionBits();
        gx->dirtyState = 0;
        GXFlush();
    }
}