#pragma once

#include "types.h"

namespace JGeometry {
    template<typename T>
    class TBox2 {
    public:
        void set<T>(const TBox2<T> &a1, const TBox2<T> &a2);

        void set(const JGeometry::TBox2<T> &, const JGeometry::TBox2<T> &);
        void operator=(const JGeometry::TBox2<T> &);

        T _0, _4, _8, _C;
    };
};