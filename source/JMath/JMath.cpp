#include "JMath/JMath.h"

void JMAVECScaleAdd(const Vec *src_0, const Vec *src_1, Vec *dest, f32 val)
{
    __asm
    {
        psq_l f0, 0(r3), 0, 0
        psq_l f2, 0(r4), 0, 0
        psq_l f3, 8(r3), 1, 0
        ps_madds0 f0, f0, f1, f2
        psq_l f4,  8(r4), 1, 0
        ps_madds0 f1, f3, f1, f4
        psq_st f0, 0(r5), 0, 0
        psq_st f1, 8(r5), 1, 0
    }
}

void JMAVECLerp(const Vec *src_0, const Vec *src_1, Vec *dest, f32 val)
{
    __asm
    {
        psq_l f0, 0(r3), 0, 0
        psq_l f2, 0(r4), 0, 0
        lfs f3, 8(r3)
        lfs f4, 8(r4)
        ps_sub f2, f2, f0
        fsubs f4, f4, f3
        ps_madds0 f2, f2, f1, f0
        fmadds f4, f4, f1, f3
        psq_st f2, 0(r5), 0, 0
        stfs f4, 8(r5)
    }
}

namespace JMathInlineVEC
{
    void PSVECAdd(const Vec *src_0, const Vec *src_1, Vec *dest)
    {
        __asm
        {
            psq_l f0, 0(r3), 0, 0
            psq_l f1, 0(r4), 0, 0
            psq_l f2, 8(r3), 1, 0
            ps_add f0, f0, f1
            psq_l f3, 8(r4),1, 0
            ps_add f1, f2, f3
            psq_st f0, 0(r5), 0, 0
            psq_st f1, 8(r5), 1, 0
        }
    }

    void PSVECSutract(const Vec *src_0, const Vec *src_1, Vec *dest)
    {
        __asm
        {
            psq_l f0, 0(r3), 0, 0
            psq_l f1, 0(r4), 0, 0
            psq_l f2, 8(r3), 1, 0
            ps_sub f0, f0, f1
            psq_l f3, 8(r4),1, 0
            ps_sub f1, f2, f3
            psq_st f0, 0(r5), 0, 0
            psq_st f1, 8(r5), 1, 0
        }
    }
};