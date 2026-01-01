#include "revolution/mtx.h"
#include <cmath>

static f32 Unit01[] = {0.0f, 1.0f};

// clang-format off
void PSMTXIdentity(register Mtx m) {
    register f32 c_zero = 0.0F;
    register f32 c_one = 1.0F;
    register f32 c_01;
    register f32 c_10;

    asm
    {
        psq_st      c_zero, 8(m),   0, 0
        ps_merge01  c_01, c_zero, c_one 
        psq_st      c_zero, 24(m),  0, 0
        ps_merge10  c_10, c_one, c_zero 
        psq_st      c_zero, 32(m),  0, 0
        psq_st      c_01,   16(m),  0, 0 
        psq_st      c_10,   0(m),   0, 0
        psq_st      c_10,   40(m),  0, 0
    }
}

asm void PSMTXCopy(const register Mtx src,register Mtx dst) {
    nofralloc
    
    psq_l       fp0, 0(src),   0, 0
    psq_st      fp0, 0(dst),   0, 0
    psq_l       fp1, 8(src),   0, 0
    psq_st      fp1, 8(dst),   0, 0
    psq_l       fp2, 16(src),  0, 0
    psq_st      fp2, 16(dst),  0, 0
    psq_l       fp3, 24(src),  0, 0
    psq_st      fp3, 24(dst),  0, 0
    psq_l       fp4, 32(src),  0, 0
    psq_st      fp4, 32(dst),  0, 0
    psq_l       fp5, 40(src),  0, 0
    psq_st      fp5, 40(dst),  0, 0

    blr
}

asm void PSMTXConcat
(
    const register Mtx mA,
    const register Mtx mB,
          register Mtx mAB
)
{
    nofralloc

#define A00_A01 fp0
#define A02_A03 fp1
#define A10_A11 fp2
#define A12_A13 fp3
#define A20_A21 fp4
#define A22_A23 fp5

#define B00_B01 fp6
#define B02_B03 fp7
#define B10_B11 fp8
#define B12_B13 fp9
#define B20_B21 fp10
#define B22_B23 fp11

#define D00_D01 fp12
#define D02_D03 fp13
#define D10_D11 fp14
#define D12_D13 fp15
#define D20_D21 fp2
#define D22_D23 fp0

#define UNIT01  fp31

    stwu    r1, -64(r1)
    psq_l   A00_A01, 0(mA), 0, 0
    stfd    fp14, 8(r1)
    psq_l   B00_B01, 0(mB), 0, 0
    addis   r6, 0, Unit01@ha
    psq_l   B02_B03, 8(mB), 0, 0
    stfd    fp15, 16(r1)
    addi    r6, r6, Unit01@l
    stfd    fp31, 40(r1)
    psq_l   B10_B11, 16(mB), 0, 0
    ps_muls0 D00_D01, B00_B01, A00_A01
    psq_l   A10_A11, 16(mA), 0, 0
    ps_muls0 D02_D03, B02_B03, A00_A01
    psq_l   UNIT01, 0(r6), 0, 0
    ps_muls0 D10_D11, B00_B01, A10_A11
    psq_l   B12_B13, 24(mB), 0, 0
    ps_muls0 D12_D13, B02_B03, A10_A11
    psq_l   A02_A03, 8(mA), 0, 0 
    ps_madds1 D00_D01, B10_B11, A00_A01, D00_D01      
    psq_l   A12_A13, 24(mA), 0, 0
    ps_madds1 D10_D11, B10_B11, A10_A11, D10_D11
    psq_l   B20_B21, 32(mB), 0, 0
    ps_madds1 D02_D03, B12_B13, A00_A01, D02_D03
    psq_l   B22_B23, 40(mB), 0, 0
    ps_madds1 D12_D13, B12_B13, A10_A11, D12_D13
    psq_l   A20_A21, 32(mA), 0, 0
    psq_l   A22_A23, 40(mA), 0, 0
    ps_madds0 D00_D01, B20_B21, A02_A03, D00_D01
    ps_madds0 D02_D03, B22_B23, A02_A03, D02_D03 
    ps_madds0 D10_D11, B20_B21, A12_A13, D10_D11
    ps_madds0 D12_D13, B22_B23, A12_A13, D12_D13
    psq_st  D00_D01, 0(mAB), 0, 0
    ps_muls0 D20_D21, B00_B01, A20_A21
    ps_madds1 D02_D03, UNIT01, A02_A03, D02_D03
    ps_muls0 D22_D23, B02_B03, A20_A21
    psq_st  D10_D11, 16(mAB), 0, 0 
    ps_madds1 D12_D13, UNIT01, A12_A13, D12_D13
    psq_st  D02_D03, 8(mAB), 0, 0
    ps_madds1 D20_D21, B10_B11, A20_A21, D20_D21
    ps_madds1 D22_D23, B12_B13, A20_A21, D22_D23
    ps_madds0 D20_D21, B20_B21, A22_A23, D20_D21
    lfd    fp14, 8(r1)
    psq_st  D12_D13, 24(mAB), 0, 0
    ps_madds0 D22_D23, B22_B23, A22_A23, D22_D23
    psq_st  D20_D21, 32(mAB), 0, 0
    ps_madds1 D22_D23, UNIT01, A22_A23, D22_D23
    lfd    fp15, 16(r1)
    psq_st  D22_D23, 40(mAB), 0, 0
    lfd    fp31, 40(r1)
    addi   r1, r1, 64
    blr 

#undef A00_A01
#undef A02_A03
#undef A10_A11
#undef A12_A13
#undef A20_A21
#undef A22_A23

#undef B00_B01
#undef B02_B03
#undef B10_B11
#undef B12_B13
#undef B20_B21
#undef B22_B23

#undef D00_D01
#undef D02_D03
#undef D10_D11
#undef D12_D13
#undef D20_D21
#undef D22_D23

#undef UNIT01 
}

