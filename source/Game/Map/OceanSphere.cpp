#include <JSystem/JGeometry/TMatrix.hpp>
#include <cmath>

inline f32 yy(f32 y) {
    return y * y;
}

template<>
void TRot3f::setRotate(const TVec3f &mLocalDirection, f32 fr1e) {
    TVec3f v;
    v.set(mLocalDirection);
    PSVECMag(v.toCVec());
    PSVECNormalize(v.toCVec(), v.toVec());
    f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
    f32 x, y, z;
    y = v.y;
    x = v.x;
    z = v.z;
    mMtx[0][0] = fr1ex + (1.0f - fr1ex) * yy(x);
    mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
    mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
    mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
    mMtx[1][1] = fr1ex + (1.0f - fr1ex) * yy(y);
    mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
    mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
    mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
    mMtx[2][2] = fr1ex + (1.0f - fr1ex) * yy(z);
}
