#ifndef GXTEXTURE_H
#define GXTEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXLoadTexObj(const GXTexObj *, GXTexMapID);

void GXInitTlutObj(GXTlutObj *, void *, GXTlutFmt, u16);

u32 GXGetTexBufferSize(u16, u16, u32, GXBool, u8);

void GXInitTexObj(GXTexObj *, void *, u16, u16, GXTexFmt, GXTexWrapMode, GXTexWrapMode, GXBool);
void GXInitTexObjLOD(GXTexObj *, GXTexFilter, GXTexFilter, f32, f32, f32, GXBool, GXBool, GXAnisotropy);
void GXInitTexObjCI(GXTexObj *, void *, u16, u16, GXCITexFmt, GXTexWrapMode, GXTexWrapMode, GXBool, u32);
void GXInitTexObjTlut(GXTexObj *, u32);

void GXLoadTlut(const GXTlutObj *, u32);

void GXInvalidateTexAll(void);

#ifdef __cplusplus
}
#endif

#endif // GXTEXTURE_H