#ifndef GXPIXEL_H
#define GXPIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

void GXSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);

void GXSetColorUpdate(GXBool);
void GXSetAlphaUpdate(GXBool);
void GXSetZMode(GXBool, GXCompare, GXBool);
void GXSetZCompLoc(GXBool);
void GXSetPixelFmt(GXPixelFmt, GXZFmt16);
void GXSetDither(GXBool);
void GXSetDstAlpha(GXBool, u8);

#ifdef __cplusplus
}
#endif

#endif // GXPIXEL_H