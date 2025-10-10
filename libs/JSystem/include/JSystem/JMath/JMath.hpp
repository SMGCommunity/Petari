#pragma once

#include <revolution.h>

f32 JMAAcosRadian(f32);
f32 JMAAsinRadian(f32);
f32 JMACosDegree(f32);
f32 JMACosRadian(f32);
f32 JMASinDegree(f32);
f32 JMASinRadian(f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);
void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);

namespace JMath {
    f32 fastReciprocal(f32);

    template<typename T>
    f32 fastSqrt(T);

    void gekko_ps_copy12(void *, const void *);
    void gekko_ps_copy16(void *, const void *);
};

namespace JMathInlineVEC {
    #ifdef __MWERKS__

    __attribute__((always_inline))
    inline void PSVECCopy(register const Vec *src, register Vec *dest) {
        register f32 xy, z;
        __asm {
            lfs z, 8(src)
            psq_l xy, 0(src), 0, 0
            stfs z, 8(dest)
            psq_st xy, 0(dest), 0, 0
        }
    }

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
    inline void PSVECAdd2(register const Vec* vec1, register const Vec* vec2, register Vec* dst)
    {
        register f32 v2z, d1z, v1z, v2xy, v1xy, d1xy;
        
        __asm {            
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            ps_add    d1xy, v1xy, v2xy
            psq_l     v1z,   8(vec1), 1, 0
            psq_l     v2z,   8(vec2), 1, 0
            ps_add    d1z, v1z, v2z
            psq_st    d1xy, 0(dst), 0, 0
            psq_st    d1z,  8(dst), 1, 0
        }
    }

    __attribute__((always_inline))
    inline void PSVECAdd3(register const Vec* vec1, register const Vec* vec2, register Vec* dst)
    {
        register f32 v2xy, v1xy, d1xy, d1z, v2z, v1z;
        
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
            ps_sub    dxy, v1xy, v2xy
            psq_st    dxy, 0(dst), 0, 0
            
            psq_l     v1z,   8(vec1), 1, 0
            psq_l     v2z,   8(vec2), 1, 0
            ps_sub    dz, v1z, v2z
            psq_st    dz,  8(dst), 1, 0            
        }
    }

    __attribute__((always_inline))
    inline void PSVECSubtract2(register const Vec *vec1, register const Vec *vec2, register Vec *dst) {
        register f32 v1z, v2z, v1xy, v2xy, dxy, dz;
        __asm {
            psq_l     v2xy, 0(vec1), 0, 0
            psq_l     v1xy, 0(vec2), 0, 0
            ps_sub    dxy, v2xy, v1xy
            psq_st    dxy, 0(dst), 0, 0
                
            psq_l     v2z,   8(vec1), 1, 0
            psq_l     v1z,   8(vec2), 1, 0
            ps_sub    dz, v2z, v1z
            psq_st    dz,  8(dst), 1, 0            
        }
    }
    
    __attribute__((always_inline))
    inline void PSVECMultiply(register const Vec* vec1, register const Vec* vec2, register Vec* dst) {
        register f32 v1xy, v2xy, dxy, v1z, v2z, dz;
        __asm {
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            ps_mul    dxy, v1xy, v2xy
            psq_st    dxy, 0(dst), 0, 0
            lfs v1z, 8(vec1)
            lfs v2z, 8(vec2)
            fmuls v2z, v1z, v2z
            stfs v2z, 8(dst)
        }
    }

    __attribute__((always_inline))
    inline f32 PSVECSquareMag(register const Vec* src)
    {
        register f32 xy, z, ret;
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
    inline void PSVECNegate(register const Vec* src, register Vec* dst)
    {
        register f32 xy;
        __asm {
            psq_l xy, 0(src), 0, 0
            ps_neg xy, xy
            psq_st xy, 0(dst), 0, 0
        }
        dst->z = -src->z;
    }
    
    __attribute__((always_inline))
    inline f32 PSVECSquareDistance(const register Vec *a, const register Vec *b)
    {
        
        register f32 dyz, dxy, sqdist;
        register f32 v0xy, v1yz, v0yz, v1xy;

        asm
        {
            psq_l    v0yz, 4(a), 0, 0 
            psq_l    v1yz, 4(b), 0, 0
            ps_sub   dyz, v0yz, v1yz
        
            psq_l    v0xy, 0(a), 0, 0
            psq_l    v1xy, 0(b), 0, 0
            ps_mul   dyz, dyz, dyz
            ps_sub   dxy, v0xy, v1xy
        
            ps_madd  sqdist, dxy, dxy, dyz
            ps_sum0  sqdist, sqdist, dyz, dyz
        }    

        return sqdist;
    }
    #else
    void PSVECCopy(const Vec *, Vec *);
    void PSVECAdd(const Vec *, const Vec *, Vec *);
    void PSVECAdd2(const Vec *, const Vec *, Vec *);
    void PSVECAdd3(const Vec *, const Vec *, Vec *);
    void PSVECSubtract(const Vec *, const Vec *, Vec *);
    void PSVECSubtract2(const Vec *, const Vec *, Vec *);
    f32 PSVECDotProduct(const Vec *, const Vec *);
    f32 PSVECSquareMag(const Vec *);
    void PSVECNegate(const Vec *, Vec *);
    f32 PSVECSquareDistance(const Vec *, const Vec *);
    void PSVECMultiply(const Vec*, const Vec*, Vec*);
#endif
};
