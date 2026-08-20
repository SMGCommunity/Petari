#pragma once

#include "JSystem/JGeometry/TPartition3.hpp"

namespace JGeometry {
    template < typename T >
    class THexahedron3 {
    public:
        THexahedron3() {
        }

        bool mayIntersectBall3(const TVec3f& rVec, f32 a2) const {
            f32 neg = -a2;
            const TPartition3< f32 >* end = mPlanes + 6;
            const TPartition3< f32 >* cur = mPlanes;

            while (cur < end) {
                if (cur->mNormal.dot(rVec) - cur->mDot < neg) {
                    return false;
                }

                cur++;
            }

            return true;
        }

        TPartition3< f32 > mPlanes[0x6];  // 0x0
    };
};  // namespace JGeometry

typedef JGeometry::THexahedron3< f32 > THex3f;
