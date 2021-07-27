#pragma once

#include <revolution.h>

namespace JMath
{
    template<s32 Len, typename T>
    class TAtanTable
    {
    public:
        TAtanTable();

        T atan2_(T, T) const;
        T get_(T, T) const;

        T mTable[Len];
        T _1000;
    };

    template<s32 Len, typename T>
    class TAsinAcosTable
    {
    public:
        TAsinAcosTable();

        T get_(T, T) const;

        T mTable[Len];
        T _1000;
    };

    static TAtanTable<1024, f32> sAtanTable;
    static TAsinAcosTable<1024, f32> sAsinAcosTable;
};