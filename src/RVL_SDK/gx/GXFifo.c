#include "revolution/gx/GXFifo.h"
#include "private/cp_reg.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/os/OSThread.h"
#include <revolution/gx.h>
#include <revolution/os.h>
#include <revolution/gx/GXRegs.h>
#include <mem.h>

#define TOPHYSICAL(a) (((u32)a) & 0x3FFFFFFF)

static __GXFifoObj CPUFifo;
static __GXFifoObj GPFifo;
static GXBool CPUFifoReady = FALSE;
static GXBool GPFifoReady = FALSE;
static GXBool CPGPLinked;
static OSThread* __GXCurrentThread;
static volatile BOOL GXOverflowSuspendInProgress;
static GXBreakPtCallback BreakPointCB;
volatile void* __GXCurrentBP;
static volatile u32 __GXOverflowCount;

static void __GXWriteFifoIntEnable(GXBool , GXBool);
static void __GXWriteFifoIntReset(GXBool, GXBool);

#define CP_READ_2U16(dest,reg,type) \
{\
  u32 temp;                                 \
  temp =  (u32)GX_CP_REG_READ_U16(reg##H)<<16;   \
  temp |= (u32)GX_CP_REG_READ_U16(reg##L);       \
  dest = (type)OSPhysicalToCached(temp);   \
}

#define CP_READ_2U16_INT(dest,reg,type) \
{\
  u32 temp;                                 \
  temp =  (u32)GX_CP_REG_READ_U16(reg##H)<<16;   \
  temp |= (u32)GX_CP_REG_READ_U16(reg##L);       \
  dest = (type)temp;   \
}

static void GXInitFifoPtrs(GXFifoObj*, void *, void*);

static void GXOverflowHandler(__OSInterrupt interrupt, OSContext *context) {
    __GXOverflowCount++;
    __GXWriteFifoIntEnable(GX_FALSE, GX_TRUE);
    __GXWriteFifoIntReset(GX_TRUE, GX_FALSE);
    GXOverflowSuspendInProgress = TRUE;
    OSSuspendThread(__GXCurrentThread);
}

static void GXUnderflowHandler(__OSInterrupt interrupt, OSContext *context) {
    OSResumeThread(__GXCurrentThread);
    GXOverflowSuspendInProgress = FALSE;
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
}

static void __GXFifoReadEnable(void) {
    SC_CP_REG_ENABLE_SET_FIFORD(gx->cpEnable, 1); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gx->cpEnable);
}

static void __GXFifoReadDisable(void) {
    SC_CP_REG_ENABLE_SET_FIFORD(gx->cpEnable, 0); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gx->cpEnable);
}

static void __GXFifoLink(GXBool en) {
    SC_CP_REG_ENABLE_SET_WRPTRINC(gx->cpEnable, (en ? 0x1 : 0)); 
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gx->cpEnable);
}

static void GXBreakPointHandler(__OSInterrupt interrupt, OSContext *context) {
    OSContext exceptionContext;
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(gx->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);

    if (BreakPointCB) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        (*BreakPointCB)();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

void GXCPInterruptHandler(__OSInterrupt interrupt, OSContext *context) {
    gx->cpStatus = GX_CP_REG_READ_U16(0);
    if (CP_REG_ENABLE_GET_UNFLINT(gx->cpEnable) && CP_REG_STATUS_GET_UNFL(gx->cpStatus)) {
        GXUnderflowHandler(interrupt, context);
    }

    if (CP_REG_ENABLE_GET_OVFLINT(gx->cpEnable) && CP_REG_STATUS_GET_OVFL(gx->cpStatus)) {
        GXOverflowHandler(interrupt, context);
    }

    if (CP_REG_ENABLE_GET_FIFOBRKINT(gx->cpEnable) && CP_REG_STATUS_GET_FIFOBRK(gx->cpStatus)) {
        GXBreakPointHandler(interrupt, context);
    }
}

static void GXInitFifoLimits(GXFifoObj *fifo, u32 hi, u32 lo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    realFifo->hiWatermark = hi;
    realFifo->loWatermark = lo;
}

void GXInitFifoBase(GXFifoObj *fifo, void *base, u32 size) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    realFifo->base = base;
    realFifo->top = (void*)((u32)base + size - 4);
    realFifo->size = size;
    realFifo->count = 0;
    GXInitFifoLimits(fifo, size - (16 * 1024), OSRoundDown32B(size/2));
    GXInitFifoPtrs(fifo, base, base);
}

static void GXInitFifoPtrs(GXFifoObj* fifo, void *readPtr, void* writePtr) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    BOOL en = OSDisableInterrupts();
    realFifo->rdPtr = readPtr;
    realFifo->wrPtr = writePtr;
    realFifo->count = (u8*)writePtr - (u8*)readPtr;
    if (realFifo->count < 0) {
        realFifo->count += realFifo->size;
    }

    OSRestoreInterrupts(en);
}

