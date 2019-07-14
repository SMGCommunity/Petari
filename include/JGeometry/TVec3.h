#pragma once

namespace JGeometry
{
    template<typename T>
    class TVec3
    {
    public:
        void set(TVec3<T> const &);

        T x; // _0
        T y; // _4
        T z; // _8
    };

};