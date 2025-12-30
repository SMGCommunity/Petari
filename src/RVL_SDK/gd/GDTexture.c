#include "revolution/gd/GDTexture.h"
#include "revolution/gd.h"
#include "revolution/os.h"

u8 GDTexMode0Ids[8] = {
    TX_SETMODE0_I0_ID, TX_SETMODE0_I1_ID, TX_SETMODE0_I2_ID, TX_SETMODE0_I3_ID,
    TX_SETMODE0_I4_ID, TX_SETMODE0_I5_ID, TX_SETMODE0_I6_ID, TX_SETMODE0_I7_ID,
};

u8 GDTexMode1Ids[8] = {
    TX_SETMODE1_I0_ID, TX_SETMODE1_I1_ID, TX_SETMODE1_I2_ID, TX_SETMODE1_I3_ID,
    TX_SETMODE1_I4_ID, TX_SETMODE1_I5_ID, TX_SETMODE1_I6_ID, TX_SETMODE1_I7_ID,
};

u8 GDTexImage0Ids[8] = {
    TX_SETIMAGE0_I0_ID, TX_SETIMAGE0_I1_ID, TX_SETIMAGE0_I2_ID, TX_SETIMAGE0_I3_ID,
    TX_SETIMAGE0_I4_ID, TX_SETIMAGE0_I5_ID, TX_SETIMAGE0_I6_ID, TX_SETIMAGE0_I7_ID,
};

u8 GDTexImage3Ids[8] = {
    TX_SETIMAGE3_I0_ID, TX_SETIMAGE3_I1_ID, TX_SETIMAGE3_I2_ID, TX_SETIMAGE3_I3_ID,
    TX_SETIMAGE3_I4_ID, TX_SETIMAGE3_I5_ID, TX_SETIMAGE3_I6_ID, TX_SETIMAGE3_I7_ID,
};

u8 GD2HWFiltConv[] = {0, 4, 1, 5, 2, 6.};

void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod,
                        f32 max_lod, f32 lod_bias, GXBool bias_clamp, GXBool do_edge_lod, GXAnisotropy max_aniso) {
    GDWriteBPCmd(TX_SETMODE0(wrap_s, wrap_t, (mag_filt == GX_LINEAR), GD2HWFiltConv[min_filt], !do_edge_lod, (u8)((s8)(lod_bias * 32.0f)), max_aniso,
                             bias_clamp, GDTexMode0Ids[id]));
    GDWriteBPCmd(TX_SETMODE1((u8)(min_lod * 16.0f), (u8)(max_lod * 16.0f), GDTexMode1Ids[id]));
}

void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format) {
    GDWriteBPCmd(TX_SETIMAGE0(width - 1, height - 1, format, GDTexImage0Ids[id]));
}

void GDSetTexImgPtr(GXTexMapID id, void* image_ptr) {
    GDWriteBPCmd(TX_SETIMAGE3(OSCachedToPhysical(image_ptr) >> 5, GDTexImage3Ids[id]));
}
