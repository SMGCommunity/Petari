#pragma once

#include "JSystem/JGeometry/TPartition3.hpp"

namespace JGeometry {
    template < typename T >
    class THexahedron3 {
    public:
        THexahedron3() {}

        bool mayIntersectBall3(const TVec3f& rVec, f32 a2) const; /*{
            f32 neg = -a2;
            s32 curIdx = 0;
            while(mPlanes[curIdx] != mPlanes[0x6]) {
                if (mPlanes[curIdx].mNormal.dot(rVec) - mPlanes[curIdx].mDot) >= neg)) {
                    return true;
                }
                curIdx++;
            }

            return false;
        }*/

        TPartition3< f32 > mPlanes[0x6];  // 0x0
    };
};  // namespace JGeometry

typedef JGeometry::THexahedron3< f32 > THex3f;