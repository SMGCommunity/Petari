#ifndef GDBASE_H
#define GDBASE_H

#include "revolution.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GDLObj {
    u8  *start;
    u32  length;
    u8  *ptr;
    u8  *top;
} GDLObj;

extern GDLObj *__GDCurrentDL;

void GDInitGDLObj(GDLObj *, void *, u32);

void GDPadCurr32();

void GDWrite_f32(f32);

static inline void GDPosition3f32(f32 x, f32 y, f32 z) {
    GDWrite_f32(x);
    GDWrite_f32(y);
    GDWrite_f32(z);
}

void GDColor4u8(u8, u8, u8, u8);

#ifdef __cplusplus
}
#endif

#endif // GDBASE_H