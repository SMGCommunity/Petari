#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>
#include <revolution/gx/GXTypes.h>

void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    u32 fogclr = 0, fog0 = 0, fog1 = 0, fog2 = 0, fog3 = 0;
    f32 A, B, B_mant, C, a, c;
    u32 B_expn, b_m, b_s, a_hex, c_hex;
    u32 fsel, proj;
    u32 rgba;

    fsel = (u32)(type & 0x07);
    proj = (u32)((type >> 3) & 0x01);

    if (proj) {
        if ((farz == nearz) || (endz == startz)) {
            a = 0.0f;
            c = 0.0f;
        } else {
            A = 1.0F / (endz - startz);
            a = (farz - nearz) * A;
            c = (startz - nearz) * A;
        }
    } else {
        if ((farz == nearz) || (endz == startz)) {
            A = 0.0f;
            B = 0.5f;
            C = 0.0f;
        } else {
            A = (farz * nearz) / ((farz - nearz) * (endz - startz));
            B = farz / (farz - nearz);
            C = startz / (endz - startz);
        }

        B_mant = B;
        B_expn = 0;
        while (B_mant > 1.0) {
            B_mant /= 2;
            B_expn++;
        }
        while ((B_mant > 0) && (B_mant < 0.5)) {
            B_mant *= 2;
            B_expn--;
        }

        a = A / (1 << (B_expn + 1));
        b_m = (u32)(B_mant * 8388638);
        b_s = B_expn + 1;
        c = C;

        SC_TEV_FOG_PARAM_1_SET_B_MAG(fog1, b_m);
        SC_TEV_FOG_PARAM_1_SET_RID(fog1, 0xEF);

        SC_TEV_FOG_PARAM_2_SET_B_SHF(fog2, b_s);
        SC_TEV_FOG_PARAM_2_SET_RID(fog2, 0xF0);
    }

    a_hex = (*(u32*)&a);
    c_hex = (*(u32*)&c);

    SC_TEV_FOG_PARAM_0_SET_A_MANT(fog0, ((a_hex >> 12) & 0x7ff));
    SC_TEV_FOG_PARAM_0_SET_A_EXPN(fog0, ((a_hex >> 23) & 0x0ff));
    SC_TEV_FOG_PARAM_0_SET_A_SIGN(fog0, (a_hex >> 31));
    SC_TEV_FOG_PARAM_0_SET_RID(fog0, 0xEE);

    SC_TEV_FOG_PARAM_3_SET_C_MANT(fog3, ((c_hex >> 12) & 0x7ff));
    SC_TEV_FOG_PARAM_3_SET_C_EXPN(fog3, ((c_hex >> 23) & 0x0ff));
    SC_TEV_FOG_PARAM_3_SET_C_SIGN(fog3, (c_hex >> 31));
    SC_TEV_FOG_PARAM_3_SET_PROJ(fog3, proj);
    SC_TEV_FOG_PARAM_3_SET_FSEL(fog3, fsel);
    SC_TEV_FOG_PARAM_3_SET_RID(fog3, 0xF1);

    rgba = *(u32*)(&color);
    SC_TEV_FOG_COLOR_SET_RGB(fogclr, (rgba >> 8));
    SC_TEV_FOG_COLOR_SET_RID(fogclr, 0xF2);

    GX_WRITE_RA_REG(fog0);
    GX_WRITE_RA_REG(fog1);
    GX_WRITE_RA_REG(fog2);
    GX_WRITE_RA_REG(fog3);
    GX_WRITE_RA_REG(fogclr);
    BP_SENT();
}

void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable* table) {
    u32 i;
    u32 range_adj, range_c;

    if (enable) {
        for (i = 0; i < 10; i += 2) {
            range_adj = 0;
            SC_TEV_RANGE_ADJ_SET_R0(range_adj, table->r[i]);
            SC_TEV_RANGE_ADJ_SET_R1(range_adj, table->r[i + 1]);
            SC_TEV_RANGE_ADJ_SET_RID(range_adj, (0xE9 + (i / 2)));
            GX_WRITE_RA_REG(range_adj);
        }
    }

    range_c = 0;
    SC_TEV_RANGE_ADJ_C_SET_CENTER(range_c, center + (u16)342.0f);
    SC_TEV_RANGE_ADJ_C_SET_ENB(range_c, enable);
    SC_TEV_RANGE_ADJ_C_SET_RID(range_c, 0xE8);
    GX_WRITE_RA_REG(range_c);
    BP_SENT();
}

