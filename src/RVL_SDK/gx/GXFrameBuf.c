#include "private/gen_reg.h"
#include "private/pe_reg.h"
#include "revolution/gx.h"
#include "revolution/gx/GXFDLShortcut.h"
#include "revolution/gx/GXRegs.h"
#include "revolution/gx/shortcut_pe_reg.h"

void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
    gx->cpDispSrc = 0;
    SC_PE_COPY_SRC_ADDR_SET_X(gx->cpDispSrc, left);
    SC_PE_COPY_SRC_ADDR_SET_Y(gx->cpDispSrc, top);
    SC_PE_COPY_SRC_ADDR_SET_RID(gx->cpDispSrc, 0x49);

    gx->cpDispSize = 0;
    SC_PE_COPY_SRC_SIZE_SET_X(gx->cpDispSize, (wd - 1));
    SC_PE_COPY_SRC_SIZE_SET_Y(gx->cpDispSize, (ht - 1));
    SC_PE_COPY_SRC_SIZE_SET_RID(gx->cpDispSize, 0x4A);
}

void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
    gx->cpTexSrc = 0;
    SC_PE_COPY_SRC_ADDR_SET_X(gx->cpTexSrc, left);
    SC_PE_COPY_SRC_ADDR_SET_Y(gx->cpTexSrc, top);
    SC_PE_COPY_SRC_ADDR_SET_RID(gx->cpTexSrc, 0x49);

    gx->cpTexSize = 0;
    SC_PE_COPY_SRC_SIZE_SET_X(gx->cpTexSize, (wd - 1));
    SC_PE_COPY_SRC_SIZE_SET_Y(gx->cpTexSize, (ht - 1));
    SC_PE_COPY_SRC_SIZE_SET_RID(gx->cpTexSize, 0x4A);
}

void GXSetDispCopyDst(u16 wd, u16 ht) {
    u16 stride;

    stride = (u16)(wd * VI_DISPLAY_PIX_SZ);
    gx->cpDispStride = 0;
    SC_PE_COPY_DST_STRIDE_SET_STRIDE(gx->cpDispStride, (stride >> 5));
    SC_PE_COPY_DST_STRIDE_SET_RID(gx->cpDispStride, 0x4D);
}

void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap) {
    u32 rowTiles, colTiles, cmpTiles;
    u32 peTexFmt;
    u32 peTexFmtH;

    gx->cpTexZ = FALSE;
    peTexFmt = (u32)(fmt & 0xf);

    if (fmt == GX_TF_Z16)
        peTexFmt = 0xB;

    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_IA8:
    case GX_CTF_YUVA8:
        SC_PE_COPY_CMD_SET_CCV(gx->cpTex, 3);
        break;
    default:
        SC_PE_COPY_CMD_SET_CCV(gx->cpTex, 2);
        break;
    }

    gx->cpTexZ = ((fmt & _GX_TF_ZTF) == _GX_TF_ZTF);

    peTexFmtH = ((peTexFmt & 0x8) >> 3);
    SC_PE_COPY_CMD_SET_TEX_FORMATH(gx->cpTex, peTexFmtH);
    peTexFmt = (peTexFmt & 0x7);

    __GetImageTileCount(fmt, wd, ht, &rowTiles, &colTiles, &cmpTiles);

    gx->cpTexStride = 0;
    SC_PE_COPY_DST_STRIDE_SET_STRIDE(gx->cpTexStride, rowTiles * cmpTiles);
    SC_PE_COPY_DST_STRIDE_SET_RID(gx->cpTexStride, 0x4D);
    SC_PE_COPY_CMD_SET_MIP_MAP_FILTER(gx->cpTex, mipmap);
    SC_PE_COPY_CMD_SET_TEX_FORMAT(gx->cpTex, peTexFmt);
}

