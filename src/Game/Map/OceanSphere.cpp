#include <JSystem/JGeometry/TMatrix.hpp>
#include <cmath>

inline f32 yy(f32 y) {
    return y * y;
}

template<>
void TRot3f::setRotate(const TVec3f &axis, f32 angle) {
    TVec3f v;
    v.set<f32>(axis);
    PSVECMag(&v);
    PSVECNormalize(&v, &v);
    f32 angley = sin(angle), anglex = cos(angle);
    f32 x, y, z;
    y = v.y;
    x = v.x;
    z = v.z;
    mMtx[0][0] = anglex + (1.0f - anglex) * yy(x);
    mMtx[0][1] = (1.0f - anglex) * x * y - angley * z;
    mMtx[0][2] = (1.0f - anglex) * x * z + angley * y;
    mMtx[1][0] = (1.0f - anglex) * x * y + angley * z;
    mMtx[1][1] = anglex + (1.0f - anglex) * yy(y);
    mMtx[1][2] = (1.0f - anglex) * y * z - angley * x;
    mMtx[2][0] = (1.0f - anglex) * x * z - angley * y;
    mMtx[2][1] = (1.0f - anglex) * y * z + angley * x;
    mMtx[2][2] = anglex + (1.0f - anglex) * yy(z);
}
