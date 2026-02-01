#pragma once

#include <math_types.hpp>
#include <revolution.h>

namespace std {
    template < typename A1, typename B1 >
    struct pair {
        A1 a1;
        B1 b1;
        pair() {
            a1 = A1();
            b1 = B1();
        }
    };
}  // namespace std

namespace JMath {

    template < typename T >
    struct TAngleConstant_;

    template <>
    struct TAngleConstant_< f32 > {
        static f32 RADIAN_DEG090() { return 1.5707964f; }
        static f32 RADIAN_DEG180() { return 3.1415927f; }
        static f32 RADIAN_DEG360() { return 6.2831855f; }
        static f32 RADIAN_TO_DEGREE_FACTOR() { return 180.0f / RADIAN_DEG180(); }
    };

    template < int Bits, typename T >
    class TSinCosTable {
    public:
        static const u32 LEN = 1 << Bits;
        std::pair< T, T > table[LEN];

        T sinShort(s16 v) const { return table[static_cast< u16 >(v) >> (16U - Bits)].a1; }
        T cosShort(s16 v) const { return table[static_cast< u16 >(v) >> (16U - Bits)].b1; }

        inline f32 sinLapRad(f32 v) {
            if (v < 0.0f) {
                f32 tmp = v * (-LEN / TWO_PI);
                return -table[(u16)tmp & LEN - 1].a1;
            } else {
                f32 tmp = v * (LEN / TWO_PI);
                return table[(u16)tmp & LEN - 1].a1;
            }
        }

        inline f32 cosLapRad(f32 v) {
            if (v < 0.0f) {
                v = -v;
            }

            f32 tmp = v;
            tmp *= (LEN / TWO_PI);
            return table[(u16)tmp & LEN - 1].b1;
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

    template < s32 Len, typename T >
    class TAtanTable {
    public:
        TAtanTable();

        T atan2_(T, T) const;
        T get_(T, T) const;

        T mTable[Len];
        T _1000;
    };

    template < s32 Len, typename T >
    class TAsinAcosTable {
    public:
        TAsinAcosTable();

        T get_(T, T) const;

        T acos_(T x) const {
            if (x >= 1.0f) {
                return 0.0f;
            } else if (x <= -1.0f) {
                return TAngleConstant_< T >::RADIAN_DEG180();
            } else if (x < 0.0f) {
                return mTable[(u32)(-x * 1023.5f)] + TAngleConstant_< T >::RADIAN_DEG090();
            } else {
                return TAngleConstant_< T >::RADIAN_DEG090() - mTable[(u32)(x * 1023.5f)];
            }
        }

        T acosDegree(T x) const { return acos_(x) * TAngleConstant_< T >::RADIAN_TO_DEGREE_FACTOR(); }

        T mTable[Len];
        T _1000;
    };

    extern TSinCosTable< 14, f32 > sSinCosTable;
    extern TAtanTable< 1024, f32 > sAtanTable;
    extern TAsinAcosTable< 1024, f32 > sAsinAcosTable;

    inline f32 acosDegree(f32 x) { return sAsinAcosTable.acosDegree(x); }
};  // namespace JMath

// inline f32 JMASSin(u16 s) {
//    return JMath::sSinCosTable.sinShort(s);
//}

inline f32 JMACosShort(s16 v) {
    return JMath::sSinCosTable.cosShort(v);
}

inline f32 JMASinShort(s16 v) {
    return JMath::sSinCosTable.sinShort(v);
}

inline f32 JMASCos(s16 v) {
    return JMACosShort(v);
}

inline f32 JMASSin(s16 v) {
    return JMASinShort(v);
}