void GXSetDispCopyFrame2Field(GXCopyMode mode) {
    SC_PE_COPY_CMD_SET_INTERLACED(gx->cpDisp, mode);
    SC_PE_COPY_CMD_SET_INTERLACED(gx->cpTex, GX_COPY_PROGRESSIVE);
}
void GXSetCopyClamp(GXFBClamp clamp) {
    GXBool clmpB, clmpT;

    clmpT = ((clamp & GX_CLAMP_TOP) == GX_CLAMP_TOP);
    clmpB = ((clamp & GX_CLAMP_BOTTOM) == GX_CLAMP_BOTTOM);

    SC_PE_COPY_CMD_SET_CLAMP_TOP(gx->cpDisp, clmpT);
    SC_PE_COPY_CMD_SET_CLAMP_BOTTOM(gx->cpDisp, clmpB);

    SC_PE_COPY_CMD_SET_CLAMP_TOP(gx->cpTex, clmpT);
    SC_PE_COPY_CMD_SET_CLAMP_BOTTOM(gx->cpTex, clmpB);
}

static u32 __GXGetNumXfbLines(u32 efbHt, u32 iScale) {
    u32 count, realHt, iScaleD;

    count = (efbHt - 1) * 256;
    realHt = count / iScale + 1;

    iScaleD = iScale;
    if (iScaleD > 0x80 && iScaleD < 0x100) {
        while ((iScaleD & 0x01) == 0) {
            iScaleD >>= 1;
        }

        if ((efbHt % iScaleD) == 0) {
            ++realHt;
        }
    }

    if (realHt > 1024) {
        realHt = 1024;
    }

    return realHt;
}

u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale) {
    u32 iScale;
    iScale = (u32)(256.0f / yScale) & 0x1ff;
    return ((u16)__GXGetNumXfbLines((u32)efbHeight, iScale));
}

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight) {
    f32 fScale, yScale;
    u32 iScale, tgtHt, realHt;

    tgtHt = xfbHeight;
    yScale = (f32)xfbHeight / (f32)efbHeight;
    iScale = (u32)(256.0f / yScale) & 0x1ff;
    realHt = __GXGetNumXfbLines((u32)efbHeight, iScale);

    while (realHt > xfbHeight) {
        tgtHt--;
        yScale = (f32)tgtHt / (f32)efbHeight;
        iScale = (u32)(256.0f / yScale) & 0x1ff;
        realHt = __GXGetNumXfbLines((u32)efbHeight, iScale);
    }

    fScale = yScale;
    while (realHt < xfbHeight) {
        fScale = yScale;
        tgtHt++;
        yScale = (f32)tgtHt / (f32)efbHeight;
        iScale = (u32)(256.0f / yScale) & 0x1ff;
        realHt = __GXGetNumXfbLines((u32)efbHeight, iScale);
    }

    return fScale;
}

u32 GXSetDispCopyYScale(f32 vscale) {
    GXBool enable;
    u32 iScale;
    u32 ht;
    u32 reg;

    iScale = (u32)(256.0f / vscale) & 0x1ff;
    enable = (iScale != 0x100);

    reg = 0;
    SC_PE_COPY_SCALE_SET_SCALE(reg, iScale);
    SC_PE_COPY_SCALE_SET_RID(reg, 0x4E);
    GX_WRITE_RA_REG(reg);
    BP_SENT();

    SC_PE_COPY_CMD_SET_VERTICAL_SCALE(gx->cpDisp, enable);
    ht = PE_COPY_SRC_SIZE_GET_Y(gx->cpDispSize) + 1;

    return (__GXGetNumXfbLines(ht, iScale));
}

