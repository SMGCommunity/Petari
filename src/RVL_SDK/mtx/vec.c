#include "revolution/mtx.h"
#include <cmath>

// clang-format off

#define RET_REG fp1
#define V1_XY   fp2
#define V1_Z    fp3
#define V2_XY   fp4
#define V2_Z    fp5
#define D1_XY   fp6
#define D1_Z    fp7
#define D2_XY   fp8
#define D2_Z    fp9
#define W1_XY   fp10
#define W1_Z    fp11
#define W2_XY   fp12
#define W2_Z    fp13

asm void PSVECAdd
(
    const register Vec *vec1,
    const register Vec *vec2,
    register Vec *dst
)
{
    nofralloc;

    psq_l     V1_XY,  0(vec1), 0, 0;
    psq_l     V2_XY,  0(vec2), 0, 0;
    ps_add  D1_XY, V1_XY, V2_XY;
    psq_st    D1_XY,  0(dst), 0, 0;
    psq_l     V1_Z,   8(vec1), 1, 0;
    psq_l     V2_Z,   8(vec2), 1, 0;
    ps_add  D1_Z, V1_Z, V2_Z;
    psq_st    D1_Z,   8(dst), 1, 0;

    blr
}

asm void PSVECSubtract
(
    const register Vec *vec1,
    const register Vec *vec2,
          register Vec *dst
)
{
    nofralloc
    psq_l     V1_XY,  0(vec1), 0, 0;
    psq_l     V2_XY,  0(vec2), 0, 0;
    ps_sub  D1_XY, V1_XY, V2_XY;
    psq_st    D1_XY, 0(dst), 0, 0;
    psq_l     V1_Z,   8(vec1), 1, 0;
    psq_l     V2_Z,   8(vec2), 1, 0;
    ps_sub  D1_Z, V1_Z, V2_Z;
    psq_st    D1_Z,  8(dst), 1, 0;

    blr
}

void PSVECScale
(
    const register Vec *src,
          register Vec *dst,
          register f32 mult
)
{
    register f32    vxy, vz, rxy, rz;

    asm
    {
        psq_l     vxy, 0(src), 0, 0
        psq_l     vz,  8(src), 1, 0
        ps_muls0    rxy, vxy, mult
        psq_st    rxy, 0(dst), 0, 0
        ps_muls0    rz,  vz,  mult
        psq_st    rz,  8(dst), 1, 0
    }

}

void PSVECNormalize
(
    const register Vec *vec1,
          register Vec *dst
)
{
    register f32 c_half  = 0.5F;
    register f32 c_three = 3.0F;
    register f32 v1_xy, v1_z;
    register f32 xx_zz, xx_yy;
    register f32 sqsum;
    register f32 rsqrt;
    register f32 nwork0, nwork1;

    asm
    {
        psq_l       v1_xy, 0(vec1), 0, 0;
        ps_mul      xx_yy, v1_xy, v1_xy;
        psq_l       v1_z, 8(vec1), 1, 0;
        ps_madd     xx_zz, v1_z, v1_z, xx_yy;
        ps_sum0     sqsum, xx_zz, v1_z, xx_yy;
        frsqrte     rsqrt, sqsum;
        fmuls       nwork0, rsqrt, rsqrt;
        fmuls       nwork1, rsqrt, c_half;
        fnmsubs     nwork0, nwork0, sqsum, c_three;
        fmuls       rsqrt, nwork0, nwork1;
        ps_muls0    v1_xy, v1_xy, rsqrt;
        psq_st      v1_xy, 0(dst), 0, 0;
        ps_muls0    v1_z, v1_z, rsqrt;
        psq_st      v1_z, 8(dst), 1, 0;
    }

}

// clang-format on

f32 C_VECSquareMag(const Vec* v) {
    f32 sqmag;
    sqmag = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
    return sqmag;
}

f32 C_VECMag(const Vec* v) {
    return sqrt(C_VECSquareMag(v));
}

