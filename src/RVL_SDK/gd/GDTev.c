#include "revolution/gd/GDTev.h"
#include "revolution/gd.h"
#include "revolution/gx.h"

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d, GXTevOp op, GXTevBias bias,
                       GXTevScale scale, GXBool clamp, GXTevRegID out_reg) {
    if (op <= GX_TEV_SUB) {
        GDWriteBPCmd(TEV_COLOR_ENV(d, c, b, a, bias, (op & 1), clamp, scale, out_reg, TEV_COLOR_ENV_0_ID + 2 * (u32)stage));
    } else {
        GDWriteBPCmd(TEV_COLOR_ENV(d, c, b, a, GX_MAX_TEVBIAS, (op & 1), clamp, ((op >> 1) & 3), out_reg, TEV_COLOR_ENV_0_ID + 2 * (u32)stage));
    }
}

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d, GXTevOp op, GXTevBias bias,
                              GXTevScale scale, GXBool clamp, GXTevRegID out_reg, GXTevSwapSel ras_sel, GXTevSwapSel tex_sel) {
    if (op <= GX_TEV_SUB) {
        GDWriteBPCmd(TEV_ALPHA_ENV(ras_sel, tex_sel, d, c, b, a, bias, (op & 1), clamp, scale, out_reg, TEV_ALPHA_ENV_0_ID + 2 * (u32)stage));
    } else {
        GDWriteBPCmd(TEV_ALPHA_ENV(ras_sel, tex_sel, d, c, b, a, GX_MAX_TEVBIAS, (op & 1), clamp, ((op >> 1) & 3), out_reg,
                                   TEV_ALPHA_ENV_0_ID + 2 * (u32)stage));
    }
}

void GDSetTevColor(GXTevRegID reg, GXColor color) {
    u32 regRA, regBG;

    regRA = TEV_REGISTERL(color.r, color.a, TEV_COLOR_REG, TEV_REGISTERL_0_ID + reg * 2);
    regBG = TEV_REGISTERH(color.b, color.g, TEV_COLOR_REG, TEV_REGISTERH_0_ID + reg * 2);
    GDWriteBPCmd(regRA);
    GDWriteBPCmd(regBG);
    GDWriteBPCmd(regBG);
    GDWriteBPCmd(regBG);
}

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1) {
    GDWriteBPCmd(TEV_ALPHAFUNC(ref0, ref1, comp0, comp1, op, TEV_ALPHAFUNC_ID));
}

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0, GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1) {
    static u8 c2r[] = {RAS1_CC_0,  RAS1_CC_1, RAS1_CC_0, RAS1_CC_1, RAS1_CC_0, RAS1_CC_1, RAS1_CC_Z, RAS1_CC_B,
                       RAS1_CC_BN, 0,         0,         0,         0,         0,         0,         RAS1_CC_Z};

    GDWriteBPCmd(RAS1_TREF((map0 & 7), (coord0 & 7), ((map0 != GX_TEXMAP_NULL) && !(map0 & GX_TEX_DISABLE)), c2r[color0 & 0xF], (map1 & 7),
                           (coord1 & 7), ((map1 != GX_TEXMAP_NULL) && !(map1 & GX_TEX_DISABLE)), c2r[color1 & 0xF],
                           (RAS1_TREF0_ID + (evenStage / 2))));
}