void GXSetCopyClear(GXColor clear_clr, u32 clear_z) {
    u32 reg;
    reg = 0;
    SC_PE_COPY_CLEAR_COLOR_AR_SET_RED(reg, clear_clr.r);
    SC_PE_COPY_CLEAR_COLOR_AR_SET_ALPHA(reg, clear_clr.a);
    SC_PE_COPY_CLEAR_COLOR_AR_SET_RID(reg, 0x4F);
    GX_WRITE_RA_REG(reg);

    reg = 0;
    SC_PE_COPY_CLEAR_COLOR_GB_SET_BLUE(reg, clear_clr.b);
    SC_PE_COPY_CLEAR_COLOR_GB_SET_GREEN(reg, clear_clr.g);
    SC_PE_COPY_CLEAR_COLOR_GB_SET_RID(reg, 0x50);
    GX_WRITE_RA_REG(reg);

    reg = 0;
    SC_PE_COPY_CLEAR_Z_SET_DATA(reg, clear_z);
    SC_PE_COPY_CLEAR_Z_SET_RID(reg, 0x51);
    GX_WRITE_RA_REG(reg);
    BP_SENT();
}

void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]) {
    u32 msLoc[4];
    u32 coeff0, coeff1;

    if (aa) {
        msLoc[0] = 0;
        SC_GEN_MSLOC_SET_XS0(msLoc[0], sample_pattern[0][0]);
        SC_GEN_MSLOC_SET_YS0(msLoc[0], sample_pattern[0][1]);
        SC_GEN_MSLOC_SET_XS1(msLoc[0], sample_pattern[1][0]);
        SC_GEN_MSLOC_SET_YS1(msLoc[0], sample_pattern[1][1]);
        SC_GEN_MSLOC_SET_XS2(msLoc[0], sample_pattern[2][0]);
        SC_GEN_MSLOC_SET_YS2(msLoc[0], sample_pattern[2][1]);
        SC_GEN_MSLOC_SET_RID(msLoc[0], 1);

        msLoc[1] = 0;
        SC_GEN_MSLOC_SET_XS0(msLoc[1], sample_pattern[3][0]);
        SC_GEN_MSLOC_SET_YS0(msLoc[1], sample_pattern[3][1]);
        SC_GEN_MSLOC_SET_XS1(msLoc[1], sample_pattern[4][0]);
        SC_GEN_MSLOC_SET_YS1(msLoc[1], sample_pattern[4][1]);
        SC_GEN_MSLOC_SET_XS2(msLoc[1], sample_pattern[5][0]);
        SC_GEN_MSLOC_SET_YS2(msLoc[1], sample_pattern[5][1]);
        SC_GEN_MSLOC_SET_RID(msLoc[1], 2);

        msLoc[2] = 0;
        SC_GEN_MSLOC_SET_XS0(msLoc[2], sample_pattern[6][0]);
        SC_GEN_MSLOC_SET_YS0(msLoc[2], sample_pattern[6][1]);
        SC_GEN_MSLOC_SET_XS1(msLoc[2], sample_pattern[7][0]);
        SC_GEN_MSLOC_SET_YS1(msLoc[2], sample_pattern[7][1]);
        SC_GEN_MSLOC_SET_XS2(msLoc[2], sample_pattern[8][0]);
        SC_GEN_MSLOC_SET_YS2(msLoc[2], sample_pattern[8][1]);
        SC_GEN_MSLOC_SET_RID(msLoc[2], 3);

        msLoc[3] = 0;
        SC_GEN_MSLOC_SET_XS0(msLoc[3], sample_pattern[9][0]);
        SC_GEN_MSLOC_SET_YS0(msLoc[3], sample_pattern[9][1]);
        SC_GEN_MSLOC_SET_XS1(msLoc[3], sample_pattern[10][0]);
        SC_GEN_MSLOC_SET_YS1(msLoc[3], sample_pattern[10][1]);
        SC_GEN_MSLOC_SET_XS2(msLoc[3], sample_pattern[11][0]);
        SC_GEN_MSLOC_SET_YS2(msLoc[3], sample_pattern[11][1]);
        SC_GEN_MSLOC_SET_RID(msLoc[3], 4);
    } else {
        msLoc[0] = GEN_MSLOC(6, 6, 6, 6, 6, 6, 1);
        msLoc[1] = GEN_MSLOC(6, 6, 6, 6, 6, 6, 2);
        msLoc[2] = GEN_MSLOC(6, 6, 6, 6, 6, 6, 3);
        msLoc[3] = GEN_MSLOC(6, 6, 6, 6, 6, 6, 4);
    }

    GX_WRITE_RA_REG(msLoc[0]);
    GX_WRITE_RA_REG(msLoc[1]);
    GX_WRITE_RA_REG(msLoc[2]);
    GX_WRITE_RA_REG(msLoc[3]);

    coeff0 = 0;
    SC_PE_COPY_VFILTER0_SET_RID(coeff0, 0x53);
    coeff1 = 0;
    SC_PE_COPY_VFILTER1_SET_RID(coeff1, 0x54);

    if (vf) {
        SC_PE_COPY_VFILTER0_SET_COEFF0(coeff0, vfilter[0]);
        SC_PE_COPY_VFILTER0_SET_COEFF1(coeff0, vfilter[1]);
        SC_PE_COPY_VFILTER0_SET_COEFF2(coeff0, vfilter[2]);
        SC_PE_COPY_VFILTER0_SET_COEFF3(coeff0, vfilter[3]);
        SC_PE_COPY_VFILTER1_SET_COEFF4(coeff1, vfilter[4]);
        SC_PE_COPY_VFILTER1_SET_COEFF5(coeff1, vfilter[5]);
        SC_PE_COPY_VFILTER1_SET_COEFF6(coeff1, vfilter[6]);
    } else {
        SC_PE_COPY_VFILTER0_SET_COEFF0(coeff0, 0);
        SC_PE_COPY_VFILTER0_SET_COEFF1(coeff0, 0);
        SC_PE_COPY_VFILTER0_SET_COEFF2(coeff0, 21);
        SC_PE_COPY_VFILTER0_SET_COEFF3(coeff0, 22);
        SC_PE_COPY_VFILTER1_SET_COEFF4(coeff1, 21);
        SC_PE_COPY_VFILTER1_SET_COEFF5(coeff1, 0);
        SC_PE_COPY_VFILTER1_SET_COEFF6(coeff1, 0);
    }

    GX_WRITE_RA_REG(coeff0);
    GX_WRITE_RA_REG(coeff1);
    BP_SENT();
}

