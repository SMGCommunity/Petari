#ifndef GXGEOMETRY_H
#define GXGEOMETRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXSetVtxDesc(GXAttr, GXAttrType);
void GXClearVtxDesc(void);

void GXSetVtxAttrFmt(GXVtxFmt, GXAttr, GXCompCnt, GXCompType, u8);

void GXSetArray(GXAttr, const void *, u8);
void GXBegin(GXPrimitive, GXVtxFmt, u16);

void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType, GXTexGenSrc, u32, GXBool, u32);

void GXSetNumTexGens(u8);

#ifdef __cplusplus
}
#endif

#endif // GXGEOMETRY_H