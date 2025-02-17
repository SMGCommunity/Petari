#ifndef GXCPU2EFB_H
#define GXCPU2EFB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

void GXPokeAlphaMode(GXCompare, u8);
void GXPokeAlphaRead(GXAlphaReadMode);
void GXPokeAlphaUpdate(GXBool);
void GXPokeBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
void GXPokeColorUpdate(GXBool);
void GXPokeDstAlpha(GXBool, u8);
void GXPokeDither(GXBool);
void GXPokeZMode(GXBool, GXCompare , GXBool);

#ifdef __cplusplus
}
#endif

#endif // GXCPU2EFB_H
