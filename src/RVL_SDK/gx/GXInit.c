#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

static GXData gxData;
GXData *const __GXData = &gxData;

volatile void*__piReg  = NULL;
volatile void*__cpReg  = NULL;
volatile void*__peReg  = NULL;
volatile void*__memReg = NULL;

void push_fmt(u32 vtxfmt) {
        FAST_FLAG_SET(gx->vatB[vtxfmt], 1, 31, 1);
        GX_WRITE_CP_STRM_REG(8, (s32)vtxfmt, gx->vatB[vtxfmt]);
}

void __GXInitRevisionBits(void)  {
    u32 i;

    for (i = 0; i < GX_MAX_VTXFMT; i++) {
        FAST_FLAG_SET(gx->vatA[i], 1, 30, 1);
		push_fmt(i);
    }

    {
        u32 reg1 = 0;
        u32 reg2 = 0;

        FAST_FLAG_SET(reg1, 1, 0, 1);
        FAST_FLAG_SET(reg1, 1, 1, 1);
        FAST_FLAG_SET(reg1, 1, 2, 1); 
        FAST_FLAG_SET(reg1, 1, 3, 1);
        FAST_FLAG_SET(reg1, 1, 4, 1);
        FAST_FLAG_SET(reg1, 1, 5, 1);
        GX_WRITE_XF_REG(0x1000, reg1, 0);
        FAST_FLAG_SET(reg2, 1, 0, 1);
        GX_WRITE_XF_REG(0x1012, reg2, 0); 
    }

    {
        u32 reg = 0;
        FAST_FLAG_SET(reg, 1, 0, 1);
        FAST_FLAG_SET(reg, 1, 1, 1);
        FAST_FLAG_SET(reg, 1, 2, 1);
        FAST_FLAG_SET(reg, 1, 3, 1);
        FAST_FLAG_SET(reg, 0x58, 24, 8);
        GX_WRITE_RA_REG(reg);
    }
}