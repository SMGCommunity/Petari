#ifndef GDBASE_H
#define GDBASE_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GDLObj {
  u8 *start;
  u32 length;
  u8 *ptr;
  u8 *top;
} GDLObj;

typedef void (*GDOverflowCallback)(void);

extern GDLObj *__GDCurrentDL;

void GDInitGDLObj(GDLObj *, void *, u32);

void GDPadCurr32();

void GDOverflowed();

static void GDOverflowCheck(u32 size) {
    if (__GDCurrentDL->ptr + size > __GDCurrentDL->top) {
        GDOverflowed();
    }
}

static void __GDWrite(u8 data) {
    *__GDCurrentDL->ptr++ = data;
}

static void GDWrite_u8(u8 data) {
    GDOverflowCheck(1);
    __GDWrite(data);
}

static void GDWrite_u16(u16 data) {
    GDOverflowCheck(2);
    __GDWrite((u8) (data >> 8));
    __GDWrite((u8) (data & 0xff));
}

static void GDWrite_u32(u32 data) {
    GDOverflowCheck(4);
    __GDWrite((u8) ((data >> 24) & 0xff));
    __GDWrite((u8) ((data >> 16) & 0xff));
    __GDWrite((u8) ((data >>  8) & 0xff));
    __GDWrite((u8) ((data >>  0) & 0xff));
}


static void GDWrite_f32(f32 data) {
    typedef union {
        f32 f;
        u32 u;
    } _FloatInt;

    _FloatInt fid;

    fid.f = data;

    GDWrite_u32(fid.u);
}

void GDOverflowed(void);

static void GDPosition3f32(f32 x, f32 y, f32 z) {
  GDWrite_f32(x);
  GDWrite_f32(y);
  GDWrite_f32(z);
}

void GDColor4u8(u8, u8, u8, u8);

#ifdef __cplusplus
}
#endif

#endif // GDBASE_H