asm u32 PSMTXInverse(const register Mtx src, register Mtx inv)
{
    nofralloc

    psq_l       fp0, 0(src), 1, 0
    psq_l       fp1, 4(src), 0, 0
    psq_l       fp2, 16(src), 1, 0
    ps_merge10  fp6, fp1, fp0
    psq_l       fp3, 20(src), 0, 0
    psq_l       fp4, 32(src), 1, 0
    ps_merge10  fp7, fp3, fp2
    psq_l       fp5, 36(src), 0, 0

    ps_mul      fp11, fp3, fp6
    ps_mul      fp13, fp5, fp7
    ps_merge10  fp8, fp5, fp4
    ps_msub     fp11, fp1, fp7, fp11
    ps_mul      fp12, fp1, fp8
    ps_msub     fp13, fp3, fp8, fp13
    ps_mul      fp10, fp3, fp4
    ps_msub     fp12, fp5, fp6, fp12
    ps_mul      fp9,  fp0, fp5
    ps_mul      fp8,  fp1, fp2

    ps_sub      fp6, fp6, fp6
    ps_msub     fp10, fp2, fp5, fp10
    ps_mul      fp7, fp0, fp13
    ps_msub     fp9,  fp1, fp4, fp9
    ps_madd     fp7, fp2, fp12, fp7
    ps_msub     fp8,  fp0, fp3, fp8
    ps_madd     fp7, fp4, fp11, fp7

    ps_cmpo0    cr0, fp7, fp6
    bne         _regular

    addi        r3, 0, 0
    blr

_regular:

    fres        fp0, fp7
    ps_add      fp6, fp0, fp0
    ps_mul      fp5, fp0, fp0
    ps_nmsub    fp0, fp7, fp5, fp6

    lfs         fp1, 12(src)
    ps_muls0    fp13, fp13, fp0
    lfs         fp2, 28(src)
    ps_muls0    fp12, fp12, fp0
    lfs         fp3, 44(src)
    ps_muls0    fp11, fp11, fp0
    ps_merge00  fp5, fp13, fp12
    ps_muls0    fp10, fp10, fp0
    ps_merge11  fp4, fp13, fp12
    ps_muls0    fp9,  fp9,  fp0

    psq_st      fp5,  0(inv), 0, 0
    ps_mul      fp6, fp13, fp1
    psq_st      fp4,  16(inv), 0, 0
    ps_muls0    fp8,  fp8,  fp0
    ps_madd     fp6, fp12, fp2, fp6
    psq_st      fp10, 32(inv), 1, 0
    ps_nmadd    fp6, fp11, fp3, fp6
    psq_st      fp9,  36(inv), 1, 0
    ps_mul      fp7, fp10, fp1
    ps_merge00  fp5, fp11, fp6
    psq_st      fp8,  40(inv), 1, 0
    ps_merge11  fp4, fp11, fp6
    psq_st      fp5,  8(inv), 0, 0
    ps_madd     fp7, fp9,  fp2, fp7
    psq_st      fp4,  24(inv), 0, 0
    ps_nmadd    fp7, fp8,  fp3, fp7
    addi        r3, 0, 1
    psq_st      fp7,  44(inv), 1, 0

    blr
}