void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op) {
    u32 reg;
    u32 blend_en;

    reg = gx->cmode0;
    blend_en = (u32)(type & 0x01);

    SC_PE_CMODE0_SET_BLENDOP(reg, (type == GX_BM_SUBTRACT));

    SC_PE_CMODE0_SET_BLEND_ENABLE(reg, blend_en);
    SC_PE_CMODE0_SET_LOGICOP_ENABLE(reg, (type == GX_BM_LOGIC));
    SC_PE_CMODE0_SET_LOGICOP(reg, op);
    SC_PE_CMODE0_SET_SFACTOR(reg, src_factor);
    SC_PE_CMODE0_SET_DFACTOR(reg, dst_factor);
    GX_WRITE_RA_REG(reg);
    gx->cmode0 = reg;
    BP_SENT();
}

void GXSetColorUpdate(GXBool update_enable) {
    u32 reg;
    reg = gx->cmode0;
    SC_PE_CMODE0_SET_COLOR_MASK(reg, update_enable);
    GX_WRITE_RA_REG(reg);
    gx->cmode0 = reg;
    BP_SENT();
}

void GXSetAlphaUpdate(GXBool update_enable) {
    u32 reg;
    reg = gx->cmode0;
    SC_PE_CMODE0_SET_ALPHA_MASK(reg, update_enable);
    GX_WRITE_RA_REG(reg);
    gx->cmode0 = reg;
    BP_SENT();
}

void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {
    u32 reg;
    reg = gx->zmode;
    SC_PE_ZMODE_SET_ENABLE(reg, compare_enable);
    SC_PE_ZMODE_SET_FUNC(reg, func);
    SC_PE_ZMODE_SET_MASK(reg, update_enable);
    GX_WRITE_RA_REG(reg);
    gx->zmode = reg;
    BP_SENT();
}

void GXSetZCompLoc(GXBool before_tex) {
    SC_PE_CONTROL_SET_ZTOP(gx->peCtrl, before_tex);
    GX_WRITE_RA_REG(gx->peCtrl);
    BP_SENT();
}

void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt) {
    u32 oldPeCtrl;
    GXBool aa;
    static u32 p2f[] = {0, 1, 2, 3, 4, 5, 6, 7};

    oldPeCtrl = gx->peCtrl;

    SC_PE_CONTROL_SET_PIXTYPE(gx->peCtrl, p2f[pix_fmt]);
    SC_PE_CONTROL_SET_ZCMODE(gx->peCtrl, z_fmt);

    if (oldPeCtrl != gx->peCtrl) {
        GX_WRITE_RA_REG(gx->peCtrl);
        aa = (GXBool)((pix_fmt == 2) ? GX_TRUE : GX_FALSE);
        SC_GEN_MODE_SET_MS_EN(gx->genMode, aa);
        gx->dirtyState |= 4;
    }

    if (p2f[pix_fmt] == 4) {
        SC_PE_CMODE1_SET_YUV(gx->cmode1, ((pix_fmt - GX_PF_Y8) & 0x3));
        SC_PE_CMODE1_SET_RID(gx->cmode1, 0x42);
        GX_WRITE_RA_REG(gx->cmode1);
    }

    BP_SENT();
}

void GXSetDither(GXBool dither) {
    u32 reg;
    reg = gx->cmode0;
    SC_PE_CMODE0_SET_DITHER_ENABLE(reg, dither);
    GX_WRITE_RA_REG(reg);
    gx->cmode0 = reg;
    BP_SENT();
}

void GXSetDstAlpha(GXBool enable, u8 alpha) {
    u32 reg;
    reg = gx->cmode1;

    SC_PE_CMODE1_SET_CONSTANT_ALPHA(reg, alpha);
    SC_PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(reg, enable);
    GX_WRITE_RA_REG(reg);
    gx->cmode1 = reg;
    BP_SENT();
}

void GXSetFieldMask(GXBool odd_mask, GXBool even_mask) {
    u32 reg;
    reg = 0;
    SC_PE_FIELD_MASK_SET_EVEN(reg, even_mask);
    SC_PE_FIELD_MASK_SET_ODD(reg, odd_mask);
    SC_PE_FIELD_MASK_SET_RID(reg, 0x44);
    GX_WRITE_RA_REG(reg);
    BP_SENT();
}

void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio) {
    u32 reg;

    SC_SU_LPSIZE_SET_FIELDMODE(gx->lpSize, half_aspect_ratio);
    GX_WRITE_RA_REG(gx->lpSize);

    __GXFlushTextureState();
    reg = TX_MISC(field_mode, 0, 0x68);
    GX_WRITE_RA_REG(reg);
    __GXFlushTextureState();
}
