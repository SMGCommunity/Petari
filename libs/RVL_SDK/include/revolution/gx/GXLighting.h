#ifndef GXLIGHTING_H
#define GXLIGHTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXInitLightAttn(GXLightObj *, f32, f32, f32, f32, f32, f32);
void GXInitLightPos(GXLightObj *, f32, f32, f32);
void GXInitLightColor(GXLightObj *, GXColor);
void GXLoadLightObjImm(const GXLightObj *, GXLightID);

void GXSetNumChans(u8);

void GXSetChanAmbColor(GXChannelID, GXColor);
void GXSetChanMatColor(GXChannelID, GXColor);

void GXSetChanCtrl(GXChannelID, GXBool, GXColorSrc, GXColorSrc, u32, GXDiffuseFn, GXAttnFn);

#ifdef __cplusplus
}
#endif

#endif // GXLIGHTING_H