asm u32 PSMTXInvXpose ( const register Mtx src, register Mtx invX )
{
    nofralloc
    psq_l       fp0, 0(src), 1, 0
    psq_l       fp1, 4(src), 0, 0
    psq_l       fp2, 16(src), 1, 0
    ps_merge10  fp6, fp1, fp0
    psq_l       fp3, 20(src), 0, 0
    psq_l       fp4, 32(src), 1, 0
    ps_merge10  fp7, fp3, fp2
    psq_l       fp5, 36(src), 0, 0
    ps_mul      fp11, fp3, fp6
    ps_merge10  fp8, fp5, fp4
    ps_mul      fp13, fp5, fp7
    ps_msub     fp11, fp1, fp7, fp11
    ps_mul      fp12, fp1, fp8
    ps_msub     fp13, fp3, fp8, fp13
    ps_msub     fp12, fp5, fp6, fp12
    ps_mul      fp10, fp3, fp4
    ps_mul      fp9,  fp0, fp5
    ps_mul      fp8,  fp1, fp2
    ps_msub     fp10, fp2, fp5, fp10
    ps_msub     fp9,  fp1, fp4, fp9
    ps_msub     fp8,  fp0, fp3, fp8
    ps_mul      fp7, fp0, fp13
    ps_sub      fp1, fp1, fp1
    ps_madd     fp7, fp2, fp12, fp7
    ps_madd     fp7, fp4, fp11, fp7
    ps_cmpo0    cr0, fp7, fp1
    bne         _regular
    addi        r3, 0, 0
    blr

  _regular:        
    fres        fp0, fp7
    psq_st      fp1,  12(invX), 1, 0
    ps_add      fp6, fp0, fp0
    ps_mul      fp5, fp0, fp0
    psq_st      fp1,  28(invX), 1, 0

    ps_nmsub    fp0, fp7, fp5, fp6
    psq_st      fp1,  44(invX), 1, 0
    ps_muls0    fp13, fp13, fp0
    ps_muls0    fp12, fp12, fp0
    ps_muls0    fp11, fp11, fp0
    psq_st      fp13,  0(invX), 0, 0
    psq_st      fp12,  16(invX), 0, 0
    ps_muls0    fp10, fp10, fp0
    ps_muls0    fp9,  fp9,  fp0
    psq_st      fp11,  32(invX), 0, 0
    psq_st      fp10,  8(invX), 1, 0
    ps_muls0    fp8,  fp8,  fp0

    addi        r3, 0, 1
    psq_st      fp9,   24(invX), 1, 0
    psq_st      fp8,   40(invX), 1, 0
    
    blr
}

