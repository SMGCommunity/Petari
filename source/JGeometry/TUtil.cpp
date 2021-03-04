#include "JGeometry/TUtil.h"

namespace JGeometry
{
    template<typename T>
    bool TUtil<T>::epsilonEquals(T arg1, T arg2, T arg3)
    {
        f32 diff = arg1 - arg2;
        bool res = false;

        if (-arg3 <= diff && diff <= arg3)
        {
            res = true; 
        }

        return res;
    }

    template<>
    f32 TUtil<f32>::sqrt(f32 val)
    {
        if (val <= 0.0f)
        {
            return val;
        }

        f32 sqrt = __frsqrte(val);
        val = (val * ((0.5f * sqrt) * (3.0f - (val * (sqrt * sqrt)))));
        return val;
    }

    template<typename T>
    T TUtil<T>::clamp(T arg1, T arg2, T arg3)
    {
        if (arg1 < arg2)
            return arg2;
        
        if (arg1 > arg3)
            return arg3;

        return arg3;
    }
};