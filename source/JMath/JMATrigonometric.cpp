#include "JMath/JMATrigonometric.h"
#include "math.h"

namespace JMath
{
    template<s32 Len, typename T>
    TAtanTable<Len, T>::TAtanTable()
    {
        for (int i = 0; i < Len; ++i)
            mTable[i] = atan(i / static_cast<f64>(Len));
        
        mTable[0] = 0.0f;
        _1000 = 0.78539819f;
    }

    template<s32 Len, typename T>
    TAsinAcosTable<Len, T>::TAsinAcosTable()
    {
        for (int i = 0; i < Len; ++i)
            mTable[i] = asin(i / static_cast<f64>(Len));
        
        mTable[0] = 0.0f;
        _1000 = 0.78539819f;
    }

    template<s32 Len, typename T>
    T TAtanTable<Len, T>::get_(T arg1, T arg2) const
    {
        if (arg2 == 0.0f)
        {
            return 0.0f;
        }

        return mTable[(0.5f + (static_cast<f32>(Len) * arg1) / arg2)];
    }
};