void PSMTXRotTrig (
    register Mtx    m,
    register char   axis,
    register f32    sinA,
    register f32    cosA )
{
    register f32    fc0, fc1, nsinA;
    register f32    fw0, fw1, fw2, fw3;

    asm
    {
        frsp        sinA, sinA
        frsp        cosA, cosA
    }

    fc0 = 0.0F;
    fc1 = 1.0F;

    asm
    {
        ori         axis, axis, 0x20
        ps_neg      nsinA, sinA
        cmplwi      axis, 'x'
        beq         _case_x
        cmplwi      axis, 'y'
        beq         _case_y
        cmplwi      axis, 'z'
        beq         _case_z
        b           _end

    _case_x:
        psq_st      fc1,  0(m), 1, 0
        psq_st      fc0,  4(m), 0, 0
        ps_merge00  fw0, sinA, cosA
        psq_st      fc0, 12(m), 0, 0
        ps_merge00  fw1, cosA, nsinA
        psq_st      fc0, 28(m), 0, 0
        psq_st      fc0, 44(m), 1, 0
        psq_st      fw0, 36(m), 0, 0
        psq_st      fw1, 20(m), 0, 0
        b           _end;

    _case_y:
        ps_merge00  fw0, cosA, fc0
        ps_merge00  fw1, fc0, fc1
        psq_st      fc0, 24(m), 0, 0
        psq_st      fw0,  0(m), 0, 0
        ps_merge00  fw2, nsinA, fc0
        ps_merge00  fw3, sinA, fc0
        psq_st      fw0, 40(m), 0, 0;
        psq_st      fw1, 16(m), 0, 0;
        psq_st      fw3,  8(m), 0, 0;
        psq_st      fw2, 32(m), 0, 0;
        b           _end;

    _case_z:
        psq_st      fc0,  8(m), 0, 0
        ps_merge00  fw0, sinA, cosA
        ps_merge00  fw2, cosA, nsinA
        psq_st      fc0, 24(m), 0, 0
        psq_st      fc0, 32(m), 0, 0
        ps_merge00  fw1, fc1, fc0
        psq_st      fw0, 16(m), 0, 0
        psq_st      fw2,  0(m), 0, 0
        psq_st      fw1, 40(m), 0, 0

    _end:
    
    }
}

static void __PSMTXRotAxisRadInternal(
    register Mtx    m,
    const register Vec *axis,
    register f32    sT,
    register f32    cT )
{
    register f32    tT, fc0;
    register f32    tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32    tmp5, tmp6, tmp7, tmp8, tmp9;
    
    tmp9 = 0.5F;
    tmp8 = 3.0F;
    
    asm
    {
        frsp        cT, cT
        psq_l       tmp0, 0(axis), 0, 0
        frsp        sT, sT
        lfs         tmp1, 8(axis)
        ps_mul      tmp2, tmp0, tmp0
        fadds       tmp7, tmp9, tmp9
        ps_madd     tmp3, tmp1, tmp1, tmp2
        fsubs       fc0, tmp9, tmp9
        ps_sum0     tmp4, tmp3, tmp1, tmp2
        fsubs       tT, tmp7, cT
        frsqrte     tmp5, tmp4
        fmuls       tmp2, tmp5, tmp5
        fmuls       tmp3, tmp5, tmp9
        fnmsubs     tmp2, tmp2, tmp4, tmp8
        fmuls       tmp5, tmp2, tmp3
        ps_merge00  cT, cT, cT
        ps_muls0    tmp0, tmp0, tmp5
        ps_muls0    tmp1, tmp1, tmp5
        ps_muls0    tmp4, tmp0, tT
        ps_muls0    tmp9, tmp0, sT
        ps_muls0    tmp5, tmp1, tT
        ps_muls1    tmp3, tmp4, tmp0
        ps_muls0    tmp2, tmp4, tmp0
        ps_muls0    tmp4, tmp4, tmp1
        fnmsubs     tmp6, tmp1, sT, tmp3
        fmadds      tmp7, tmp1, sT, tmp3
        ps_neg      tmp0, tmp9
        ps_sum0     tmp8, tmp4, fc0, tmp9
        ps_sum0     tmp2, tmp2, tmp6, cT
        ps_sum1     tmp3, cT, tmp7, tmp3
        ps_sum0     tmp6, tmp0, fc0 ,tmp4
            psq_st      tmp8, 8(m), 0, 0
        ps_sum0     tmp0, tmp4, tmp4, tmp0
            psq_st      tmp2, 0(m), 0, 0
        ps_muls0    tmp5, tmp5, tmp1
            psq_st      tmp3, 16(m), 0, 0

        ps_sum1     tmp4, tmp9, tmp0, tmp4
            psq_st      tmp6, 24(m), 0, 0
        ps_sum0     tmp5, tmp5, fc0, cT
            psq_st      tmp4, 32(m), 0, 0
            psq_st      tmp5, 40(m), 0, 0
    }
}

