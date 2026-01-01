#include "revolution/mtx.h"

extern f64 tan(f64);

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) {
    f32 angle;
    f32 cot;
    f32 tmp;
    angle = fovY * 0.5f;
    angle = MTXDegToRad(angle);
    cot = tan(angle);
    cot = 1 / cot;

    m[0][0] = cot / aspect;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = cot;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;

    tmp = 1.0f / (f - n);
    m[2][2] = -(n)*tmp;
    m[2][3] = -(f * n) * tmp;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = -1.0f;
    m[3][3] = 0.0f;
}

void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    f32 tmp;
    tmp = 1.0f / (r - l);
    m[0][0] = 2.0f * tmp;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = -(r + l) * tmp;

    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = 2.0f * tmp;
    m[1][2] = 0.0f;
    m[1][3] = -(t + b) * tmp;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;

    tmp = 1.0f / (f - n);
    m[2][2] = -(1.0f) * tmp;
    m[2][3] = -(f)*tmp;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

// clang-format off

void PSMTX44Identity( register Mtx44 m )
{
    register f32 c1 = 1.0F;
    register f32 c0 = 0.0F;
    
    asm
    {
        stfs        c1,  0(m);
        psq_st      c0,  4(m), 0, 0;
        psq_st      c0, 12(m), 0, 0;
        stfs        c1, 20(m);
        psq_st      c0, 24(m), 0, 0;
        psq_st      c0, 32(m), 0, 0;
        stfs        c1, 40(m);
        psq_st      c0, 44(m), 0, 0;
        psq_st      c0, 52(m), 0, 0;
        stfs        c1, 60(m);
    }
}

asm void PSMTX44Copy( const register Mtx44 src, register Mtx44 dst )
{
    nofralloc;
    psq_l       fp1,  0(src), 0, 0;
    psq_st      fp1,  0(dst), 0, 0;
    psq_l       fp1,  8(src), 0, 0;
    psq_st      fp1,  8(dst), 0, 0;
    psq_l       fp1, 16(src), 0, 0;
    psq_st      fp1, 16(dst), 0, 0;
    psq_l       fp1, 24(src), 0, 0;
    psq_st      fp1, 24(dst), 0, 0;
    psq_l       fp1, 32(src), 0, 0;
    psq_st      fp1, 32(dst), 0, 0;
    psq_l       fp1, 40(src), 0, 0;
    psq_st      fp1, 40(dst), 0, 0;
    psq_l       fp1, 48(src), 0, 0;
    psq_st      fp1, 48(dst), 0, 0;
    psq_l       fp1, 56(src), 0, 0;
    psq_st      fp1, 56(dst), 0, 0;
    blr;
}

//clang-format on
