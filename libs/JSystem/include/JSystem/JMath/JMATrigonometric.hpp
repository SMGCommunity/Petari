#pragma once

#include <math_types.hpp>
#include <revolution.h>

namespace std {
    template <typename A1, typename B1>
    struct pair {
        A1 a1;
        B1 b1;
        pair()
        {
            a1 = A1();
            b1 = B1();
        }
    };
}    // namespace std

namespace JMath {
    template <s32 Bits, typename T>
    class TSinCosTable {
    public:
        static const u32 LEN = 1 << Bits;
        std::pair<f32, f32> table[LEN];

        f32 sinShort(s8 v) const { return table[static_cast<u8>(v) >> 3].a1; }
        f32 cosShort(s8 v) const { return table[static_cast<u8>(v) >> 3].b1; }

        inline f32 sinLap(f32 v)
        {
            if (v < 0.0f) {
                f32 tmp = v * -LEN / TWO_PI;
                return -table[(u16)tmp & LEN - 1].a1;
            }
            else {
                f32 tmp = v * LEN / TWO_PI;
                return table[(u16)tmp & LEN - 1].a1;
            }
        }

        inline f32 cosLap(f32 v) {
            if (v < 0.0f) {
                v = -v;
            }
            // 45.511112f == LEN / TWO_PI * PI / 180
            v = 45.511112f * v;
    
            return table[(u16)v & LEN - 1].b1;
        }
        inline f32 get(f32 v) { return table[(u16)v & LEN - 1].b1; }
    };

    template <s32 Len, typename T>
    class TAtanTable {
    public:
        TAtanTable();

        T atan2_(T, T) const;
        T get_(T, T) const;

        T mTable[Len];
        T _1000;
    };

    template <s32 Len, typename T>
    class TAsinAcosTable {
    public:
        TAsinAcosTable();

        T get_(T, T) const;

        T mTable[Len];
        T _1000;
    };

    static TSinCosTable<14, f32> sSinCosTable;
    static TAtanTable<1024, f32> sAtanTable;
    static TAsinAcosTable<1024, f32> sAsinAcosTable;
};    // namespace JMath

inline f32 JMASSin(u16 s)
{
    return JMath::sSinCosTable.sinShort(s);
}

inline f32 JMASCos(u16 s)
{
    return JMath::sSinCosTable.cosShort(s);
}
