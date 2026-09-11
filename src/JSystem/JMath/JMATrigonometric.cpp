#include "JSystem/JMath/JMATrigonometric.hpp"
#include <cmath>

namespace JMath {
    template < >
    f32 TAtanTable< 1024, f32 >::atan2_(f32 y, f32 x) const {
        if (y >= 0.0f) {
            if (x >= 0.0f) {
                if (x >= y) {
                    return get_(y, x);
                }
                return TAngleConstant_< f32 >::RADIAN_DEG090() - get_(x, y);
            }
            x = -x;
            if (x < y) {
                return TAngleConstant_< f32 >::RADIAN_DEG090() + get_(x, y);
            }
            return TAngleConstant_< f32 >::RADIAN_DEG180() - get_(y, x);
        }
        f32 negY = -y;
        if (x < 0.0f) {
            x = -x;
            if (x >= negY) {
                return -TAngleConstant_< f32 >::RADIAN_DEG180() + get_(negY, x);
            }
            return -TAngleConstant_< f32 >::RADIAN_DEG090() - get_(x, negY);
        }
        if (x < negY) {
            return -TAngleConstant_< f32 >::RADIAN_DEG090() + get_(x, negY);
        }
        return -get_(negY, x);
    }

    template < s32 Len, typename T >
    T TAtanTable< Len, T >::get_(T y, T x) const {
        if (x == 0.0f) {
            return 0.0f;
        }
        return mTable[static_cast< s32 >(0.5f + Len * y / x)];
    }

    template < int Bits, typename T >
    TSinCosTable< Bits, T >::TSinCosTable() {
        for (s32 i = 0; i < static_cast< s32 >(LEN); ++i) {
            table[i].a1 = sin(i * static_cast< f64 >(TAngleConstant_< T >::RADIAN_DEG360()) / LEN);
            table[i].b1 = cos(i * static_cast< f64 >(TAngleConstant_< T >::RADIAN_DEG360()) / LEN);
        }
    }

    template < s32 Len, typename T >
    TAtanTable< Len, T >::TAtanTable() {
        for (s32 i = 0; static_cast< u32 >(i) < Len; ++i) {
            mTable[i] = atan(static_cast< f64 >(i) / Len);
        }
        mTable[0] = 0.0f;
        _1000 = TAngleConstant_< T >::RADIAN_DEG090() / 2;
    }

    template < s32 Len, typename T >
    TAsinAcosTable< Len, T >::TAsinAcosTable() {
        for (s32 i = 0; i < Len; ++i) {
            mTable[i] = asin(static_cast< f64 >(i) / Len);
        }
        mTable[0] = 0.0f;
        _1000 = TAngleConstant_< T >::RADIAN_DEG090() / 2;
    }

    TSinCosTable< 14, f32 > sSinCosTable;
    TAtanTable< 1024, f32 > sAtanTable;
    TAsinAcosTable< 1024, f32 > sAsinAcosTable;
}
