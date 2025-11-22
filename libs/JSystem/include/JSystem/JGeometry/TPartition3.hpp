#pragma once

#include "JSystem/JGeometry/TVec.hpp"

namespace JGeometry {
    template < typename T >
    class TPartition3 {
    public:
        TVec3< T > mNormal;  // 0x0
        T mDot;              // 0xC
    };
};  // namespace JGeometry