// clang-format off

f32 PSVECMag ( const register Vec *v )
{
    register f32    vxy, vzz, sqmag;
    register f32    rmag, nwork0, nwork1;
    register f32    c_three, c_half, c_zero;

    c_half = 0.5F;

    asm
    {
        psq_l       vxy, 0(v), 0, 0
        ps_mul      vxy, vxy, vxy
        lfs         vzz, 8(v)
        fsubs       c_zero, c_half, c_half
        ps_madd     sqmag, vzz, vzz, vxy
        ps_sum0     sqmag, sqmag, vxy, vxy
        fcmpu       cr0, sqmag, c_zero
        beq-        __exit
        frsqrte     rmag, sqmag
    }
    c_three = 3.0F;
    
    asm
    {
        fmuls       nwork0, rmag, rmag
        fmuls       nwork1, rmag, c_half
        fnmsubs     nwork0, nwork0, sqmag, c_three
        fmuls       rmag, nwork0, nwork1
        fmuls       sqmag, sqmag, rmag

    __exit:
    }

    return sqmag;
}

asm f32 PSVECDotProduct(const register Vec *vec1, const register Vec *vec2)
{
    nofralloc;

    psq_l    fp2, 4(vec1), 0, 0;
    psq_l    fp3, 4(vec2), 0, 0;

    ps_mul   fp2, fp2, fp3;

    psq_l    fp5, 0(vec1), 0, 0;
    psq_l    fp4, 0(vec2), 0, 0;
    
    ps_madd  fp3, fp5, fp4, fp2;
    ps_sum0  fp1, fp3, fp2, fp2;

    blr;
}

asm void PSVECCrossProduct
(
    const register Vec *vec1,
    const register Vec *vec2,
          register Vec *dst
)
{
    nofralloc;
    psq_l       fp1, 0(vec2), 0, 0
    lfs         fp2, 8(vec1)
    psq_l       fp0, 0(vec1), 0, 0
    ps_merge10  fp6, fp1, fp1
    lfs         fp3, 8(vec2)
    ps_mul      fp4, fp1, fp2
    ps_muls0    fp7, fp1, fp0
    ps_msub     fp5, fp0, fp3, fp4
    ps_msub     fp8, fp0, fp6, fp7
    ps_merge11  fp9, fp5, fp5
    ps_merge01  fp10, fp5, fp8 
    psq_st      fp9, 0(dst), 1, 0
    ps_neg      fp10, fp10
    psq_st      fp10, 4(dst), 0, 0
    blr;
}

f32 PSVECDistance( const register Vec *a, const register Vec *b )
{
    register f32    v0yz, v1yz, v0xy, v1xy;
    register f32    dyz, dxy, sqdist, rdist;
    register f32    nwork0, nwork1;
    register f32    c_half, c_three, c_zero;

    asm
    {
        psq_l       v0yz, 4(a), 0, 0 
        psq_l       v1yz, 4(b), 0, 0
        ps_sub      dyz, v0yz, v1yz
    
        psq_l       v0xy, 0(a), 0, 0
        psq_l       v1xy, 0(b), 0, 0 
        ps_mul      dyz, dyz, dyz
        ps_sub      dxy, v0xy, v1xy
    }
    c_half  = 0.5F;
   
    asm
    {
        ps_madd     sqdist, dxy, dxy, dyz
        fsubs       c_zero, c_half, c_half
        ps_sum0     sqdist, sqdist, dyz, dyz
        fcmpu       cr0, c_zero, sqdist
        beq-        __exit
    }
    c_three = 3.0F;

    asm
    { 
        frsqrte     rdist, sqdist
        fmuls       nwork0, rdist, rdist
        fmuls       nwork1, rdist, c_half
        fnmsubs     nwork0, nwork0, sqdist, c_three
        fmuls       rdist, nwork0, nwork1
        fmuls       sqdist, sqdist, rdist

    __exit:
    }
    
    return sqdist;
}
