#include "JMath/JMATrigonometric.h"
#include "math.h"

namespace JMath
{
    template<s32 Len, typename T>
    TAtanTable<Len, T>::TAtanTable()
    {
        s32 curIdx = 0;
        s32 curTblIdx = 0;

        do
        {
            mTable[curTblIdx] = atan(curIdx / 1024.0);
            curIdx++;
            curTblIdx++;
        } while (curIdx < 0x400);
        
        mTable[0] = 0.0f;
        _1000 = 0.78539819f;
    }

    template<s32 Len, typename T>
    TAsinAcosTable<Len, T>::TAsinAcosTable()
    {
        s32 curIdx = 0;
        s32 curTblIdx = 0;

        do
        {
            mTable[curTblIdx] = asin(curIdx / 1024.0);
            curIdx++;
            curTblIdx++;
        } while (curIdx < 0x400);
        
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

        return mTable[(0.5f + (1024.0f * arg1) / arg2)];
    }
};