GXBool CPGPLinkCheck(void) {
    u32 check = 0;
    s32 range1, range2, overlap;

    if (!CPUFifoReady || !GPFifoReady) {
        return GX_FALSE;
    }

    if (CPUFifo.base == GPFifo.base) {
        ++check;
    }

    if (CPUFifo.top == GPFifo.top) {
        ++check;
    }

    if (check == 2) {
        if (CPUFifo.hiWatermark == GPFifo.hiWatermark) {
            ++check;
        }

        if (CPUFifo.loWatermark == GPFifo.loWatermark) {
            ++check;
        }

        return GX_TRUE;
    }
    
    range1 = (s32)((u8*)CPUFifo.top - (u8*)GPFifo.base);
    range2 = (s32)((u8*)GPFifo.top - (u8*)CPUFifo.base);
    overlap = ((range1 > 0) && (range2 > 0) || (range1 < 0 && (range2 < 0)));

    if (overlap) {
        OSReport("CPUFifo: %08X - %08X\n", (u32)CPUFifo.base, (u32)CPUFifo.top);
        OSReport("GP Fifo: %08X - %08X\n", (u32)GPFifo.base, (u32)GPFifo.top);
    }

    return GX_FALSE;
}

void GXSetCPUFifo(const GXFifoObj* fifo) {
    const __GXFifoObj* realFifo = (const __GXFifoObj*)fifo;
    BOOL en = OSDisableInterrupts();
    u32 reg;

    if (fifo == NULL) {
        CPUFifoReady = GX_FALSE;
        CPGPLinked = GX_FALSE;
        CPUFifo.bind_cpu = CPUFifo.bind_gp = GX_FALSE;
        OSRestoreInterrupts(en);
        return;
    }

    CPUFifo = *realFifo;
    CPUFifoReady = GX_TRUE;
    CPUFifo.bind_cpu = GX_TRUE;

    if (CPGPLinkCheck()) {
        CPGPLinked = GX_TRUE;
        CPUFifo.bind_gp = GX_TRUE;

        reg = 0;
        
        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base));
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.top));

        SC_PI_CPWRT_REG_SET_CPWRT(reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);

        __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }
    else {
        CPUFifo.bind_gp = GX_FALSE;

        if (CPGPLinked) {
            __GXFifoLink(GX_FALSE);
            CPGPLinked = GX_FALSE;
        }

        __GXWriteFifoIntEnable (GX_FALSE, GX_FALSE);

        reg = 0;

        GX_PI_REG_WRITE_U32(PI_REG_CPBAS, TOPHYSICAL(CPUFifo.base));
        GX_PI_REG_WRITE_U32(PI_REG_CPTOP, TOPHYSICAL(CPUFifo.top));  
        SC_PI_CPWRT_REG_SET_CPWRT(reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
    }

    PPCSync();
    OSRestoreInterrupts(en);
}

