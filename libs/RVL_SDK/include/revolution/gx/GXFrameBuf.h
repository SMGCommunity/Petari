#ifndef GXFRAMEBUFFER_H
#define GXFRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/gx.h>

void GXSetDispCopySrc(u16, u16, u16, u16);
void GXSetTexCopySrc(u16, u16, u16, u16);
void GXSetDispCopyDst(u16, u16);
void GXSetTexCopyDst(u16, u16, GXTexFmt, GXBool);
void GXSetDispCopyFrame2Field(GXCopyMode);
void GXSetCopyClamp(GXFBClamp);
u16 GXGetNumXfbLines(u16, f32);
f32 GXGetYScaleFactor(u16, u16);
u32 GXSetDispCopyYScale(f32);
void GXSetCopyClear(GXColor, u32);
void GXSetCopyFilter(GXBool, const u8[12][2], GXBool, const u8[7]);
void GXSetDispCopyGamma(GXGamma);
void GXCopyDisp(void*, GXBool);
void GXCopyTex(void*, GXBool);
void GXClearBoundingBox(void);

extern GXRenderModeObj GXNtsc240Ds;
extern GXRenderModeObj GXNtsc240DsAa;
extern GXRenderModeObj GXNtsc240Int;
extern GXRenderModeObj GXNtsc240IntAa;
extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXNtsc480Int;
extern GXRenderModeObj GXNtsc480IntAa;
extern GXRenderModeObj GXNtsc480Prog;
extern GXRenderModeObj GXNtsc480ProgSoft;
extern GXRenderModeObj GXNtsc480ProgAa;
extern GXRenderModeObj GXMpal240Ds;
extern GXRenderModeObj GXMpal240DsAa;
extern GXRenderModeObj GXMpal240Int;
extern GXRenderModeObj GXMpal240IntAa;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXMpal480Int;
extern GXRenderModeObj GXMpal480IntAa;
extern GXRenderModeObj GXMpal480Prog;
extern GXRenderModeObj GXMpal480ProgSoft;
extern GXRenderModeObj GXMpal480ProgAa;
extern GXRenderModeObj GXPal264Ds;
extern GXRenderModeObj GXPal264DsAa;
extern GXRenderModeObj GXPal264Int;
extern GXRenderModeObj GXPal264IntAa;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXPal528Int;
extern GXRenderModeObj GXPal524IntAa;
extern GXRenderModeObj GXEurgb60Hz240Ds;
extern GXRenderModeObj GXEurgb60Hz240DsAa;
extern GXRenderModeObj GXEurgb60Hz240Int;
extern GXRenderModeObj GXEurgb60Hz240IntAa;
extern GXRenderModeObj GXEurgb60Hz480IntDf;
extern GXRenderModeObj GXEurgb60Hz480Int;
extern GXRenderModeObj GXEurgb60Hz480IntAa;
extern GXRenderModeObj GXEurgb60Hz480Prog;
extern GXRenderModeObj GXEurgb60Hz480ProgSoft;
extern GXRenderModeObj GXEurgb60Hz480ProgAa;

#ifdef __cplusplus
}
#endif

#endif // GXFRAMEBUFFER_H
