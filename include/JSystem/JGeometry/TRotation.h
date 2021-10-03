#pragma once

#include "JSystem/JGeometry/TMatrix.h"

namespace JGeometry {
    template<class T>
    class TRotation3 : public T {
    public:
        void mult33(const TVec3f &, const TVec3f &) const;
    };
};

typedef JGeometry::TRotation3<TMtx34f> TRot3f;