void PSMTXRotAxisRad(
    Mtx             m,
    const Vec      *axis,
    f32             rad )
{
    f32     sinT, cosT;
    
    sinT = sin(rad);
    cosT = cos(rad);
    
    __PSMTXRotAxisRadInternal(m, axis, sinT, cosT);
}

void PSMTXTrans(
    register Mtx m,
    register f32 xT,
    register f32 yT,
    register f32 zT
)
{
    register f32 c0 = 0.0F;
    register f32 c1 = 1.0F;

    asm
    {
        stfs        xT,     12(m)
        stfs        yT,     28(m)
        psq_st      c0,      4(m), 0, 0
        psq_st      c0,     32(m), 0, 0
        stfs        c0,     16(m)
        stfs        c1,     20(m)
        stfs        c0,     24(m)
        stfs        c1,     40(m)
        stfs        zT,     44(m)
        stfs        c1,      0(m)
    }
}

asm void PSMTXTransApply(
    const register Mtx src,
    register Mtx dst,
    register f32 xT,
    register f32 yT,
    register f32 zT )
{
    nofralloc;
    psq_l       fp4, 0(src),        0, 0;
    frsp        xT, xT;
    psq_l       fp5, 8(src),        0, 0;
    frsp        yT, yT;
    psq_l       fp7, 24(src),       0, 0;
    frsp        zT, zT;
    psq_l       fp8, 40(src),       0, 0;
    psq_st      fp4, 0(dst),        0, 0;
    ps_sum1     fp5, xT, fp5, fp5;
    psq_l       fp6, 16(src),       0, 0;   
    psq_st      fp5, 8(dst),        0, 0;
    ps_sum1     fp7, yT, fp7, fp7;
    psq_l       fp9, 32(src),       0, 0;
    psq_st      fp6, 16(dst),       0, 0;
    ps_sum1     fp8, zT, fp8, fp8;
    psq_st      fp7, 24(dst),       0, 0;
    psq_st      fp9, 32(dst),       0, 0;
    psq_st      fp8, 40(dst),       0, 0;
    blr;
}

void PSMTXScale(
    register Mtx m,
    register f32 xS,
    register f32 yS,
    register f32 zS
)
{
    register f32 c0 = 0.0F;

    asm
    {
        stfs        xS,      0(m)
        psq_st      c0,      4(m), 0, 0
        psq_st      c0,     12(m), 0, 0
        stfs        yS,     20(m)
        psq_st      c0,     24(m), 0, 0
        psq_st      c0,     32(m), 0, 0
        stfs        zS,     40(m)
        stfs        c0,     44(m)
    }
}

asm void PSMTXScaleApply (
    const register Mtx src,
    register Mtx dst,
    register f32 xS,
    register f32 yS,
    register f32 zS )
{
    nofralloc;
    frsp        xS, xS;
    psq_l       fp4, 0(src),        0, 0;
    frsp        yS, yS;
    psq_l       fp5, 8(src),        0, 0;
    frsp        zS, zS;
    ps_muls0    fp4, fp4, xS;
    psq_l       fp6, 16(src),       0, 0;
    ps_muls0    fp5, fp5, xS;
    psq_l       fp7, 24(src),       0, 0;
    ps_muls0    fp6, fp6, yS;
    psq_l       fp8, 32(src),       0, 0;
    psq_st      fp4, 0(dst),        0, 0;
    ps_muls0    fp7, fp7, yS;
    psq_l       fp2, 40(src),       0, 0;
    psq_st      fp5, 8(dst),        0, 0;
    ps_muls0    fp8, fp8, zS;
    psq_st      fp6, 16(dst),       0, 0;
    ps_muls0    fp2, fp2, zS;
    psq_st      fp7, 24(dst),       0, 0;
    psq_st      fp8, 32(dst),       0, 0;
    psq_st      fp2, 40(dst),       0, 0;
    blr;
}

