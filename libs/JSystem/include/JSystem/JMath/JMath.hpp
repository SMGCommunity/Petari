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
        register f32 v1xy, v2xy, d1xy, d1z, v1z, v2z;
        
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
        /*register f32 v1xy, v2xy, dxy, v1z, v2z, dz;
        __asm {
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            psq_l     v1z,   8(vec1), 1, 0
            ps_sub    dxy, v1xy, v2xy
            psq_l     v2z,   8(vec2), 1, 0
            ps_sub    dz, v1z, v2z
            psq_st    dxy, 0(dst), 0, 0
            psq_st    dz,  8(dst), 1, 0            
        }*/
        
        register f32 v2xy, v1z, v2z, v1xy, dxy, dz;
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
    inline void PSVECMultiply(register const Vec* vec1, register const Vec* vec2, register Vec* dst) {
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

    __attribute__((always_inline))
    inline f32 PSVECSquareDistance(register const Vec* this_vec, register const Vec* rVec1) { 
        //register const TVec3<T> *this_vec = this;
        register f32 yz_1, x_1;
        register f32 yz_2, x_2;
        register f32 ret;
        __asm {
            psq_l yz_1, 4(this_vec), 0, 0
            psq_l yz_2, 4(rVec1), 0, 0
            psq_l x_1, 0(this_vec), 0, 0
            ps_sub yz_2, yz_1, yz_2
            psq_l x_2, 0(rVec1), 0, 0
            ps_sub x_2, x_1, x_2
            ps_mul yz_2, yz_2, yz_2
            ps_madd ret, x_2, x_2, yz_2
            ps_sum0 ret, ret, yz_2, yz_2
        };
        return ret;
    }
    
    #else
    void PSVECAdd(const Vec *, const Vec *, Vec *);
    void PSVECSubtract(const Vec *, const Vec *, Vec *);
    f32 PSVECDotProduct(const Vec *, const Vec *);
    f32 PSVECSquareMag(const Vec *);
#endif
};
