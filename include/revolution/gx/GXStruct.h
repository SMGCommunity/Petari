#ifndef GXSTRUCT_H
#define GXSTRUCT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXColor
{
    u8 r, g, b, a;

    void inline operator=(_GXColor color) 
    { 
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    };
} GXColor;

#ifdef __cplusplus
}
#endif

#endif // GXSTRUCT_H