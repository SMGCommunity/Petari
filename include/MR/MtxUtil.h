#ifndef MTXUTIL_H
#define MTXUTIL_H

#include <revolution/mtx.h>
#include "JGeometry/TVec3.h"

namespace MR
{
    void makeMtxRotate(Mtx *, const JGeometry::TVec3<f32> &);
};

#endif // MTXUTIL_H