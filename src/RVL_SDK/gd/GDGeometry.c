#include "revolution/gd.h"
#include "revolution/gx.h"
#include "revolution/os.h"

void GDSetVtxDescv(const GXVtxDescList* attrPtr) {
    u32 nnorms = 0;
    u32 ncols = 0;
    u32 ntexs = 0;

    u32 pnMtxIdx = GX_NONE;
    u32 txMtxIdxMask = 0;
    u32 posn = GX_DIRECT;
    u32 norm = GX_NONE;
    u32 col0 = GX_NONE;
    u32 col1 = GX_NONE;
    u32 tex0 = GX_NONE;
    u32 tex1 = GX_NONE;
    u32 tex2 = GX_NONE;
    u32 tex3 = GX_NONE;
    u32 tex4 = GX_NONE;
    u32 tex5 = GX_NONE;
    u32 tex6 = GX_NONE;
    u32 tex7 = GX_NONE;

    while (attrPtr->attr != GX_VA_NULL) {
        switch (attrPtr->attr) {
        case GX_VA_PNMTXIDX:
            pnMtxIdx = attrPtr->type;
            break;

        case GX_VA_TEX0MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~1) | (attrPtr->type << 0);
            break;
        case GX_VA_TEX1MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~2) | (attrPtr->type << 1);
            break;
        case GX_VA_TEX2MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~4) | (attrPtr->type << 2);
            break;
        case GX_VA_TEX3MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~8) | (attrPtr->type << 3);
            break;
        case GX_VA_TEX4MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~16) | (attrPtr->type << 4);
            break;
        case GX_VA_TEX5MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~32) | (attrPtr->type << 5);
            break;
        case GX_VA_TEX6MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~64) | (attrPtr->type << 6);
            break;
        case GX_VA_TEX7MTXIDX:
            txMtxIdxMask = (txMtxIdxMask & ~128) | (attrPtr->type << 7);
            break;

        case GX_VA_POS:
            posn = attrPtr->type;
            break;

        case GX_VA_NRM:
            if (attrPtr->type != GX_NONE) {
                norm = attrPtr->type;
                nnorms = 1;
            }
            break;
        case GX_VA_NBT:
            if (attrPtr->type != GX_NONE) {
                norm = attrPtr->type;
                nnorms = 2;
            }
            break;

        case GX_VA_CLR0:
            col0 = attrPtr->type;
            ncols += (col0 != GX_NONE);
            break;
        case GX_VA_CLR1:
            col1 = attrPtr->type;
            ncols += (col1 != GX_NONE);
            break;

        case GX_VA_TEX0:
            tex0 = attrPtr->type;
            ntexs += (tex0 != GX_NONE);
            break;
        case GX_VA_TEX1:
            tex1 = attrPtr->type;
            ntexs += (tex1 != GX_NONE);
            break;
        case GX_VA_TEX2:
            tex2 = attrPtr->type;
            ntexs += (tex2 != GX_NONE);
            break;
        case GX_VA_TEX3:
            tex3 = attrPtr->type;
            ntexs += (tex3 != GX_NONE);
            break;
        case GX_VA_TEX4:
            tex4 = attrPtr->type;
            ntexs += (tex4 != GX_NONE);
            break;
        case GX_VA_TEX5:
            tex5 = attrPtr->type;
            ntexs += (tex5 != GX_NONE);
            break;
        case GX_VA_TEX6:
            tex6 = attrPtr->type;
            ntexs += (tex6 != GX_NONE);
            break;
        case GX_VA_TEX7:
            tex7 = attrPtr->type;
            ntexs += (tex7 != GX_NONE);
            break;
        default:
            break;
        }
        attrPtr++;
    }

    GDWriteCPCmd(CP_VCD_LO_ID, CP_VCD_REG_LO_PS(pnMtxIdx, txMtxIdxMask, posn, norm, col0, col1));
    GDWriteCPCmd(CP_VCD_HI_ID, CP_VCD_REG_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7));
    GDWriteXFCmd(XF_INVTXSPEC_ID, XF_INVTXSPEC(ncols, nnorms, ntexs));
}

void GDSetArray(GXAttr attr, const void* base_ptr, u8 stride) {
    s32 cpAttr;
    cpAttr = (attr == GX_VA_NBT) ? (GX_VA_NRM - GX_VA_POS) : (attr - GX_VA_POS);
    GDWriteCPCmd((u8)(CP_ARRAY_BASE_ID + cpAttr), CP_ARRAY_BASE_REG(OSCachedToPhysical(base_ptr)));
    GDWriteCPCmd((u8)(CP_ARRAY_STRIDE_ID + cpAttr), CP_ARRAY_STRIDE_REG(stride));
}

void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride) {
    s32 cpAttr;
    cpAttr = (attr == GX_VA_NBT) ? (GX_VA_NRM - GX_VA_POS) : (attr - GX_VA_POS);
    GDWriteCPCmd((u8)(CP_ARRAY_BASE_ID + cpAttr), CP_ARRAY_BASE_REG(base_ptr_raw));
    GDWriteCPCmd((u8)(CP_ARRAY_STRIDE_ID + cpAttr), CP_ARRAY_STRIDE_REG(stride));
}

void GDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, GXBool normalize, u32 postmtx) {
    u32 form;
    u32 tgType;
    u32 proj;
    u32 row;
    u32 embossRow;
    u32 embossLit;

    form = XF_TEX_AB11;
    proj = XF_TEX_ST;
    row = XF_TEX0_INROW;
    embossRow = XF_TEX0_INROW;
    embossLit = 0;

    switch (src_param) {
    case GX_TG_POS:
        row = XF_GEOM_INROW;
        form = XF_TEX_ABC1;
        break;
    case GX_TG_NRM:
        row = XF_NORMAL_INROW;
        form = XF_TEX_ABC1;
        break;
    case GX_TG_BINRM:
        row = XF_BINORMAL_T_INROW;
        form = XF_TEX_ABC1;
        break;
    case GX_TG_TANGENT:
        row = XF_BINORMAL_B_INROW;
        form = XF_TEX_ABC1;
        break;
    case GX_TG_COLOR0:
        row = XF_COLORS_INROW;
        break;
    case GX_TG_COLOR1:
        row = XF_COLORS_INROW;
        break;
    case GX_TG_TEX0:
        row = XF_TEX0_INROW;
        break;
    case GX_TG_TEX1:
        row = XF_TEX1_INROW;
        break;
    case GX_TG_TEX2:
        row = XF_TEX2_INROW;
        break;
    case GX_TG_TEX3:
        row = XF_TEX3_INROW;
        break;
    case GX_TG_TEX4:
        row = XF_TEX4_INROW;
        break;
    case GX_TG_TEX5:
        row = XF_TEX5_INROW;
        break;
    case GX_TG_TEX6:
        row = XF_TEX6_INROW;
        break;
    case GX_TG_TEX7:
        row = XF_TEX7_INROW;
        break;
    case GX_TG_TEXCOORD0:
        embossRow = 0;
        break;
    case GX_TG_TEXCOORD1:
        embossRow = 1;
        break;
    case GX_TG_TEXCOORD2:
        embossRow = 2;
        break;
    case GX_TG_TEXCOORD3:
        embossRow = 3;
        break;
    case GX_TG_TEXCOORD4:
        embossRow = 4;
        break;
    case GX_TG_TEXCOORD5:
        embossRow = 5;
        break;
    case GX_TG_TEXCOORD6:
        embossRow = 6;
        break;
    default:
        break;
    }

    switch (func) {
    case GX_TG_MTX2x4:
        tgType = XF_TEXGEN_REGULAR;
        break;

    case GX_TG_MTX3x4:
        tgType = XF_TEXGEN_REGULAR;
        proj = XF_TEX_STQ;
        break;

    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7:
        tgType = XF_TEXGEN_EMBOSS_MAP;
        embossLit = (u32)(func - GX_TG_BUMP0);
        break;

    case GX_TG_SRTG:
        if (src_param == GX_TG_COLOR0) {
            tgType = XF_TEXGEN_COLOR_STRGBC0;
        } else {
            tgType = XF_TEXGEN_COLOR_STRGBC1;
        }
        break;

    default:
        break;
    }

    GDWriteXFCmd((u16)(XF_TEX0_ID + dst_coord), XF_TEX(proj, form, tgType, row, embossRow, embossLit));
    GDWriteXFCmd((u16)(XF_DUALTEX0_ID + dst_coord), XF_DUALTEX((postmtx - GX_PTTEXMTX0), normalize));
}

#define GEN_MODE_MASK_SETCULLMODE (0x000003 << GEN_MODE_REJECT_EN_SHIFT)

void GDSetCullMode(GXCullMode mode) {
    static u8 cm2hw[] = {0, 2, 1, 3};
    GDWriteBPCmd(SS_MASK(GEN_MODE_MASK_SETCULLMODE));
    GDWriteBPCmd(GEN_MODE(0, 0, 0, 0, cm2hw[mode], 0, 0, GEN_MODE_ID));
}

#define GEN_MODE_MASK_SETGENMODE ((0x00000F << GEN_MODE_NTEX_SHIFT) | (0x000003 << GEN_MODE_NCOL_SHIFT) | (0x00000F << GEN_MODE_NTEV_SHIFT))

void GDSetGenMode(u8 nTexGens, u8 nChans, u8 nTevs) {
    GDWriteBPCmd(SS_MASK(GEN_MODE_MASK_SETGENMODE));
    GDWriteBPCmd(GEN_MODE(nTexGens, nChans, 0, (nTevs - 1), 0, 0, 0, GEN_MODE_ID));
    GDWriteXFCmd(XF_NUMCOLORS_ID, XF_NUMCOLORS(nChans));
    GDWriteXFCmd(XF_NUMTEX_ID, XF_NUMTEX(nTexGens));
}

#define GEN_MODE_MASK_SETGENMODE2                                                                                                                    \
    ((0x00000F << GEN_MODE_NTEX_SHIFT) | (0x000003 << GEN_MODE_NCOL_SHIFT) | (0x00000F << GEN_MODE_NTEV_SHIFT) |                                     \
     (0x000003 << GEN_MODE_REJECT_EN_SHIFT) | (0x000007 << GEN_MODE_NBMP_SHIFT))

void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm) {
    static u8 cm2hw[] = {0, 2, 1, 3};
    GDWriteBPCmd(SS_MASK(GEN_MODE_MASK_SETGENMODE2));
    GDWriteBPCmd(GEN_MODE(nTexGens, nChans, 0, (nTevs - 1), cm2hw[cm], nInds, 0, GEN_MODE_ID));
    GDWriteXFCmd(XF_NUMCOLORS_ID, XF_NUMCOLORS(nChans));
    GDWriteXFCmd(XF_NUMTEX_ID, XF_NUMTEX(nTexGens));
}
