#ifndef GDGEOMETRY_H
#define GDGEOMETRY_H

#include "revolution/gd/GDBase.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GEN_MODE_ID 0x00000000

#define CP_MATINDEX_A_ID 0x30
#define CP_MATINDEX_B_ID 0x40
#define CP_VCD_LO_ID 0x50
#define CP_VCD_HI_ID 0x60
#define CP_VAT_A_ID 0x70
#define CP_VAT_B_ID 0x80
#define CP_VAT_C_ID 0x90
#define CP_ARRAY_BASE_ID 0xa0
#define CP_ARRAY_STRIDE_ID 0xb0

#define XF_INVTXSPEC_ID 0x1008
#define XF_NUMCOLORS_ID 0x1009
#define XF_NUMTEX_ID 0x103f
#define XF_TEX0_ID 0x1040
#define XF_TEX1_ID 0x1041
#define XF_TEX2_ID 0x1042
#define XF_TEX3_ID 0x1043
#define XF_TEX4_ID 0x1044
#define XF_TEX5_ID 0x1045
#define XF_TEX6_ID 0x1046
#define XF_TEX7_ID 0x1047
#define XF_DUALTEX0_ID 0x1050
#define XF_DUALTEX1_ID 0x1051
#define XF_DUALTEX2_ID 0x1052
#define XF_DUALTEX3_ID 0x1053
#define XF_DUALTEX4_ID 0x1054
#define XF_DUALTEX5_ID 0x1055
#define XF_DUALTEX6_ID 0x1056
#define XF_DUALTEX7_ID 0x1057

#define XF_TEX_ST 0x00000000
#define XF_TEX_STQ 0x00000001

#define XF_TEX_AB11 0x00000000
#define XF_TEX_ABC1 0x00000001

#define XF_TEXGEN_REGULAR 0x00000000
#define XF_TEXGEN_EMBOSS_MAP 0x00000001
#define XF_TEXGEN_COLOR_STRGBC0 0x00000002
#define XF_TEXGEN_COLOR_STRGBC1 0x00000003

#define XF_GEOM_INROW 0x00000000
#define XF_NORMAL_INROW 0x00000001
#define XF_COLORS_INROW 0x00000002
#define XF_BINORMAL_T_INROW 0x00000003
#define XF_BINORMAL_B_INROW 0x00000004
#define XF_TEX0_INROW 0x00000005
#define XF_TEX1_INROW 0x00000006
#define XF_TEX2_INROW 0x00000007
#define XF_TEX3_INROW 0x00000008
#define XF_TEX4_INROW 0x00000009
#define XF_TEX5_INROW 0x0000000a
#define XF_TEX6_INROW 0x0000000b
#define XF_TEX7_INROW 0x0000000c

