#pragma once

#include "JSystem/JGeometry/TPartition3.h"

namespace JGeometry {
    template<typename T>
    class THexahedron3 {
    public:
        THexahedron3() {

        }

        bool mayIntersectBall3(const TVec3f &, f32) const;

        TPartition3<f32> _0[0x6];
        /*TPartition3<f32> _10;
        TPartition3<f32> _20;
        TPartition3<f32> _30;
        TPartition3<f32> _40;
        TPartition3<f32> _50;*/
    };
};