#pragma once

namespace JGeometry
{
    template<typename T>
    class TVec3
    {
    public:
        TVec3();
        TVec3(const TVec3<T> &);
        TVec3(T, T, T);

        void set(TVec3<T> const &);
        void zero();

        TVec3<T> operator +=(const TVec3<T> &);
        TVec3<T> operator -(const TVec3<T> &);

        T x; // _0
        T y; // _4
        T z; // _8
    };

};