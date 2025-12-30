#include "revolution/gd/GDLight.h"
#include "revolution/gd.h"
#include "revolution/gx.h"

void GDSetChanAmbColor(GXChannelID chan, GXColor color) {
    GDWriteXFCmd((u16)(XF_AMBIENT0_ID + (chan & 1)), (((u32)color.r << 24) | ((u32)color.g << 16) | ((u32)color.b << 8) | ((u32)color.a)));
}

void GDSetChanMatColor(GXChannelID chan, GXColor color) {
    GDWriteXFCmd((u16)(XF_MATERIAL0_ID + (chan & 1)), (((u32)color.r << 24) | ((u32)color.g << 16) | ((u32)color.b << 8) | ((u32)color.a)));
}

void GDSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn) {
    u32 reg = XF_COLOR0CNTRL_F_PS(mat_src, enable, (light_mask & 0x0f), amb_src, ((attn_fn == GX_AF_SPEC) ? GX_DF_NONE : diff_fn),
                                  (attn_fn != GX_AF_NONE), (attn_fn != GX_AF_SPEC), ((light_mask >> 4) & 0x0f));
    GDWriteXFCmd((u16)(XF_COLOR0CNTRL_ID + (chan & 3)), reg);

    if (chan == GX_COLOR0A0 || chan == GX_COLOR1A1) {
        GDWriteXFCmd((u16)(XF_COLOR0CNTRL_ID + (chan - 2)), reg);
    }
}
