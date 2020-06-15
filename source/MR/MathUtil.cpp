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

    f32 getRandom(f32 min, f32 max)
    {
        return min + ((max - min) * getRandom());
    }

    f32 getRandomDegree()
    {
        // todo -- figure out why 0.0f get omitted
        return 0.0f + (360.0f * getRandom());
    }

    f32 min(f32 val_0, f32 val_1)
    {
        f32 ret = (val_0 >= val_1) ? val_1 : val_0;
        return ret;
    }

    f32 max(f32 val_0, f32 val_1)
    {
        f32 ret = (val_0 >= val_1) ? val_0 : val_1;
        return ret;
    }
};