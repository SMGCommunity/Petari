#pragma once

#include <revolution.h>
#include <revolution/gx.h>

const s32 GPU_FIFO_ADDR = 0xCC008000;

class J3DSys
{
public:
    void loadPosMtxIndx(s32, u16) const;
    void loadNrmMtxIndx(s32, u16) const;

    void drawInit();
    void reinitGX();
    void reinitLighting();
    void reinitTransform();
    void reinitTexture();
    void reinitTevStages();
    void reinitIndStages();
    void reinitPixelProc();

    static GXColor sAmbientLightColor;
    static GXColor sLightMaterialColor;

    static void* sImage;

    static J3DSys j3dSys;
};