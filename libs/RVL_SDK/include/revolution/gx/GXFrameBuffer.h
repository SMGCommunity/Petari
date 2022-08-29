#ifndef GXFRAMEBUFFER_H
#define GXFRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/gx.h>

void GXSetTexCopySrc(u16, u16, u16, u16);

void GXSetTexCopyDst(u16, u16, GXTexFmt, GXBool);

void GXCopyTex(void *, GXBool);

#ifdef __cplusplus
}
#endif

#endif // GXFRAMEBUFFER_H