void GXSetDispCopyGamma(GXGamma gamma) {
    SC_PE_COPY_CMD_SET_GAMMA(gx->cpDisp, gamma);
}

#define __GXVerifCopy(a, b) (void)0

void GXCopyDisp(void* dest, GXBool clear) {
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    GXBool changePeCtrl;

    __GXVerifCopy(dest, clear);

    if (clear) {
        reg = gx->zmode;
        SC_PE_ZMODE_SET_ENABLE(reg, 1);
        SC_PE_ZMODE_SET_FUNC(reg, 7);
        GX_WRITE_RA_REG(reg);

        reg = gx->cmode0;
        SC_PE_CMODE0_SET_BLEND_ENABLE(reg, 0);
        SC_PE_CMODE0_SET_LOGICOP_ENABLE(reg, 0);
        GX_WRITE_RA_REG(reg);
    }

    changePeCtrl = FALSE;
    if ((clear || (PE_CONTROL_GET_PIXTYPE(gx->peCtrl) == 3)) && (PE_CONTROL_GET_ZTOP(gx->peCtrl) == 1)) {
        changePeCtrl = TRUE;
        tempPeCtrl = gx->peCtrl;
        SC_PE_CONTROL_SET_ZTOP(tempPeCtrl, 0);
        GX_WRITE_RA_REG(tempPeCtrl);
    }

    // Set copy parameters.
    GX_WRITE_RA_REG(gx->cpDispSrc);
    GX_WRITE_RA_REG(gx->cpDispSize);
    GX_WRITE_RA_REG(gx->cpDispStride);

    phyAddr = GX_PHY_ADDR(dest);
    reg = 0;
    SC_PE_COPY_DST_BASE_SET_BASE(reg, (phyAddr >> 5));
    SC_PE_COPY_DST_BASE_SET_RID(reg, 0x4B);
    GX_WRITE_RA_REG(reg);

    SC_PE_COPY_CMD_SET_CLEAR(gx->cpDisp, clear);
    SC_PE_COPY_CMD_SET_OPCODE(gx->cpDisp, 1);
    SC_PE_COPY_CMD_SET_RID(gx->cpDisp, 0x52);
    GX_WRITE_RA_REG(gx->cpDisp);

    if (clear) {
        GX_WRITE_RA_REG(gx->zmode);
        GX_WRITE_RA_REG(gx->cmode0);
    }

    if (changePeCtrl) {
        GX_WRITE_RA_REG(gx->peCtrl);
    }

    BP_SENT();
}

