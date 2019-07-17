#ifndef HITINFO_H
#define HITINFO_H

#include "Collision/Triangle.h"

class HitInfo : public Triangle
{
public:
    HitInfo();

    f32 _60;
    JGeometry::TVec3<f32> _64;
    JGeometry::TVec3<f32> _70;
    JGeometry::TVec3<f32> _7C;
    u8 _88;
};

#endif // HITINFO_H