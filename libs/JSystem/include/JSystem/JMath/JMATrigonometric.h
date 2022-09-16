#pragma once

#include <revolution.h>

namespace std {
template <typename A1, typename B1>
struct pair {
    A1 a1;
    B1 b1;
    pair() {
        a1 = A1();
        b1 = B1();
    }
};
}  // namespace std

namespace JMath
{
    template<s32 Len, typename T>
    class TSinCosTable {
    public:
        std::pair<f32, f32> table[Len];

        f32 sinShort(s8 v) const { return table[static_cast<u8>(v) >> 3].a1; }
        f32 cosShort(s8 v) const { return table[static_cast<u8>(v) >> 3].b1; }
    };

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

    static TSinCosTable<14, f32> sSinCosTable;
    static TAtanTable<1024, f32> sAtanTable;
    static TAsinAcosTable<1024, f32> sAsinAcosTable;
};

inline f32 JMASSin(u16 s) {
    return JMath::sSinCosTable.sinShort(s);
}

inline f32 JMASCos(u16 s) {
    return JMath::sSinCosTable.cosShort(s);
}
