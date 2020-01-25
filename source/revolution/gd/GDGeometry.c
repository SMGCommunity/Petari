#include "revolution/GD/GDGeometry.h"
#include "revolution/GD/GDBase.h"

void GDBegin(GXPrimitive prim, GXVtxFmt fmt, u16 numVerts)
{
    GDOverflowCheck(1);
    *__GDCurrentDL->mPtr++ = fmt | prim;
    GDOverflowCheck(2);
    *__GDCurrentDL->mPtr++ = numVerts >> 8;
    *__GDCurrentDL->mPtr++ = numVerts;
}