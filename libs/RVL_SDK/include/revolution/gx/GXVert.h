#ifndef GXVERT_H
#define GXVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/base/PPCWGPipe.h>

#ifdef __MWERKS__
volatile PPCWGPipe GXWGFifo : 0xCC008000;
#else
volatile PPCWGPipe GXWGFifo;
#endif

#define __GXCDEF(prfx, n, t) __GXCDEF##n(prfx##n##t, t, t)
#define __GXCDEFX(func, n, t) __GXCDEF##n(func, t, t)

#define __GXCDEF1(func, ts, td)                                                                                                                      \
    static void func(const ts x) {                                                                                                                   \
        GXWGFifo.td = (td)x;                                                                                                                         \
        return;                                                                                                                                      \
    }

#define __GXCDEF2(func, ts, td)                                                                                                                      \
    static void func(const ts x, const ts y) {                                                                                                       \
        GXWGFifo.td = (td)x;                                                                                                                         \
        GXWGFifo.td = (td)y;                                                                                                                         \
        return;                                                                                                                                      \
    }

#define __GXCDEF3(func, ts, td)                                                                                                                      \
    static void func(const ts x, const ts y, const ts z) {                                                                                           \
        GXWGFifo.td = (td)x;                                                                                                                         \
        GXWGFifo.td = (td)y;                                                                                                                         \
        GXWGFifo.td = (td)z;                                                                                                                         \
        return;                                                                                                                                      \
    }

#define __GXCDEF4(func, ts, td)                                                                                                                      \
    static void func(const ts x, const ts y, const ts z, const ts w) {                                                                               \
        GXWGFifo.td = (td)x;                                                                                                                         \
        GXWGFifo.td = (td)y;                                                                                                                         \
        GXWGFifo.td = (td)z;                                                                                                                         \
        GXWGFifo.td = (td)w;                                                                                                                         \
        return;                                                                                                                                      \
    }

__GXCDEF(GXCmd, 1, u8)
__GXCDEF(GXCmd, 1, u16)
__GXCDEF(GXCmd, 1, u32)

__GXCDEF(GXPosition, 2, f32)

__GXCDEF(GXPosition, 3, f32)
__GXCDEF(GXPosition, 3, s16)

__GXCDEF(GXNormal, 3, f32)

__GXCDEF(GXColor, 1, u32)
__GXCDEF(GXColor, 4, u8)

__GXCDEF(GXTexCoord, 2, u16)
__GXCDEF(GXTexCoord, 2, s16)
__GXCDEF(GXTexCoord, 2, f32)

__GXCDEFX(GXPosition1x8, 1, u8)
__GXCDEFX(GXNormal1x8, 1, u8)
__GXCDEFX(GXTexCoord1x8, 1, u8)

#ifdef __cplusplus
}
#endif

#endif  // GXVERT_H
