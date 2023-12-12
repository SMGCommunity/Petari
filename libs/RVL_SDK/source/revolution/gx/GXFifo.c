#include <revolution/gx.h>
#include <revolution/os.h>
#include <revolution/gx/GXRegs.h>

static __GXFifoObj CPUFifo;
static __GXFifoObj GPFifo;
static GXBool GPFifoReady = FALSE;
static GXBool CPGPLinked;
volatile void* __GXCurrentBP;

#define TOPHYSICAL(a) (((u32)a) & 0x3FFFFFFF)

static void __GXFifoLink(GXBool en) {
    FAST_FLAG_SET(gx->cpEnable, (en ? 1 : 0), 4, 1); 
    GX_CP_REG_WRITE_U16(1, (u16)gx->cpEnable);
}

static void __GXWriteFifoIntEnable(GXBool hi, GXBool lo) {
    FAST_FLAG_SET(gx->cpEnable, hi, 2, 1);
    FAST_FLAG_SET(gx->cpEnable, lo, 3, 1);
    GX_CP_REG_WRITE_U16(1, gx->cpEnable);
}

static void __GXWriteFifoIntReset(GXBool hiWatermarkClr, GXBool loWatermarkClr) {
    FAST_FLAG_SET(gx->cpClr, hiWatermarkClr, 0, 1);
    FAST_FLAG_SET(gx->cpClr, loWatermarkClr, 1, 1);
    GX_CP_REG_WRITE_U16(2, (u16)gx->cpClr);
}

static void __GXFifoReadEnable(void) {
    FAST_FLAG_SET(gx->cpEnable, 1, 0, 1); 
    GX_CP_REG_WRITE_U16(1, (u16)gx->cpEnable);
}

static void __GXFifoReadDisable(void) {
    FAST_FLAG_SET(gx->cpEnable, 0, 0, 1); 
    GX_CP_REG_WRITE_U16(1, (u16)gx->cpEnable);
}

void __GXCleanGPFifo(void) {
    BOOL enabled;

    if (!GPFifoReady) {
        return;
    }

    enabled = OSDisableInterrupts();
    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(GX_FALSE, GX_FALSE);

    GPFifo.rdPtr = GPFifo.wrPtr;
    GPFifo.count = 0;

    GX_CP_REG_WRITE_U16(0x18, GPFifo.count);
    GX_CP_REG_WRITE_U16(0x1A, TOPHYSICAL(GPFifo.wrPtr));
    GX_CP_REG_WRITE_U16(0x1C, TOPHYSICAL(GPFifo.rdPtr));

    GX_CP_REG_WRITE_U16(0x19, GPFifo.count >> 16);
    GX_CP_REG_WRITE_U16(0x1B, TOPHYSICAL(GPFifo.wrPtr) >> 16);
    GX_CP_REG_WRITE_U16(0x1D, TOPHYSICAL(GPFifo.rdPtr) >> 16);

    PPCSync();

    if (CPGPLinked) {
        u32 reg = 0;
        CPUFifo.rdPtr = GPFifo.rdPtr;
        CPUFifo.wrPtr = GPFifo.wrPtr;
        CPUFifo.count = GPFifo.count;
        
        FAST_FLAG_SET(reg, (GX_PHY_ADDR(TOPHYSICAL(CPUFifo.wrPtr)) >> 5), 5, 24);
        GX_PI_REG_WRITE_U32(0x14, reg);  

        __GXWriteFifoIntEnable(GX_TRUE, GX_FALSE);
        __GXFifoLink(GX_TRUE);
    }

    FAST_FLAG_SET(gx->cpEnable, 0, 1, 1);
    FAST_FLAG_SET(gx->cpEnable, 0, 5, 1);
    GX_CP_REG_WRITE_U16(1, gx->cpEnable);
    __GXCurrentBP = 0;
    __GXWriteFifoIntReset(GX_TRUE, GX_TRUE);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

GXBool __GXIsGPFifoReady(void) {
    return GPFifoReady;
}