#ifndef BINDER_H
#define BINDER_H

#include "JGeometry/TVec3.h"
#include "types.h"

class Binder
{
public:
    Binder(f32 *[4], const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> *, f32, f32, u64);

    void clear();

    u8 _0[0x1F0];
};

#endif // BINDER_H