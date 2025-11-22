#pragma once

#include <JSystem/JGeometry.hpp>

namespace MR {
    inline void getUpVecFromQuat(TVec3f* pOut, const TQuat4f& rQuat) {
        pOut->set< f32 >((2.0f * (rQuat.x * rQuat.y)) - (2.0f * (rQuat.w * rQuat.z)),
                         ((1.0f - (2.0f * (rQuat.x * rQuat.x))) - (2.0f * (rQuat.z * rQuat.z))),
                         ((2.0f * (rQuat.y * rQuat.z)) + (2.0f * (rQuat.w * rQuat.x))));
    }
};  // namespace MR
