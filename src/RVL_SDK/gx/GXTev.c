#include "revolution/gx/GXTev.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXRegs.h"
#include <revolution/gx.h>

static tev_color_env_t TEVCOpTableST0[] = {
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC,
        GX_CC_RASC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_RASC,
        GX_CC_TEXC,
        GX_CC_TEXA,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_RASC,
        GX_CC_ONE, 
        GX_CC_TEXC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_RASC  
    },
};

static tev_color_env_t TEVCOpTableST1[] =
{
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC,
        GX_CC_CPREV,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_CPREV,
        GX_CC_TEXC,
        GX_CC_TEXA,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_CPREV,
        GX_CC_ONE, 
        GX_CC_TEXC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_CPREV 
    },
};

static tev_alpha_env_t TEVAOpTableST0[] =
{
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_RASA,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_RASA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_RASA,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_RASA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
};

static tev_alpha_env_t TEVAOpTableST1[] =
{
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_APREV,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_APREV,
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_APREV,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_APREV,
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
};

void GXSetTevOp(GXTevStageID id, GXTevMode mode) {
    u32 *ctmp, *atmp, tevReg;

    if (id == GX_TEVSTAGE0) {
        ctmp = (u32*)&TEVCOpTableST0[mode];
        atmp = (u32*)&TEVAOpTableST0[mode];
    }
    else {
        ctmp = (u32*)&TEVCOpTableST1[mode];
        atmp = (u32*)&TEVAOpTableST1[mode];
    }

    tevReg = gx->tevc[id];
    tevReg = ((*ctmp & ~0xFF000000) | (tevReg & 0xFF000000));
    GX_WRITE_RA_REG(tevReg);
    gx->tevc[id] = tevReg;

    tevReg = gx->teva[id];
    tevReg = ((*atmp & ~0xFF00000F) | (tevReg & 0xFF00000F));
    GX_WRITE_RA_REG(tevReg);
    gx->teva[id] = tevReg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevColorIn( GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d ) {
    u32 tevReg = gx->tevc[stage];
    SC_TEV_COLOR_ENV_SET_SELA(tevReg, a);
    SC_TEV_COLOR_ENV_SET_SELB(tevReg, b);
    SC_TEV_COLOR_ENV_SET_SELC(tevReg, c);
    SC_TEV_COLOR_ENV_SET_SELD(tevReg, d);
    GX_WRITE_RA_REG(tevReg);
    gx->tevc[stage] = tevReg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) {
    u32 tevReg = gx->teva[stage];
    SC_TEV_ALPHA_ENV_SET_SELA(tevReg, a);
    SC_TEV_ALPHA_ENV_SET_SELB(tevReg, b);
    SC_TEV_ALPHA_ENV_SET_SELC(tevReg, c);
    SC_TEV_ALPHA_ENV_SET_SELD(tevReg, d);
    GX_WRITE_RA_REG(tevReg);
    gx->teva[stage] = tevReg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg ) {
    u32 tevReg = gx->tevc[stage];
    SC_TEV_COLOR_ENV_SET_SUB(tevReg, (op & 1));

    if (op <= GX_TEV_SUB) {
        SC_TEV_COLOR_ENV_SET_SHIFT(tevReg, scale);
        SC_TEV_COLOR_ENV_SET_BIAS(tevReg, bias);
    } else {
        SC_TEV_COLOR_ENV_SET_SHIFT(tevReg, ((op >> 1) & 3));
        SC_TEV_COLOR_ENV_SET_BIAS(tevReg, GX_MAX_TEVBIAS);
    }

    SC_TEV_COLOR_ENV_SET_CLAMP(tevReg, clamp);
    SC_TEV_COLOR_ENV_SET_DEST(tevReg, out_reg);
    GX_WRITE_RA_REG(tevReg);
    gx->tevc[stage] = tevReg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg) {
    u32 tevReg = gx->teva[stage];
    SC_TEV_ALPHA_ENV_SET_SUB(tevReg, (op & 1));
    if (op <= GX_TEV_SUB) {
        SC_TEV_ALPHA_ENV_SET_SHIFT(tevReg, scale);
        SC_TEV_ALPHA_ENV_SET_BIAS(tevReg, bias);
    } else {
        SC_TEV_ALPHA_ENV_SET_SHIFT(tevReg, ((op >> 1) & 3));
        SC_TEV_ALPHA_ENV_SET_BIAS(tevReg, GX_MAX_TEVBIAS);
    }

    SC_TEV_ALPHA_ENV_SET_CLAMP(tevReg, clamp);
    SC_TEV_ALPHA_ENV_SET_DEST(tevReg, out_reg);
    GX_WRITE_RA_REG(tevReg);
    gx->teva[stage] = tevReg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevColor(GXTevRegID id, GXColor color) {
    u32 rgba, regRA, regBG;
    rgba = *(u32*)(&color);
    regRA = TEV_REGISTERL(0, 0, (0xE0 + id * 2));
    SC_TEV_REGISTERL_SET_R8(regRA, (rgba >> 24) & 0xFF);
    SC_TEV_REGISTERL_SET_A8(regRA, rgba & 0xFF);

    regBG = TEV_REGISTERH(0, 0, (0xE1 + id * 2));
    SC_TEV_REGISTERH_SET_B8(regBG, (rgba >> 8) & 0xFF);
    SC_TEV_REGISTERH_SET_G8(regBG, (rgba >> 16) & 0xFF);

    GX_WRITE_RA_REG(regRA);
    GX_WRITE_RA_REG(regBG);
    GX_WRITE_RA_REG(regBG);
    GX_WRITE_RA_REG(regBG);
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevColorS10(GXTevRegID id, GXColorS10 color) {
    u32 sRG, sBA, regRA, regBG;
    sRG = *(u32*)(&color);
    sBA = *((u32*)(&color) + 1);
    regRA = TEV_REGISTERL(0, 0, (0xE0 + id * 2));
    SC_TEV_REGISTERL_SET_R(regRA, (sRG >> 16) & ((1 << TEV_REGISTERL_R_SIZE) - 1));
    SC_TEV_REGISTERL_SET_A(regRA, sBA & ((1 << TEV_REGISTERL_A_SIZE) - 1));

    regBG = TEV_REGISTERH(0, 0, (0xE1 + id * 2));
    SC_TEV_REGISTERH_SET_B(regBG, (sBA >> 16) & ((1 << TEV_REGISTERH_B_SIZE) - 1));
    SC_TEV_REGISTERH_SET_G(regBG, sRG & ((1 << TEV_REGISTERH_G_SIZE) - 1));

    GX_WRITE_RA_REG(regRA);
    GX_WRITE_RA_REG(regBG);
    GX_WRITE_RA_REG(regBG);
    GX_WRITE_RA_REG(regBG);
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevKColor(GXTevKColorID id, GXColor color) {
    u32 rgba, regRA, regBG;
    rgba = *(u32*)(&color);
    regRA = TEV_KREGISTERL(0, 0, (0xE0 + id * 2));
    SC_TEV_KREGISTERL_SET_R(regRA, (rgba >> 24) & 0xFF);
    SC_TEV_KREGISTERL_SET_A(regRA, rgba & 0xFF);
    SC_TEV_KREGISTERL_SET_PAD1(regRA, 0x08);

    regBG = TEV_KREGISTERH(0, 0, (0xE1 + id * 2));
    SC_TEV_KREGISTERH_SET_B(regBG, (rgba >> 8) & 0xFF);
    SC_TEV_KREGISTERH_SET_G(regBG, (rgba >> 16) & 0xFF);
    SC_TEV_KREGISTERH_SET_PAD1(regBG, 0x08);

    GX_WRITE_RA_REG(regRA);
    GX_WRITE_RA_REG(regBG); 
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel) {
    u32* reg = &gx->tevKsel[stage >> 1];
    if (stage & 1) {
        SC_TEV_KSEL_SET_KCSEL1((*reg), sel);
    }
    else {
        SC_TEV_KSEL_SET_KCSEL0((*reg), sel);
    }

    GX_WRITE_RA_REG((*reg));
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel) {
    u32* reg = &gx->tevKsel[stage >> 1];
    if (stage & 1) {
        SC_TEV_KSEL_SET_KASEL1((*reg), sel);
    }
    else {
        SC_TEV_KSEL_SET_KASEL0((*reg), sel);
    }

    GX_WRITE_RA_REG((*reg));
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel ras_sel, GXTevSwapSel tex_sel) {
    u32* reg = &gx->teva[stage];
    SC_TEV_ALPHA_ENV_SET_MODE((*reg), ras_sel);
    SC_TEV_ALPHA_ENV_SET_SWAP((*reg), tex_sel);
    GX_WRITE_RA_REG((*reg));
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red, GXTevColorChan green, GXTevColorChan blue, GXTevColorChan alpha) {
    u32* reg = &gx->tevKsel[table << 1];
    SC_TEV_KSEL_SET_XRB((*reg), red);
    SC_TEV_KSEL_SET_XGA((*reg), green);
    GX_WRITE_RA_REG((*reg));

    reg = &gx->tevKsel[(table << 1) + 1];
    SC_TEV_KSEL_SET_XRB((*reg), blue);
    SC_TEV_KSEL_SET_XGA((*reg), alpha);
    GX_WRITE_RA_REG((*reg));
    gx->bpSentNot = GX_FALSE;
}

void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1) {
    u32 reg = 0xF3000000;
    SC_TEV_ALPHAFUNC_SET_A0(reg, ref0);
    SC_TEV_ALPHAFUNC_SET_A1(reg, ref1);
    SC_TEV_ALPHAFUNC_SET_OP0(reg, comp0);
    SC_TEV_ALPHAFUNC_SET_OP1(reg, comp1);
    SC_TEV_ALPHAFUNC_SET_LOGIC(reg, op);
    GX_WRITE_RA_REG(reg);
    gx->bpSentNot = GX_FALSE;
}

void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias) {
    u32 zenv0, zenv1, type;
    zenv0 = 0;
    SC_TEV_Z_ENV_0_SET_ZOFF(zenv0, bias);
    SC_TEV_Z_ENV_0_SET_RID(zenv0, 0xF4);

    zenv1 = 0;
    switch (fmt) {
        case GX_TF_Z8:
            type = 0;
            break;
        case GX_TF_Z16:
            type = 1;
            break;
        case GX_TF_Z24X8:
            type = 2;
            break;
        default:
            type = 2;
            break;
    }

    SC_TEV_Z_ENV_1_SET_TYPE(zenv1, type);
    SC_TEV_Z_ENV_1_SET_OP(zenv1, op);
    SC_TEV_Z_ENV_1_SET_RID(zenv1, 0xF5);
   
    GX_WRITE_RA_REG(zenv0);
    GX_WRITE_RA_REG(zenv1);
    gx->bpSentNot = GX_FALSE;
}

void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color) {
    u32* ptref, tmap, tcoord;
    static u32 c2r[] = {0,1,0,1,0,1,7,5,6, 0, 0, 1, 2, 3, 4, 4, 4, 5};
    ptref = &gx->tref[stage / 2];
    gx->texmapId[stage] = map;
    tmap = (u32)(map & ~GX_TEX_DISABLE);
    tmap = (tmap >= GX_MAX_TEXMAP) ? GX_TEXMAP0 : tmap;

    if (coord >= GX_MAX_TEXCOORD) {
        tcoord = GX_TEXCOORD0;
        gx->tevTcEnab = gx->tevTcEnab & ~(1 << stage);
    }
    else {
        tcoord = coord;
        gx->tevTcEnab = gx->tevTcEnab | (1 << stage);
    }

    if (stage & 1) {
        SC_RAS1_TREF_SET_TI1(*ptref, tmap);
        SC_RAS1_TREF_SET_TC1(*ptref, tcoord);
        SC_RAS1_TREF_SET_CC1(*ptref, ((color == GX_COLOR_NULL) ? 7 : c2r[color]));
        SC_RAS1_TREF_SET_TE1(*ptref, ((map != GX_TEXMAP_NULL) && !(map & GX_TEX_DISABLE)));
    }
    else {
        SC_RAS1_TREF_SET_TI0(*ptref, tmap);
        SC_RAS1_TREF_SET_TC0(*ptref, tcoord);
        SC_RAS1_TREF_SET_CC0(*ptref, ((color == GX_COLOR_NULL) ? 7 : c2r[color]));
        SC_RAS1_TREF_SET_TE0(*ptref, ((map != GX_TEXMAP_NULL) && !(map & GX_TEX_DISABLE)));
    }

    GX_WRITE_RA_REG(*ptref);
    gx->bpSentNot = GX_FALSE;
    gx->dirtyState |= 1;
}

void GXSetNumTevStages(u8 nStages) {
    SC_GEN_MODE_SET_NTEV(gx->genMode, (nStages - 1));
    gx->dirtyState |= 4;
}
