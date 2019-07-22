#include "os/mtx.h"

void PSMTXIdentity(Mtx matrix)
{
    __asm
    {
		lfs f0, (mtx_zero)(r2)
		lfs f1, (mtx_one)(r2)
		psq_st f0, 8(r3), 0, 0
		ps_merge10 f2, f1, f0
		ps_merge01 f1, f0, f1
		psq_st f0, 0x18(r3), 0, 0
		psq_st f0, 0x20(r3), 0, 0
		psq_st f1, 0x10(r3), 0, 0
		psq_st f2, 0(r3), 0, 0
		psq_st f2, 0x28(r3), 0, 0
		blr
    }
}

void PSMTXCopy(Mtx src, Mtx dest)
{
    __asm
    {
        psq_l f0, 0(r3), 0, 0
        psq_st f0, 0(r4), 0, 0
        psq_l f1, 8(r3), 0, 0
        psq_st f1, 8(r4), 0, 0
        psq_l f2, 0x10(r3), 0, 0
        psq_st f2, 0x10(r4), 0, 0
        psq_l f3, 0x18(r3), 0, 0
        psq_st f3, 0x18(r4), 0, 0
        psq_l f4, 0x20(r3), 0, 0
        psq_st f4, 0x20(r4), 0, 0
        psq_l f5, 0x28(r3), 0, 0
        psq_st f5, 0x28(r4), 0, 0
    }
}

void PSMTXRotTrig(Mtx mtx, s8 axis, f32 sin, f32 cos)
{
    __asm
    {
        frsp f5, f1
        ori r0, r4, 0x20
        frsp f4, f2
        cmplwi r0, 0x78
        lfs f0, (mtx_zero)(r2)
        ps_neg f2, f5
        lfs f1, (mtx_one)(r2)
        beq isEqual78
        cmplwi r0, 0x79
        beq isEqual79
        cmplwi r0, 0x7A
        beq isEqual7A
        blr

        isEqual78:
            ps_merge00 f3, f5, f4
            psq_st f1, 0(r3), 1, 0
            ps_merge00 f1, f4, f2
            psq_st f0, 4(r3), 0, 0
            psq_st f0, 0xC(r3), 0, 0
            psq_st f0, 0x1C(r3), 0, 0
            psq_st f0, 0x2C(r3), 1, 0
            psq_st f3, 0x24(r3), 0, 0
            psq_st f1, 0x14(r3), 0, 0
            blr

        isEqual79:
            ps_merge00 f3, f4, f0
            psq_st    f0, 0x18(r3), 0, 0
            ps_merge00 f1, f0, f1
            ps_merge00 f2, f2, f0
            ps_merge00 f0, f5, f0
            psq_st    f3, 0(r3), 0, 0
            psq_st    f3, 0x28(r3), 0, 0
            psq_st    f1, 0x10(r3), 0, 0
            psq_st    f0, 8(r3), 0, 0
            psq_st    f2, 0x20(r3), 0, 0
            blr

        isEqual7A:
            ps_merge00 f3, f5, f4
            psq_st    f0, 8(r3), 0, 0
            ps_merge00 f2, f4, f2
            ps_merge00 f1, f1, f0
            psq_st    f0, 0x18(r3), 0, 0
            psq_st    f0, 0x20(r3), 0, 0
            psq_st    f3, 0x10(r3), 0, 0
            psq_st    f2, 0(r3), 0, 0
            psq_st    f1, 0x28(r3), 0, 0
            blr
    }
}