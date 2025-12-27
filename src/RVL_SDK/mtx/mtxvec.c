#include "revolution/mtx.h"

// clang-format off

asm void PSMTXMultVec
(
    const register Mtx m,
    const register Vec *src,
          register Vec *dst
)
{
    nofralloc

    psq_l       fp0, 0(src), 0, 0
    psq_l       fp2, 0(m), 0, 0
    psq_l       fp1, 8(src), 1, 0
    ps_mul      fp4, fp2, fp0
    psq_l       fp3, 8(m), 0, 0
    ps_madd     fp5, fp3, fp1, fp4
    psq_l       fp8, 16(m), 0, 0
    ps_sum0     fp6, fp5, fp6, fp5
    psq_l       fp9, 24(m), 0, 0
    ps_mul      fp10, fp8, fp0
    psq_st      fp6, 0(dst), 1, 0
    ps_madd     fp11, fp9, fp1, fp10
    psq_l       fp2, 32(m), 0, 0
    ps_sum0     fp12, fp11, fp12, fp11
    psq_l       fp3, 40(m), 0, 0
    ps_mul      fp4, fp2, fp0
    psq_st      fp12, 4(dst), 1, 0
    ps_madd     fp5, fp3, fp1, fp4
    ps_sum0     fp6, fp5, fp6, fp5
    psq_st      fp6, 8(dst), 1, 0

    blr
}

asm void PSMTXMultVecSR
(
    const register Mtx m,
    const register Vec *src,
          register Vec *dst
)
{
    nofralloc

    psq_l   fp0, 0(m), 0, 0
    psq_l   fp6, 0(src), 0, 0
    psq_l   fp2, 16(m), 0, 0
    ps_mul  fp8, fp0, fp6
    psq_l   fp4, 32(m), 0, 0
    ps_mul  fp10, fp2, fp6
    psq_l   fp7, 8(src), 1, 0
    ps_mul  fp12, fp4, fp6
    psq_l   fp3, 24(m), 0, 0
    ps_sum0 fp8, fp8, fp8, fp8
    psq_l   fp5, 40(m), 0, 0
    ps_sum0 fp10, fp10, fp10, fp10
    psq_l   fp1, 8(m), 0, 0
    ps_sum0 fp12, fp12, fp12, fp12
    ps_madd fp9, fp1, fp7, fp8
    psq_st  fp9, 0(dst), 1, 0
    ps_madd fp11, fp3, fp7, fp10
    psq_st  fp11, 4(dst), 1, 0
    ps_madd fp13, fp5, fp7, fp12
    psq_st  fp13, 8(dst), 1, 0

    blr
}

asm void PSMTXMultVecArraySR
(
    const register Mtx    m,
    const register Vec   *srcBase,
          register Vec   *dstBase,
          register u32    count
)
{
    nofralloc

    psq_l       fp13, 0(m), 0, 0
    psq_l       fp12, 16(m), 0, 0
    subi        count, count, 1
    psq_l       fp11, 8(m), 1, 0
    ps_merge00  fp0, fp13, fp12
    subi        dstBase, dstBase, 4
    psq_l       fp10, 24(m), 1, 0
    ps_merge11  fp1, fp13, fp12
    mtctr       count
    psq_l       fp3, 32(m), 0, 0
    ps_merge00  fp2, fp11, fp10
    psq_l       fp4, 40(m), 1, 0
    psq_l       fp6, 0(srcBase), 0, 0
    psq_lu      fp7, 8(srcBase), 1, 0
    ps_muls0    fp8, fp0, fp6
    ps_mul      fp9, fp3, fp6
    ps_madds1   fp8, fp1, fp6, fp8
    ps_madd     fp10, fp4, fp7, fp9

_mloop:
    psq_lu      fp6, 4(srcBase), 0, 0
    ps_madds0   fp12, fp2, fp7, fp8
    psq_lu      fp7, 8(srcBase), 1, 0
    ps_sum0     fp13, fp10, fp9, fp9
    ps_muls0    fp8, fp0, fp6
    ps_mul      fp9, fp3, fp6
    psq_stu     fp12, 4(dstBase), 0, 0
    ps_madds1   fp8, fp1, fp6, fp8
    psq_stu     fp13, 8(dstBase), 1, 0
    ps_madd     fp10, fp4, fp7, fp9
    bdnz        _mloop

    ps_madds0   fp12, fp2, fp7, fp8
    ps_sum0     fp13, fp10, fp9, fp9
    psq_stu     fp12, 4(dstBase), 0, 0
    psq_stu     fp13, 8(dstBase), 1, 0

    blr
}

// clang-format on