void GXSetGPFifo(const GXFifoObj* fifo) {
    const __GXFifoObj* realFifo = (const __GXFifoObj*)fifo;
    BOOL en;
    u32 stbtmp;

    en = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable (GX_FALSE, GX_FALSE);

    if (fifo == NULL) {
        GPFifoReady = GX_FALSE;
        CPGPLinked = GX_FALSE;
        __GXFifoLink(GX_FALSE);
        GPFifo.bind_gp = GPFifo.bind_cpu = GX_FALSE;
        OSRestoreInterrupts(en);
        return;
    }

    GPFifo = *realFifo;
    GPFifoReady = GX_TRUE;
    GPFifo.bind_gp = GX_TRUE;
    GX_CP_REG_WRITE_U16(CP_FIFO_BASEL, TOPHYSICAL(GPFifo.base));
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPL, TOPHYSICAL(GPFifo.top));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, TOPHYSICAL(GPFifo.wrPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, TOPHYSICAL(GPFifo.rdPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTL, GPFifo.hiWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTL, GPFifo.loWatermark);
    GX_CP_REG_WRITE_U16(CP_FIFO_BASEH, TOPHYSICAL(GPFifo.base) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_TOPH, TOPHYSICAL(GPFifo.top) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, GPFifo.count >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, TOPHYSICAL(GPFifo.wrPtr) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, TOPHYSICAL(GPFifo.rdPtr) >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_HICNTH, GPFifo.hiWatermark >> 16);
    GX_CP_REG_WRITE_U16(CP_FIFO_LOCNTH, GPFifo.loWatermark >> 16);

    PPCSync();

    if (CPGPLinkCheck()) {
        CPGPLinked = GX_TRUE;
        GPFifo.bind_cpu = GX_TRUE;
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }
    else {
        CPGPLinked = GX_FALSE;
        GPFifo.bind_cpu = GX_FALSE;
        __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
        __GXFifoLink(GX_FALSE);
    }

    stbtmp = gx->cpEnable;
    SC_CP_REG_ENABLE_SET_FIFOBRK(stbtmp, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(stbtmp, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, stbtmp);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(en);
}

static void __GXSaveFifoCPStat(void) {
    CP_READ_2U16(GPFifo.rdPtr, CP_FIFO_RPTR, void*);
    CP_READ_2U16_INT(GPFifo.count, CP_FIFO_COUNT, s32);
}

static void __GXSaveFifoPIStat(void) {
    u32 reg = GX_PI_REG_READ_U32(PI_REG_CPWRT);
    CPUFifo.wrPtr = OSPhysicalToCached(reg & PI_CPWRT_REG_CPWRT_MASK);
    CPUFifo.wrap = (GXBool)((reg & PI_CPWRT_REG_CPWRAP_MASK) ? GX_TRUE : GX_FALSE);
}

void __GXSaveFifo(void) {
    BOOL en = OSDisableInterrupts();

    if (CPUFifoReady) {
        __GXSaveFifoPIStat();
    }

    if (GPFifoReady) {
        __GXSaveFifoCPStat();
    }

    if (CPGPLinked) {
        CPUFifo.rdPtr = GPFifo.rdPtr;
        CPUFifo.count = GPFifo.count;
        GPFifo.wrPtr = CPUFifo.wrPtr;
        GPFifo.wrap = CPUFifo.wrap;
    }
    else if (CPUFifoReady) {
        CPUFifo.count  = (s32)CPUFifo.wrPtr - (s32)CPUFifo.rdPtr;
        if (CPUFifo.count < 0) {
            CPUFifo.count += CPUFifo.size;
        }
    }

    OSRestoreInterrupts(en);
}

GXBool __GXIsGPFifoReady(void) {
    return GPFifoReady;
}

void GXGetGPStatus(GXBool* overhi, GXBool* underlow, GXBool* readIdle, GXBool* cmdIdle, GXBool* brkpt) {
    gx->cpStatus = GX_CP_REG_READ_U16(CP_STATUS);
    *overhi = (u8)CP_REG_STATUS_GET_OVFL(gx->cpStatus);
    *underlow = (u8)CP_REG_STATUS_GET_UNFL(gx->cpStatus);
    *readIdle = (u8)CP_REG_STATUS_GET_FIFO_RDIDLE(gx->cpStatus);
    *cmdIdle = (u8)CP_REG_STATUS_GET_CPIDLE(gx->cpStatus);
    *brkpt = (u8)CP_REG_STATUS_GET_FIFOBRK(gx->cpStatus);
}

GXBool GXGetCPUFifo(GXFifoObj *fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    if (!CPUFifoReady) {
        return GX_FALSE;
    }

    __GXSaveFifo();
    *realFifo = CPUFifo;
    return GX_TRUE;
}

void GXGetFifoPtrs(const GXFifoObj* fifo, void** readPtr, void** writePtr) {
    __GXFifoObj* realFifo = (__GXFifoObj*) fifo;
    *readPtr  = realFifo->rdPtr;
    *writePtr = realFifo->wrPtr;
}

u32 GXGetFifoCount(const GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*)fifo;
    return (u32)realFifo->count;
}

