#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>

static void WriteProjPS(const register f32 proj[6], register volatile void* dest)
 {
    register f32 p01, p23, p45;

    asm {
        psq_l p01, 0(proj),  0, 0
        psq_l p23, 8(proj),  0, 0
        psq_l p45, 16(proj), 0, 0
        psq_st p01, 0(dest), 0, 0
        psq_st p23, 0(dest), 0, 0
        psq_st p45, 0(dest), 0, 0
    }
}

void __GXSetProjection(void) {
    u32 reg = CP_XF_LOADREGS(0x1020, 6);
    GX_WRITE_U8(CP_OPCODE(0, 2));
    GX_WRITE_U32(reg);
    WriteProjPS(gx->projMtx, (volatile void*)0xCC008000);
    GX_WRITE_XF_MEM_U32(0x1026, gx->projType);
}

void __GXSetViewport(void) {
    f32 sx, sy, sz, ox, oy, oz, zmin, zmax;
    u32 reg;

    sx = gx->vpWd / 2.0f;
    sy = - gx->vpHt / 2.0f;
    ox = gx->vpLeft + (gx->vpWd / 2.0f) + 342.0f;
    oy = gx->vpTop + (gx->vpHt / 2.0f) + 342.0f;

    zmin = gx->vpNearz * gx->zScale;
    zmax = gx->vpFarz * gx->zScale;

    sz = (zmax - zmin);
    oz = zmax + gx->zOffset;

    reg = CP_XF_LOADREGS(0x101A, 5);

    GX_WRITE_U8(CP_OPCODE(0, 2));
    GX_WRITE_U32(reg);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 0, sx);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 1, sy);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 2, sz);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 3, ox);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 4, oy);
    GX_WRITE_XF_MEM_F32(XF_SCALEX + 5, oz);
}

void GXSetScissor(u32 left, u32 top, u32 width, u32 height) {
    u32 _top, _left, bottom, right;

    _top = top + (u32)342.0f;
    _left = left + (u32)342.0f;
    bottom = _top + height - 1;
    right = _left + width - 1;

    SET_FLAG(gx->suScis0, _top, 0, 11);
    SET_FLAG(gx->suScis0, _left, 12, 11);

    SET_FLAG(gx->suScis1, bottom, 0, 11);
    SET_FLAG(gx->suScis1, right, 12, 11);

    GX_WRITE_REG(gx->suScis0);
    GX_WRITE_REG(gx->suScis1);
    gx->bpSentNot = GX_FALSE;
}

void __GXSetMatrixIndex(GXAttr matIdxAttr) {
    if (matIdxAttr < GX_VA_TEX4MTXIDX) {
        GX_WRITE_CP_STRM_REG(3, 0, gx->matIdxA);
        GX_WRITE_XF_REG(0x1018, gx->matIdxA, 0);
    }
    else {
        GX_WRITE_CP_STRM_REG(4, 0, gx->matIdxB);
        GX_WRITE_XF_REG(0x1019, gx->matIdxB, 0);
    }
    
    gx->bpSentNot = GX_TRUE;
}