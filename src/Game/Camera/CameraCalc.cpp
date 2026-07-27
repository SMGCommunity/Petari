#include "Game/Util/MathUtil.hpp"
#include "Game/Util/VectorUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

void DUMMY() {
    f32 q = MR::acos(1.0f);
    TVec3f a, b, c;

    f32 x, y, z;
    MR::crossToPolar(a, b, &x, &y, &z);
    MR::crossToPolar(a, b, &x, &y, &z);

    MR::polarToCross(a, &b, x, y, z);
    MR::polarToCross(a, &b, x, y, z);

    MR::polarToCrossDegree(a, &b, x, y, z);
    MR::polarToCrossDegree(a, &b, x, y, z);

    f32 f1 = MR::diffRadian(x, y);
    f32 f2 = MR::diffRadian(y, x);

    MR::diffRadianAroundAxis(&x, a, b, c);
    MR::diffRadianAroundAxis(&x, a, b, c);

    MR::projectVecOnPlane(&a, b, c);
    MR::projectVecOnPlane(&a, b, c);
}

void DUMMY2() {
    TVec3f a, b, c;
    b *= 2.0f;
}
