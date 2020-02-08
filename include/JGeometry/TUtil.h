#ifndef TUTIL_H
#define TUTIL_H

#include <revolution.h>

namespace JGeometry
{
    template<typename T>
    class TUtil
    {
    public:
        static bool epsilonEquals(T, T, T);
        static f32 sqrt(T);
        static f32 clamp(T, T, T);
    };
};

#endif // TUTIL_H