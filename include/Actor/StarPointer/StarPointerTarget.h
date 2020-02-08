#ifndef STARPOINTERTARGET_H
#define STARPOINTERTARGET_H

#include <revolution.h>
#include "JGeometry/TVec2.h"
#include "JGeometry/TVec3.h"

class StarPointerTarget
{
public:
    StarPointerTarget(f32, const JGeometry::TVec3<f32> *, Mtx *, JGeometry::TVec3<f32>);

    u8 _0[0x1C];
};

#endif // STARPOINTERTARGET_H