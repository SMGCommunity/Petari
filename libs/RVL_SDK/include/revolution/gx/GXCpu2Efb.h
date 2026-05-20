#ifndef GXCPU2EFB_H
#define GXCPU2EFB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/gx/GXEnum.h>
#include <revolution/types.h>

void GXPokeAlphaMode(GXCompare, u8);
void GXPokeAlphaRead(GXAlphaReadMode);
void GXPokeAlphaUpdate(GXBool);
void GXPokeBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
void GXPokeColorUpdate(GXBool);
void GXPokeDstAlpha(GXBool, u8);
void GXPokeDither(GXBool);
void GXPokeZMode(GXBool, GXCompare, GXBool);
void GXPeekARGB(u16 x, u16 y, u32* color);
void GXPeekZ(u16 x, u16 y, u32* z);

#ifdef __cplusplus
}
#endif

#endif  // GXCPU2EFB_H