void GXCopyTex(void* dest, GXBool clear) {
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    GXBool changePeCtrl;

    __GXVerifCopy(dest, clear);

    if (clear) {
        reg = gx->zmode;
        SC_PE_ZMODE_SET_ENABLE(reg, 1);
        SC_PE_ZMODE_SET_FUNC(reg, 7);
        GX_WRITE_RA_REG(reg);

        reg = gx->cmode0;
        SC_PE_CMODE0_SET_BLEND_ENABLE(reg, 0);
        SC_PE_CMODE0_SET_LOGICOP_ENABLE(reg, 0);
        GX_WRITE_RA_REG(reg);
    }

    changePeCtrl = FALSE;
    tempPeCtrl = gx->peCtrl;

    if (gx->cpTexZ && (PE_CONTROL_GET_PIXTYPE(tempPeCtrl) != 3)) {
        changePeCtrl = TRUE;
        SC_PE_CONTROL_SET_PIXTYPE(tempPeCtrl, 3);
    }

    if (clear || (PE_CONTROL_GET_PIXTYPE(tempPeCtrl) == 3)) {
        if (PE_CONTROL_GET_ZTOP(tempPeCtrl) == 1) {
            changePeCtrl = TRUE;
            SC_PE_CONTROL_SET_ZTOP(tempPeCtrl, 0);
        }
    }

    if (changePeCtrl) {
        GX_WRITE_RA_REG(tempPeCtrl);
    }

    GX_WRITE_RA_REG(gx->cpTexSrc);
    GX_WRITE_RA_REG(gx->cpTexSize);
    GX_WRITE_RA_REG(gx->cpTexStride);

    phyAddr = GX_PHY_ADDR(dest);
    reg = 0;
    SC_PE_COPY_DST_BASE_SET_BASE(reg, (phyAddr >> 5));
    SC_PE_COPY_DST_BASE_SET_RID(reg, 0x4B);
    GX_WRITE_RA_REG(reg);

    SC_PE_COPY_CMD_SET_CLEAR(gx->cpTex, clear);
    SC_PE_COPY_CMD_SET_OPCODE(gx->cpTex, 0);
    SC_PE_COPY_CMD_SET_RID(gx->cpTex, 0x52);
    GX_WRITE_RA_REG(gx->cpTex);

    if (clear) {
        GX_WRITE_RA_REG(gx->zmode);
        GX_WRITE_RA_REG(gx->cmode0);
    }

    if (changePeCtrl) {
        GX_WRITE_RA_REG(gx->peCtrl);
    }

    BP_SENT();
}

void GXClearBoundingBox(void) {
    u32 reg;

    reg = PE_XBOUND(1023, 0, 0x55);
    GX_WRITE_RA_REG(reg);

    reg = PE_YBOUND(1023, 0, 0x56);
    GX_WRITE_RA_REG(reg);
    BP_SENT();
}
