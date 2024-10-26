#include <JSystem/JGeometry/TMatrix.hpp>

template<>
void TVec3f::scale(f32 scalar, const TVec3f &rOther) {
    x = rOther.x * scalar;
    y = rOther.y * scalar;
    z = rOther.z * scalar;
}

template<>
void TRot3f::mult33(TVec3f &rDst) const {
    mult33Inline(rDst, rDst);
}