void PSMTXQuat ( register Mtx m, const register Quaternion *q )
{
    register f32    c_zero, c_one, c_two, scale;
    register f32    tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32    tmp5, tmp6, tmp7, tmp8, tmp9;

    c_one = 1.0F;

    asm
    {
        psq_l       tmp0, 0(q), 0, 0
        psq_l       tmp1, 8(q), 0, 0
        fsubs       c_zero, c_one, c_one
        fadds       c_two, c_one, c_one
        ps_mul      tmp2, tmp0, tmp0
        ps_merge10  tmp5, tmp0, tmp0
        ps_madd     tmp4, tmp1, tmp1, tmp2
        ps_mul      tmp3, tmp1, tmp1
        ps_sum0     scale, tmp4, tmp4, tmp4
        ps_muls1    tmp7, tmp5, tmp1
        fres        tmp9, scale
        ps_sum1     tmp4, tmp3, tmp4, tmp2
        ps_nmsub    scale, scale, tmp9, c_two
        ps_muls1    tmp6, tmp1, tmp1
        ps_mul      scale, tmp9, scale
        ps_sum0     tmp2, tmp2, tmp2, tmp2
        fmuls       scale, scale, c_two
        ps_madd     tmp8, tmp0, tmp5, tmp6
        ps_msub     tmp6, tmp0, tmp5, tmp6
        psq_st      c_zero, 12(m), 1, 0
        ps_nmsub    tmp2, tmp2, scale, c_one
        ps_nmsub    tmp4, tmp4, scale, c_one
        psq_st      c_zero, 44(m), 1, 0
        ps_mul      tmp8, tmp8, scale
        ps_mul      tmp6, tmp6, scale
        psq_st      tmp2, 40(m), 1, 0
        ps_madds0   tmp5, tmp0, tmp1, tmp7
        ps_merge00  tmp1, tmp8, tmp4
        ps_nmsub    tmp7, tmp7, c_two, tmp5
        ps_merge10  tmp0, tmp4, tmp6
        psq_st      tmp1, 16(m), 0, 0
        ps_mul      tmp5, tmp5, scale
        ps_mul      tmp7, tmp7, scale
        psq_st      tmp0,  0(m), 0, 0
        psq_st      tmp5,  8(m), 1, 0
        ps_merge10  tmp3, tmp7, c_zero
        ps_merge01  tmp9, tmp7, tmp5
        psq_st      tmp3, 24(m), 0, 0
        psq_st      tmp9, 32(m), 0, 0
        
    }
}
// clang-format on

void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target) {
    Vec vLook, vRight, vUp;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    PSVECNormalize(&vLook, &vLook);
    PSVECCrossProduct(camUp, &vLook, &vRight);
    PSVECNormalize(&vRight, &vRight);
    PSVECCrossProduct(&vLook, &vRight, &vUp);

    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -(camPos->x * vRight.x + camPos->y * vRight.y + camPos->z * vRight.z);
    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -(camPos->x * vUp.x + camPos->y * vUp.y + camPos->z * vUp.z);
    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -(camPos->x * vLook.x + camPos->y * vLook.y + camPos->z * vLook.z);
}

// fun float stuff going on here
extern f64 tan(f64);

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, float scaleS, float scaleT, float transS, float transT) {
    f32 angle;
    f32 cot;
    angle = fovY * 0.5f;
    angle = MTXDegToRad(angle);
    cot = tan(angle);
    cot = 1/cot;
    m[0][0] = (cot / aspect) * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = -transS;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = cot * scaleT;
    m[1][2] = -transT;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, float scaleS, float scaleT, float transS, float transT) {
    f32 tmp;
    tmp = 1.0f / (r - l);
    m[0][0] = (2.0f * tmp * scaleS);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = ((-(r + l) * tmp) * scaleS) + transS;
    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = (2.0f * tmp) * scaleT;
    m[1][2] = 0.0f;
    m[1][3] = ((-(t + b) * tmp) * scaleT) + transT;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 0.0f;
    m[2][3] = 1.0f;
}
