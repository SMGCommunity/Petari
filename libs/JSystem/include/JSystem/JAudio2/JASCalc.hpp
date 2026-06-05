#pragma once

#include <limits>
#include <revolution.h>

namespace JASCalc {
    void imixcopy(const s16*, const s16*, s16*, u32);
    void bcopyfast(const void*, void*, u32);
    void bcopy(const void*, void*, u32);
    void bzerofast(void*, u32);
    void bzero(void*, u32);
    f32 pow2(f32);

    inline f32 clamp01(f32 x) {
        if (x <= 0.0f) {
            return 0.0f;
        }
        if (x >= 1.0f) {
            return 1.0f;
        }
        return x;
    };

    template < typename T, typename S >
    inline T clamp(S x) {
        if (x <= std::numeric_limits< T >::min())
            return std::numeric_limits< T >::min();
        if (x >= std::numeric_limits< T >::max())
            return std::numeric_limits< T >::max();
        return x;
    }

};  // namespace JASCalc
