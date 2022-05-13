#pragma once

#include "JSystem/JGeometry/TVec.h"
#include "revolution.h"

namespace JGeometry {
    template<typename T>
    class TBox2 {
    public:
        //void set<T>(const TBox2<T> &a1, const TBox2<T> &a2);

        void set(const JGeometry::TBox2<T> &, const JGeometry::TBox2<T> &);
        void operator=(const JGeometry::TBox2<T> &);

        T _0, _4, _8, _C;
    };

    template<typename T>
    class TBox3 {
    public:
        TBox3() {

        }

        JGeometry::TVec3<T> mMin;   // _0
        JGeometry::TVec3<T> mMax;   // _C
    };

    template<typename T>
    class TDirBox3 {
    public:
        JGeometry::TVec3<T> _0;
        JGeometry::TVec3<T> _C;
        JGeometry::TVec3<T> _18;
        JGeometry::TVec3<T> _24;
        JGeometry::TVec3<T> _30;
    };
};

typedef JGeometry::TBox3<f32> TBox3f;
typedef JGeometry::TDirBox3<f32> TDirBox3f;