#ifndef TUTIL_H
#define TUTIL_H

#include "types.h"

namespace JGeometry
{
    template<typename T>
    class TUtil
    {
        bool epsilonEquals(f32, f32, f32);
        f32 sqrt(f32);
    };
};

#endif // TUTIL_H