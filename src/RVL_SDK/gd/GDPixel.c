#include "revolution/gd/GDPixel.h"
#include "revolution/gd.h"
#include "revolution/gx.h"

void GDSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    f32 A, B, B_mant, C, A_f;
    u32 b_expn, b_m, a_hex, c_hex;
    u32 fsel, proj;

    fsel = (u32)(type & 0x07);
    proj = (u32)((type >> 3) & 0x01);

    if (proj) {
        if ((farz == nearz) || (endz == startz)) {
            A_f = 0.0f;
            C = 0.0f;
        } else {
            A = 1.0f / (endz - startz);
            A_f = (farz - nearz) * A;
            C = (startz - nearz) * A;
        }

        b_expn = 0;
        b_m = 0;
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
        b_expn = 1;
        while (B_mant > 1.0) {
            B_mant /= 2;
            b_expn++;
        }
        while ((B_mant > 0) && (B_mant < 0.5)) {
            B_mant *= 2;
            b_expn--;
        }

        A_f = A / (1 << (b_expn));
        b_m = (u32)(B_mant * 8388638);
    }

    a_hex = (*(u32*)&A_f);
    c_hex = (*(u32*)&C);

    GDWriteBPCmd(TEV_FOG_PARAM_0_PS((a_hex >> 12), TEV_FOG_PARAM_0_ID));
    GDWriteBPCmd(TEV_FOG_PARAM_1(b_m, TEV_FOG_PARAM_1_ID));
    GDWriteBPCmd(TEV_FOG_PARAM_2(b_expn, TEV_FOG_PARAM_2_ID));
    GDWriteBPCmd(TEV_FOG_PARAM_3_PS((c_hex >> 12), proj, fsel, TEV_FOG_PARAM_3_ID));
    GDWriteBPCmd(TEV_FOG_COLOR(color.b, color.g, color.r, TEV_FOG_COLOR_ID));
}

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp logic_op) {
    GDWriteBPCmd(SS_MASK(PE_CMODE1_MASK_SETBLENDMODE));

    GDWriteBPCmd(PE_CMODE0(((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT)), (type == GX_BM_LOGIC), 0, 0, 0, dst_factor, src_factor,
                           (type == GX_BM_SUBTRACT), logic_op, PE_CMODE0_ID));
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp logic_op, GXBool color_update_enable,
                       GXBool alpha_update_enable, GXBool dither_enable) {
    GDWriteBPCmd(PE_CMODE0(((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT)), (type == GX_BM_LOGIC), dither_enable, color_update_enable,
                           alpha_update_enable, dst_factor, src_factor, (type == GX_BM_SUBTRACT), logic_op, PE_CMODE0_ID));
}

void GDSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {
    GDWriteBPCmd(PE_ZMODE(compare_enable, func, update_enable, PE_ZMODE_ID));
}

void GDSetDstAlpha(GXBool enable, u8 alpha) {
    GDWriteBPCmd(PE_CMODE1(alpha, enable, PE_CMODE1_ID));
}
