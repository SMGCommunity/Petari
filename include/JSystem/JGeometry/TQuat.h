#pragma once

#include "JGeometry/TVec.h"

namespace JGeometry {
    template<typename T>
    class TQuat4 {
    public:
        TQuat4();

        void getZDir(TVec3f &) const;
        void setEuler(T, T, T);
        void setRotate(const JGeometry::TVec3<T> &, const JGeometry::TVec3<T> &);
        void slerp(const JGeometry::TQuat4<T> &, T);
        void normalize(const JGeometry::TQuat4<T> &, T);
        void getEuler(const JGeometry::TVec3<T> &) const;
    };
};

typedef JGeometry::TQuat4<f32> TQuat4f;