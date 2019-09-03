#ifndef TUTIL_H
#define TUTIL_H

#include "types.h"

namespace JGeometry
{
    template<typename T>
    class TUtil
    {
    public:
        bool epsilonEquals(f32, f32, f32);
        f32 sqrt(f32);
        static f32 clamp(T, T, T);
    };
};

#endif // TUTIL_H