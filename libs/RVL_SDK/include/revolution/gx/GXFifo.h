#ifndef GXFIFO_H
#define GXFIFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

typedef struct _GXFifoObj {
    u8* base;
    u8* top;
    u32 size;
    u32 hiWatermark;
    u32 loWatermark;
    void* rdPtr;
    void* wrPtr;
    s32 count;
    GXBool wrap;
    GXBool bind_cpu;
    GXBool bind_gp;
} GXFifoObj;

GXBool GXGetCPUFifo(GXFifoObj *);
GXBool GXGetGPFifo(GXFifoObj *);

void GXGetFifoPtrs(const GXFifoObj *, void **, void **);

void GXDisableBreakPt(void);

#ifdef __cplusplus
}
#endif

#endif // GXFIFO_H