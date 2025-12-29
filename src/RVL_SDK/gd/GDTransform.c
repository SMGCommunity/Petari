#include "revolution/gd/GDTransform.h"
#include "revolution/gd.h"

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5, u32 t6, u32 t7) {
    u32 regA;
    u32 regB;

    regA = MATIDX_REG_A(pn, t0, t1, t2, t3);
    regB = MATIDX_REG_B(t4, t5, t6, t7);

    GDWriteCPCmd(CP_MATINDEX_A, regA);
    GDWriteCPCmd(CP_MATINDEX_B, regB);
    GDWriteXFCmdHdr(XF_MATINDEX_A, 2);
    GDWrite_u32(regA);
    GDWrite_u32(regB);
}
