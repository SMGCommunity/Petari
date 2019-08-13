#include "MR/MathUtil.h"

namespace MR
{
    f32 getInterpolatevalue(f32 a1, f32 a2, f32 a3)
    {
        return (a2 + a1 *(a3 - a2));
    }

    f32 getLinerValue(f32 a1, f32 a2, f32 a3, f32 a4)
    {
        return (a2 + (a1 / a4) * (a3 - a2));
    }
};