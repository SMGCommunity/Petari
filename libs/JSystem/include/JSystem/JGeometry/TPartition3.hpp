#pragma once

#include "JSystem/JGeometry/TVec.hpp"

namespace JGeometry {
    template < typename T >
    class TPartition3 {
    public:
        void set(const TVec3< T >& a1, const TVec3< T >& a2, const TVec3< T >& a3);

        TVec3< T > mNormal;  // 0x0
        T mDot;              // 0xC
    };
};  // namespace JGeometry