#define CP_VCD_REG_LO_PMIDX_SHIFT 0
#define CP_VCD_REG_LO_T0MIDX_SHIFT 1
#define CP_VCD_REG_LO_T1MIDX_SHIFT 2
#define CP_VCD_REG_LO_T2MIDX_SHIFT 3
#define CP_VCD_REG_LO_T3MIDX_SHIFT 4
#define CP_VCD_REG_LO_T4MIDX_SHIFT 5
#define CP_VCD_REG_LO_T5MIDX_SHIFT 6
#define CP_VCD_REG_LO_T6MIDX_SHIFT 7
#define CP_VCD_REG_LO_T7MIDX_SHIFT 8
#define CP_VCD_REG_LO_POS_SHIFT 9
#define CP_VCD_REG_LO_NRM_SHIFT 11
#define CP_VCD_REG_LO_COL0_SHIFT 13
#define CP_VCD_REG_LO_COL1_SHIFT 15
#define CP_VCD_REG_LO(pmidx, t0midx, t1midx, t2midx, t3midx, t4midx, t5midx, t6midx, t7midx, pos, nrm, col0, col1)                                   \
    ((((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT) | (((unsigned long)(t0midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT) |                           \
     (((unsigned long)(t1midx)) << CP_VCD_REG_LO_T1MIDX_SHIFT) | (((unsigned long)(t2midx)) << CP_VCD_REG_LO_T2MIDX_SHIFT) |                         \
     (((unsigned long)(t3midx)) << CP_VCD_REG_LO_T3MIDX_SHIFT) | (((unsigned long)(t4midx)) << CP_VCD_REG_LO_T4MIDX_SHIFT) |                         \
     (((unsigned long)(t5midx)) << CP_VCD_REG_LO_T5MIDX_SHIFT) | (((unsigned long)(t6midx)) << CP_VCD_REG_LO_T6MIDX_SHIFT) |                         \
     (((unsigned long)(t7midx)) << CP_VCD_REG_LO_T7MIDX_SHIFT) | (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT) |                               \
     (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT) | (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT) |                                   \
     (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT))

#define CP_VCD_REG_LO_PS(pmidx, t76543210midx, pos, nrm, col0, col1)                                                                                 \
    ((((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT) | (((unsigned long)(t76543210midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT) |                    \
     (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT) | (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT) |                                     \
     (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT) | (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT))

#define CP_VCD_REG_HI_TEX0_SHIFT 0
#define CP_VCD_REG_HI_TEX1_SHIFT 2
#define CP_VCD_REG_HI_TEX2_SHIFT 4
#define CP_VCD_REG_HI_TEX3_SHIFT 6
#define CP_VCD_REG_HI_TEX4_SHIFT 8
#define CP_VCD_REG_HI_TEX5_SHIFT 10
#define CP_VCD_REG_HI_TEX6_SHIFT 12
#define CP_VCD_REG_HI_TEX7_SHIFT 14
#define CP_VCD_REG_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7)                                                                                \
    ((((unsigned long)(tex0)) << CP_VCD_REG_HI_TEX0_SHIFT) | (((unsigned long)(tex1)) << CP_VCD_REG_HI_TEX1_SHIFT) |                                 \
     (((unsigned long)(tex2)) << CP_VCD_REG_HI_TEX2_SHIFT) | (((unsigned long)(tex3)) << CP_VCD_REG_HI_TEX3_SHIFT) |                                 \
     (((unsigned long)(tex4)) << CP_VCD_REG_HI_TEX4_SHIFT) | (((unsigned long)(tex5)) << CP_VCD_REG_HI_TEX5_SHIFT) |                                 \
     (((unsigned long)(tex6)) << CP_VCD_REG_HI_TEX6_SHIFT) | (((unsigned long)(tex7)) << CP_VCD_REG_HI_TEX7_SHIFT))

#define XF_INVTXSPEC_HOST_COLORS_SHIFT 0
#define XF_INVTXSPEC_HOST_NORMAL_SHIFT 2
#define XF_INVTXSPEC_HOST_TEXTURES_SHIFT 4
#define XF_INVTXSPEC(host_colors, host_normal, host_textures)                                                                                        \
    ((((unsigned long)(host_colors)) << XF_INVTXSPEC_HOST_COLORS_SHIFT) | (((unsigned long)(host_normal)) << XF_INVTXSPEC_HOST_NORMAL_SHIFT) |       \
     (((unsigned long)(host_textures)) << XF_INVTXSPEC_HOST_TEXTURES_SHIFT))

#define CP_ARRAY_BASE_REG_BASE_SHIFT 0
#define CP_ARRAY_BASE_REG(base) ((((unsigned long)(base)) << CP_ARRAY_BASE_REG_BASE_SHIFT))

#define CP_ARRAY_STRIDE_REG_STRIDE_SHIFT 0
#define CP_ARRAY_STRIDE_REG(stride) ((((unsigned long)(stride)) << CP_ARRAY_STRIDE_REG_STRIDE_SHIFT))

#define XF_TEX_PROJECTION_SHIFT 1
#define XF_TEX_INPUT_FORM_SHIFT 2
#define XF_TEX_TEXGEN_TYPE_SHIFT 4
#define XF_TEX_SOURCE_ROW_SHIFT 7
#define XF_TEX_EMBOSS_SOURCE_SHIFT 12
#define XF_TEX_EMBOSS_LIGHT_SHIFT 15
#define XF_TEX(projection, input_form, texgen_type, source_row, emboss_source, emboss_light)                                                         \
    ((((unsigned long)(projection)) << XF_TEX_PROJECTION_SHIFT) | (((unsigned long)(input_form)) << XF_TEX_INPUT_FORM_SHIFT) |                       \
     (((unsigned long)(texgen_type)) << XF_TEX_TEXGEN_TYPE_SHIFT) | (((unsigned long)(source_row)) << XF_TEX_SOURCE_ROW_SHIFT) |                     \
     (((unsigned long)(emboss_source)) << XF_TEX_EMBOSS_SOURCE_SHIFT) | (((unsigned long)(emboss_light)) << XF_TEX_EMBOSS_LIGHT_SHIFT))

#define XF_DUALTEX_DUALMTX_SHIFT 0
#define XF_DUALTEX_NORMAL_ENABLE_SHIFT 8
#define XF_DUALTEX(dualmtx, normal_enable)                                                                                                           \
    ((((unsigned long)(dualmtx)) << XF_DUALTEX_DUALMTX_SHIFT) | (((unsigned long)(normal_enable)) << XF_DUALTEX_NORMAL_ENABLE_SHIFT))

#define GEN_MODE_NTEX_SHIFT 0
#define GEN_MODE_NCOL_SHIFT 4
#define GEN_MODE_MS_EN_SHIFT 9
#define GEN_MODE_NTEV_SHIFT 10
#define GEN_MODE_REJECT_EN_SHIFT 14
#define GEN_MODE_NBMP_SHIFT 16
#define GEN_MODE_ZFREEZE_SHIFT 19
#define GEN_MODE_RID_SHIFT 24
#define GEN_MODE(ntex, ncol, ms_en, ntev, reject_en, nbmp, zfreeze, rid)                                                                             \
    ((((unsigned long)(ntex)) << GEN_MODE_NTEX_SHIFT) | (((unsigned long)(ncol)) << GEN_MODE_NCOL_SHIFT) |                                           \
     (((unsigned long)(ms_en)) << GEN_MODE_MS_EN_SHIFT) | (((unsigned long)(ntev)) << GEN_MODE_NTEV_SHIFT) |                                         \
     (((unsigned long)(reject_en)) << GEN_MODE_REJECT_EN_SHIFT) | (((unsigned long)(nbmp)) << GEN_MODE_NBMP_SHIFT) |                                 \
     (((unsigned long)(zfreeze)) << GEN_MODE_ZFREEZE_SHIFT) | (((unsigned long)(rid)) << GEN_MODE_RID_SHIFT))

#define XF_NUMCOLORS_SHIFT 0
#define XF_NUMCOLORS(numcolors) ((((unsigned long)(numcolors)) << XF_NUMCOLORS_SHIFT))

#define XF_NUMTEX_SHIFT 0
#define XF_NUMTEX(numtex) ((((unsigned long)(numtex)) << XF_NUMTEX_SHIFT))

static void GDBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);

#define XF_REG_TEX(proj, form, tgType, row, embossRow, embossLit)                                                                                    \
    ((proj) << 1 | (form) << 2 | (tgType) << 4 | (row) << 7 | (embossRow) << 12 | (embossLit) << 15)

#define XF_REG_DUALTEX(mtx, normalize) ((mtx) << 0 | (normalize) << 8)

#define BP_GEN_MODE(nTexGens, nChans, nTevs, p4, nInds)                                                                                              \
    ((u32)(nTexGens) << 0 | (u32)(nChans) << 4 | (u32)(nTevs) << 10 | (u32)(p4) << 14 | (u32)(nInds) << 16)

#define CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType, c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt, tx0Type, tx0Frac, p12, nrmIdx3)             \
    ((posCnt) << 0 | (posType) << 1 | (posFrac) << 4 | (nrmCnt) << 9 | (nrmType) << 10 | (c0Cnt) << 13 | (c0Type) << 14 | (c1Cnt) << 17 |            \
     (c1Type) << 18 | (tx0Cnt) << 21 | (tx0Type) << 22 | (tx0Frac) << 25 | (p12) << 30 | (nrmIdx3) << 31)

#define CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type, tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt, tx4Type, p11)                          \
    ((tx1Cnt) << 0 | (tx1Type) << 1 | (tx1Frac) << 4 | (tx2Cnt) << 9 | (tx2Type) << 10 | (tx2Frac) << 13 | (tx3Cnt) << 18 | (tx3Type) << 19 |        \
     (tx3Frac) << 22 | (tx4Cnt) << 27 | (tx4Type) << 28 | p11 << 31)

#define CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt, tx6Type, tx6Frac, tx7Cnt, tx7Type, tx7Frac)                                       \
    ((tx4Frac) << 0 | (tx5Cnt) << 5 | (tx5Type) << 6 | (tx5Frac) << 9 | (tx6Cnt) << 14 | (tx6Type) << 15 | (tx6Frac) << 18 | (tx7Cnt) << 23 |        \
     (tx7Type) << 24 | (tx7Frac) << 27)

void GDSetArray(GXAttr attr, const void* base_ptr, u8 stride);
void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride);
void GDSetVtxDescv(const GXVtxDescList* attrPtr);

#ifdef __cplusplus
}
#endif

#endif  // GDGEOMETRY_H