GXBool GXGetFifoWrap(const GXFifoObj* fifo) {
    __GXFifoObj* realFifo = (__GXFifoObj*) fifo;
    return realFifo->wrap;
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
    GXBreakPtCallback oldcb = BreakPointCB;
    BOOL enabled = OSDisableInterrupts();
    BreakPointCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

void GXEnableBreakPt(void* break_pt) {
    BOOL enabled = OSDisableInterrupts();
    __GXFifoReadDisable();
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKL, TOPHYSICAL(break_pt));
    GX_CP_REG_WRITE_U16(CP_FIFO_BRKH, TOPHYSICAL(break_pt) >> 16);
    SC_CP_REG_ENABLE_SET_FIFOBRK(gx->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(gx->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);
    SC_CP_REG_ENABLE_SET_FIFOBRK(gx->cpEnable, 1);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(gx->cpEnable, 1);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);
    __GXCurrentBP = break_pt;
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void GXDisableBreakPt(void) {
    BOOL enabled = OSDisableInterrupts();
    SC_CP_REG_ENABLE_SET_FIFOBRK(gx->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(gx->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);
    __GXCurrentBP = 0;
    OSRestoreInterrupts(enabled);
}

void __GXFifoInit() {
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_CP, GXCPInterruptHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_CP);
    __GXCurrentThread = OSGetCurrentThread();
    GXOverflowSuspendInProgress = FALSE;
    memset(&CPUFifo, 0, sizeof(__GXFifoObj));
    memset(&GPFifo, 0, sizeof(__GXFifoObj));
    CPUFifoReady = GX_FALSE;
    GPFifoReady  = GX_FALSE;
}

void __GXCleanGPFifo (void)
{
    BOOL enabled;

    if ( !GPFifoReady) {
        return;
    }

    enabled = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);
    GPFifo.rdPtr = GPFifo.wrPtr;
    GPFifo.count = 0;
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTL, GPFifo.count);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRL, TOPHYSICAL(GPFifo.wrPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRL, TOPHYSICAL(GPFifo.rdPtr));
    GX_CP_REG_WRITE_U16(CP_FIFO_COUNTH, GPFifo.count>>16);
    GX_CP_REG_WRITE_U16(CP_FIFO_WPTRH, TOPHYSICAL(GPFifo.wrPtr)>>16);
    GX_CP_REG_WRITE_U16(CP_FIFO_RPTRH, TOPHYSICAL(GPFifo.rdPtr)>>16);

    PPCSync();

    if (CPGPLinked) {
        u32 reg = 0;
        CPUFifo.rdPtr = GPFifo.rdPtr;
        CPUFifo.wrPtr = GPFifo.wrPtr;
        CPUFifo.count = GPFifo.count;
        SC_PI_CPWRT_REG_SET_CPWRT(reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5));
        SC_PI_CPWRT_REG_SET_CPWRAP(reg, 0);
        GX_PI_REG_WRITE_U32(PI_REG_CPWRT, reg);
        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }

    SC_CP_REG_ENABLE_SET_FIFOBRK(gx->cpEnable, 0);
    SC_CP_REG_ENABLE_SET_FIFOBRKINT(gx->cpEnable, 0);
    GX_CP_REG_WRITE_U16(CP_ENABLE, gx->cpEnable);
    __GXCurrentBP = 0;
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

static void __GXWriteFifoIntEnable(GXBool hi, GXBool lo) {
    SC_CP_REG_ENABLE_SET_OVFLINT(gx->cpEnable, hi);
    SC_CP_REG_ENABLE_SET_UNFLINT(gx->cpEnable, lo);
    GX_CP_REG_WRITE_U16(CP_ENABLE, (u16)gx->cpEnable);
}

static void __GXWriteFifoIntReset(GXBool hi, GXBool lo) {
    SC_CP_REG_CLR_SET_OVFLINT(gx->cpClr, hi);
    SC_CP_REG_CLR_SET_UNFLINT(gx->cpClr, lo);
    GX_CP_REG_WRITE_U16(CP_CLR, (u16)gx->cpClr);
}
