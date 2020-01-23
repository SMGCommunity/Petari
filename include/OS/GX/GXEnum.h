#ifndef GXENUM_H
#define GXENUM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXTexMapID
{
    GX_TEXMAP0,
    GX_TEXMAP1,
    GX_TEXMAP2,
    GX_TEXMAP3,
    GX_TEXMAP4,
    GX_TEXMAP5,
    GX_TEXMAP6,
    GX_TEXMAP7,
    GX_MAX_TEXMAP,
    GX_TEXMAP_NULL = 0xFF,
    GX_TEX_DISABLE = 0x100
} GXTexMapID;

#ifdef __cplusplus
}
#endif

#endif // GXENUM_H