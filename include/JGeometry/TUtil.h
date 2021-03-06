#pragma once

#include <revolution.h>

namespace JGeometry
{
    template<typename T>
    class TUtil
    {
    public:
        static T inv_sqrt(T);
        static bool epsilonEquals(T, T, T);
        static f32 sqrt(T);
        static T clamp(T, T, T);
    };
};