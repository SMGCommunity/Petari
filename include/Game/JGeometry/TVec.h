#pragma once

#include "types.h"

namespace JGeometry
{
    template<typename T>
    class TVec3
    {
    public:
        TVec3(T val)
        {
            x = T;
            y = T;
            z = T;
        }

        void set<T>(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        T x, y, z;
    };

    template<typename T>
    class TVec4
    {
    public:

        void set<T>(T _x, T _y, T _z, T _h)
        {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
        }

        T x, y, z, h;
    };
};