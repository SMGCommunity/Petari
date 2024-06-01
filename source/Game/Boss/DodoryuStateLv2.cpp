#include <JSystem/JGeometry/TMatrix.hpp>

template<>
void TVec3f::scale(f32 scalar, const TVec3f &rOther) {
    x = rOther.x * scalar;
    y = rOther.y * scalar;
    z = rOther.z * scalar;
}

template<>
void TRot3f::mult33(TVec3f &rDst) const {
    f32 z = mMtx[2][1], y = mMtx[1][1], x = mMtx[0][1];
    z *= rDst.y;
    y *= rDst.y;
    x *= rDst.y;
    z += rDst.x * mMtx[2][0];
    y += rDst.x * mMtx[1][0];
    x += rDst.x * mMtx[0][0];
    x += rDst.z * mMtx[0][2];
    y += rDst.z * mMtx[1][2];
    z += rDst.z * mMtx[2][2];

    rDst.set(x, y, z);
    
}
