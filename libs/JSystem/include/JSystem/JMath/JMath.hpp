#pragma once

#include <revolution.h>

f32 JMAAsinRadian(f32);
f32 JMACosRadian(f32);

f32 JMAAcosRadian(f32);

f32 JMASinDegree(f32);
f32 JMACosDegree(f32);

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);


namespace JMath {
    void gekko_ps_copy12(void *, const void *);
};

namespace JMathInlineVEC {
    #ifdef __MWERKS__
    __attribute__((always_inline))
    inline void PSVECAdd(register const Vec* vec1, register const Vec* vec2, register Vec* dst)
    {
        register f32 v1z, v2z, d1z, d1xy, v2xy, v1xy;
        __asm {            
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            ps_add    d1xy, v1xy, v2xy
            psq_st    d1xy, 0(dst), 0, 0
            
            psq_l     v1z,   8(vec1), 1, 0
            psq_l     v2z,   8(vec2), 1, 0
            ps_add    d1z, v1z, v2z
            psq_st    d1z,  8(dst), 1, 0
        }
    }

    __attribute__((always_inline))
    inline void PSVECSubtract(register const Vec* vec1, register const Vec* vec2, register Vec* dst) {
        register f32 v1xy, v2xy, dxy, v1z, v2z, dz;
        __asm {
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            psq_l     v1z,   8(vec1), 1, 0
            ps_sub    dxy, v1xy, v2xy
            psq_l     v2z,   8(vec2), 1, 0
            ps_sub    dz, v1z, v2z
            psq_st    dxy, 0(dst), 0, 0
            psq_st    dz,  8(dst), 1, 0            
        }
    }

    __attribute__((always_inline))
    inline f32 PSVECSquareMag(register const Vec* src)
    {
        register f32 z, xy, ret;
        __asm {
            psq_l xy, 0(src), 0, 0
            ps_mul xy, xy, xy
            lfs z, 8(src)            
            ps_madd ret, z, z, xy
            ps_sum0 ret, ret, xy, xy            
        }
        return ret;
    }
    #else
    void PSVECAdd(const Vec *, const Vec *, Vec *);
    void PSVECSubtract(const Vec *, const Vec *, Vec *);
    f32 PSVECDotProduct(const Vec *, const Vec *);
    f32 PSVECSquareMag(const Vec *);
#endif
};
