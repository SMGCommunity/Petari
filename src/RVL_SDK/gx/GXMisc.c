#include "revolution/gx/GXEnum.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/os/OSThread.h"
#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>
#include <revolution/gx/shortcut_pe_misc.h>
#include <private/flipper.h>
#include <revolution/os.h>

static GXDrawSyncCallback TokenCB;
static GXDrawDoneCallback DrawDoneCB;
static volatile GXBool DrawDone;
static OSThreadQueue FinishQueue;

void GXSetMisc(GXMiscToken token, u32 val) {
    switch (token) {
        case GX_MT_NULL:
            break;
        case GX_MT_XF_FLUSH:
            gx->vNum = val;
            gx->vNumNot = !gx->vNum;
            gx->bpSentNot = GX_TRUE;
            if (gx->vNum > 0) {
                gx->dirtyState |= 8;
            }
            break;
        case GX_MT_DL_SAVE_CONTEXT:
            gx->dlSaveContext = (val != 0);
            break;
        case GX_MT_ABORT_WAIT_COPYOUT:
            gx->abtWaitPECopy = (val != 0);
            break;
    }
}

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

void GXResetWriteGatherPipe(void) {
    while (PPCMfwpar() & 0x1) {

    }

    PPCMtwpar(OSUncachedToPhysical((void*)(0xC008000)));
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

void GXSetDrawSync(u16 token) {
    BOOL en;
    u32 reg = 0;
    en = OSDisableInterrupts();
    reg = PE_TOKEN_INT(token, 0x48);
    GX_WRITE_RA_REG(reg);

    SC_PE_TOKEN_SET_TOKEN(reg, token);
    SC_PE_TOKEN_SET_RID(reg, 0x47);
    GX_WRITE_RA_REG(reg);
    GXFlush();
    OSRestoreInterrupts(en);
    gx->bpSentNot = GX_FALSE;
}

void GXSetDrawDone(void) {
    u32 reg;
    BOOL en = OSDisableInterrupts();
    reg = PE_FINISH(2, 0x45);
    GX_WRITE_RA_REG(reg);
    GXFlush();
    DrawDone = GX_FALSE;
    OSRestoreInterrupts(en);
}

void GXWaitDrawDone(void) {
    BOOL en = OSDisableInterrupts();
    while (!DrawDone) {
        OSSleepThread(&FinishQueue);
    }
    OSRestoreInterrupts(en);
}

void GXDrawDone(void) {
    GXSetDrawDone();
    GXWaitDrawDone();
}

void GXPixModeSync(void) {
    GX_WRITE_RA_REG(gx->peCtrl);
    gx->bpSentNot = GX_FALSE;
}

void GXTexModeSync(void) {
    u32 reg = TX_LOADBLOCK3(0, 0, 0x63);
    GX_WRITE_RA_REG(reg);
    gx->bpSentNot = GX_FALSE;
}

void GXPokeAlphaMode(GXCompare func, u8 threshold) {
    u32 reg = (u32)(func << 8 | threshold);
    GX_PE_REG_WRITE_U16(3, reg);
}

void GXPokeAlphaRead(GXAlphaReadMode mode) {
    u32 reg = 0;
    SC_PE_PI_CTL_SET_AFMT(reg, mode);
    SC_PE_PI_CTL_SET_ZFMT(reg, 1);
    GX_PE_REG_WRITE_U16(4, reg);
}

void GXPokeAlphaUpdate(GXBool update_en) {
    u32 reg = GX_PE_REG_READ_U16(1);
    SC_PE_CMODE0_SET_ALPHA_MASK(reg, update_en);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src, GXBlendFactor dst, GXLogicOp op) {
    u32 reg = GX_PE_REG_READ_U16(1);
    SC_PE_CMODE0_SET_BLEND_ENABLE(reg, ((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT)));
    SC_PE_CMODE0_SET_BLENDOP(reg, (type == GX_BM_SUBTRACT));
    SC_PE_CMODE0_SET_LOGICOP_ENABLE(reg, (type == GX_BM_LOGIC));
    SC_PE_CMODE0_SET_LOGICOP(reg, op);
    SC_PE_CMODE0_SET_SFACTOR(reg, src);
    SC_PE_CMODE0_SET_DFACTOR(reg, dst);
    SC_PE_CMODE0_SET_RID(reg, 0x41);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeColorUpdate(GXBool update_en) {
    u32 reg = GX_PE_REG_READ_U16(1);
    SC_PE_CMODE0_SET_COLOR_MASK(reg, update_en);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeDstAlpha(GXBool enable, u8 alpha) {
    u32 reg = 0;
    SC_PE_CMODE1_SET_CONSTANT_ALPHA(reg, alpha);
    SC_PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(reg, enable);
    GX_PE_REG_WRITE_U16(2, reg);
}

void GXPokeDither(GXBool dither) {
    u32 reg = GX_PE_REG_READ_U16(1);
    SC_PE_CMODE0_SET_DITHER_ENABLE(reg, dither);
    GX_PE_REG_WRITE_U16(1, reg);
}

void GXPokeZMode(GXBool compare_en, GXCompare func, GXBool update_en) {
    u32 reg = 0;
    SC_PE_ZMODE_SET_ENABLE(reg, compare_en);
    SC_PE_ZMODE_SET_FUNC(reg, func);
    SC_PE_ZMODE_SET_MASK(reg, update_en);
    GX_PE_REG_WRITE_U16(0, reg);
}

void GXPeekARGB(u16 x, u16 y, u32* color) {
    u32 addr = (u32) OSPhysicalToUncached(0x8000000);
    SC_PE_PI_EFB_ADDR_SET_X(addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(addr,  0);
    *color = (*(u32 *)addr);
}

void GXPeekZ(u16 x, u16 y, u32* z)  {
    u32 addr = (u32) OSPhysicalToUncached(0x8000000);
    SC_PE_PI_EFB_ADDR_SET_X(addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(addr,  1);
    *z = *(u32*)addr;
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) { 
    GXDrawSyncCallback oldcb = TokenCB;
    BOOL en = OSDisableInterrupts();
    TokenCB = cb;
    OSRestoreInterrupts(en);
    return oldcb;
}

static void GXTokenInterruptHandler(__OSInterrupt, OSContext* context)
{
    u16 token;
    OSContext exceptionContext;
    u32 reg;
    
    token = (u16)GX_PE_REG_READ_U16(7);
    if (TokenCB) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        (*TokenCB)(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);        
    }

    reg = GX_PE_REG_READ_U16(5);
    SC_PE_INTRCTL_SET_INT0CLR(reg, 1);
    GX_PE_REG_WRITE_U16(5, reg);
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
    GXDrawDoneCallback oldcb = DrawDoneCB;
    BOOL en = OSDisableInterrupts();
    DrawDoneCB = cb;
    OSRestoreInterrupts(en);
    return oldcb;
}

static void
GXFinishInterruptHandler(__OSInterrupt, OSContext* context) {
    OSContext exceptionContext;
    u32 reg = 0;
    
    reg = GX_PE_REG_READ_U16(5);
    SC_PE_INTRCTL_SET_INT1CLR(reg, 1);
    GX_PE_REG_WRITE_U16(5, reg);

    DrawDone = GX_TRUE;

    if (DrawDoneCB)
    {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        (*DrawDoneCB)();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);        
    }

    OSWakeupThread(&FinishQueue);
}

void __GXPEInit (void) {
    u32 reg;
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_PE_TOKEN,  GXTokenInterruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_PE_FINISH, GXFinishInterruptHandler);

    OSInitThreadQueue(&FinishQueue);

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_PE_TOKEN);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_PE_FINISH);    

    reg = GX_PE_REG_READ_U16(5);
    SC_PE_INTRCTL_SET_INT0CLR(reg, 1);
    SC_PE_INTRCTL_SET_INT1CLR(reg, 1);
    SC_PE_INTRCTL_SET_INT0EN(reg, 1);
    SC_PE_INTRCTL_SET_INT1EN(reg, 1);
    GX_PE_REG_WRITE_U16(5, reg);
}
