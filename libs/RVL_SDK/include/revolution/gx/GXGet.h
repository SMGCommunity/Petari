#ifndef GXGET_H
#define GXGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx.h>

void GXGetViewportv(f32 *);

u32 GXGetTexObjTlut(const GXTexObj *);

void GXGetTexObjAll(const GXTexObj *, void **, u16 *, u16 *, GXTexFmt *, GXTexWrapMode *, GXTexWrapMode *, GXBool *);

void GXGetTexObjLODAll(const GXTexObj *, GXTexFilter *, GXTexFilter *, f32 *, f32 *, f32 *, GXBool *, GXBool *, GXAnisotropy *);

GXTexFmt GXGetTexObjFmt(const GXTexObj *);
GXBool GXGetTexObjMipMap(const GXTexObj *); 

#ifdef __cplusplus
}
#endif

#endif // GXGET_H
