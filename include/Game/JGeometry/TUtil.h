#pragma once

namespace JGeometry
{
    template<typename T>
    class TUtil
    {
    public:
        static int epsilonEquals(T a1, T a2, T a3)
        {
            int ret = 0;

            a1 -= a2;

            if (-a3 <= a1 && a1 <= a3)
            {
                ret = 1;
            }

            return ret;
        }

        static T sqrt(T val)
        {
            if (val <= 0.0f)
            {
                return;
            }

            float outVal = __frsqrte(val);
            return (val * (0.5f * outVal * (3.0f - (val * (outVal * outVal)))));
        }
    };
};