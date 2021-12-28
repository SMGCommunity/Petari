#pragma once

#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TRotation.h"
#include "Inline.h"

namespace JGeometry {
    template<class T>
    struct TPosition3 : public TRotation3<T> {
    public:
        void getTrans(TVec3f &);
    };
};

typedef JGeometry::TPosition3<TMtx34f> TPos3f;