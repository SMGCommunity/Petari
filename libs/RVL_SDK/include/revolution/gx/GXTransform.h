#ifndef GXTRANSFORM_H
#define GXTRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXSetProjection(const f32 mtx[4][4], GXProjectionType type);
void GXSetProjectionv(const f32 *ptr);
void GXLoadPosMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadPosMtxIndx(u16 mtx_indx, u32 id);
void GXLoadNrmMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadNrmMtxImm3x3(const f32 mtx[3][3], u32 id);
void GXLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type);
void GXLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type);

void GXSetViewport(f32, f32, f32, f32, f32, f32);

void GXSetZScaleOffset(f32, f32);

void GXSetScissor(u32, u32, u32, u32);
void GXGetScissor(u32 *, u32 *, u32 *, u32 *);
void GXSetScissorBoxOffset(s32, s32);

void GXSetClipMode(GXClipMode);

void __GXSetMatrixIndex(GXAttr);

static void GXSetViewportv(const f32* vp);

#ifdef __cplusplus
}
#endif

#endif // GXTRANSFORM_H
