#include "JSystem/J3DGraphBase/J3DSys.h"

void J3DSys::loadPosMtxIndx(s32 unk1, u16 unk2) const
{
    s32 val = (unk1 * 0xC) & 0xFFFF;
    *((vu8*)GPU_FIFO_ADDR) = 0x20;
    *((vu16*)GPU_FIFO_ADDR) = unk2;
    *((vs16*)GPU_FIFO_ADDR) = val | 0xB000;
}

void J3DSys::loadNrmMtxIndx(s32 unk1, u16 unk2) const
{
    s32 val = ((unk1 * 0x9) + 0x400) & 0xFFFF;
    *((vu8*)GPU_FIFO_ADDR) = 0x28;
    *((vu16*)GPU_FIFO_ADDR) = unk2;
    *((vs16*)GPU_FIFO_ADDR) = val | 0xB000;
}

void J3DSys::reinitGX()
{
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetCoPlanar(GX_FALSE);
    reinitLighting();
    reinitTransform();
    reinitTexture();
    reinitTevStages();
    reinitIndStages();
    reinitPixelProc();
}

void J3DSys::reinitLighting()
{
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXColor color0;
    color0.r = sAmbientLightColor.r;
    color0.g = sAmbientLightColor.g;
    color0.b = sAmbientLightColor.b;
    color0.a = sAmbientLightColor.a;
    GXSetChanAmbColor(GX_COLOR0A0, color0);

    GXColor color1 = sAmbientLightColor;
    GXSetChanAmbColor(GX_COLOR1A1, color1);

    GXColor mat_color0;
    mat_color0.r = sLightMaterialColor.r;
    mat_color0.g = sLightMaterialColor.g;
    mat_color0.b = sLightMaterialColor.b;
    mat_color0.a = sLightMaterialColor.a;
    GXSetChanMatColor(GX_COLOR0A0, mat_color0);

    GXColor mat_color1;
    mat_color1.r = sLightMaterialColor.r;
    mat_color1.g = sLightMaterialColor.g;
    mat_color1.b = sLightMaterialColor.b;
    mat_color1.a = sLightMaterialColor.a;
    GXSetChanMatColor(GX_COLOR1A1, mat_color1);
}

void J3DSys::reinitTransform()
{
    GXSetCurrentMtx(0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, 0x3C, GX_FALSE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, 0x3C, GX_FALSE, 0x7D);
}

void J3DSys::reinitTexture()
{
    GXTexObj obj;

    GXInitTexObj(&obj, sImage, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXLoadTexObj(&obj, GX_TEXMAP1);
    GXLoadTexObj(&obj, GX_TEXMAP2);
    GXLoadTexObj(&obj, GX_TEXMAP3);
    GXLoadTexObj(&obj, GX_TEXMAP4);
    GXLoadTexObj(&obj, GX_TEXMAP5);
    GXLoadTexObj(&obj, GX_TEXMAP6);
    GXLoadTexObj(&obj, GX_TEXMAP7);
}