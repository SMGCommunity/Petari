#ifndef GXTEXTURE_H
#define GXTEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXLoadTexObj(const GXTexObj *, GXTexMapID);

u32 GXGetTexBufferSize(u16, u16, u32, GXBool, u8);

#ifdef __cplusplus
}
#endif

#endif // GXTEXTURE_H