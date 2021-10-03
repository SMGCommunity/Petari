#pragma once

#include "JSystem/JGeometry/TMatrix.h"

namespace JGeometry {
    template<class T>
    class TPosition3 : public T {
    public:
    };
};

typedef JGeometry::TPosition3<TMtx34f> TPos3f;