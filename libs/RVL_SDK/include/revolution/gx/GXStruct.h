#ifndef GXSTRUCT_H
#define GXSTRUCT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a;
};

typedef struct _GXTexObj {
    u32 dummy[8];
} GXTexObj;

typedef struct _GXTlutObj {
    u32 dummy[3];
} GXTlutObj;

typedef struct _GXLightObj {
    u32 dummy[16];
} GXLightObj;

typedef struct _GXTexRegion {
    u32 dummy[4];
} GXTexRegion;

#ifdef __cplusplus
}
#endif

#endif // GXSTRUCT_H