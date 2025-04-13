#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

namespace JGeometry {
    template<typename T>
    class TBox2 {
    public:
        TBox2(T, T, T, T);
        //void set<T>(const TBox2<T> &a1, const TBox2<T> &a2);

        void set(const JGeometry::TBox2<T> &, const JGeometry::TBox2<T> &);
        void operator=(const JGeometry::TBox2<T> &);

        TVec2<T> _0;
        TVec2<T> _8;
    };

    template<typename T>
    class TBox3 {
    public:
        TBox3() {

        }

        bool intersectsPoint(const TVec3f &) const;

        JGeometry::TVec3<T> mMin;   // 0x0
        JGeometry::TVec3<T> mMax;   // 0xC
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

typedef JGeometry::TBox2<s16> TBox2s;
typedef JGeometry::TBox3<f32> TBox3f;
typedef JGeometry::TBox3<JGeometry::TVec3<f32> > TBoxVec3f;
typedef JGeometry::TDirBox3<f32> TDirBox3f;
