#include "JSystem/JGeometry/TVec.hpp"

void dummy(const TQuat4f& rRotation, TVec3f& rVector) {
    void (TQuat4f::*rotate)(TVec3f&) const = &TQuat4f::rotate;
    (rRotation.*rotate)(rVector);
}
