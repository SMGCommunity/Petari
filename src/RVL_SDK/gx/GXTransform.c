#include "revolution/gx/GXEnum.h"
#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>
#include <revolution/mtx.h>

void GXProject(f32 x, f32 y, f32 z, const f32 mtx[3][4], const f32* pm, const f32* vp, f32 *sx, f32 *sy, f32 *sz) {
    Vec peye;
    f32 xc, yc, zc, wc;

    peye.x = mtx[0][0]*x + mtx[0][1]*y + mtx[0][2]*z + mtx[0][3];
    peye.y = mtx[1][0]*x + mtx[1][1]*y + mtx[1][2]*z + mtx[1][3];
    peye.z = mtx[2][0]*x + mtx[2][1]*y + mtx[2][2]*z + mtx[2][3];

    if (pm[0] == (f32)GX_PERSPECTIVE) {
        xc = peye.x * pm[1] + peye.z * pm[2];
        yc = peye.y * pm[3] + peye.z * pm[4];
        zc = peye.z * pm[5] + pm[6];
        wc = 1.0f / -peye.z;
    } else {
        xc = peye.x * pm[1] + pm[2];
        yc = peye.y * pm[3] + pm[4];
        zc = peye.z * pm[5] + pm[6];
        wc = 1.0f;
    }

    *sx = xc * vp[2]/2 * wc + vp[0] + vp[2]/2;
    *sy = -yc * vp[3]/2 * wc + vp[1] + vp[3]/2;
    *sz = zc * (vp[5] - vp[4]) * wc + vp[5];
}

inline void WriteProjPS(const register f32 proj[6], register volatile void* dest)
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

inline void Copy6Floats(register f32* dst, register const f32* src) {
    register f32 ps_0, ps_1, ps_2;

    asm volatile {
        psq_l  ps_0,  0(src), 0, 0
        psq_l  ps_1,  8(src), 0, 0
        psq_l  ps_2, 16(src), 0, 0
        psq_st ps_0,  0(dst), 0, 0
        psq_st ps_1,  8(dst), 0, 0
        psq_st ps_2, 16(dst), 0, 0
    }
}

void __GXSetProjection(void) {
    volatile void* wgpipe = (volatile void*)0xCC008000;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(0x61020);
    WriteProjPS(gx->projMtx, wgpipe);
    GX_WRITE_XF_MEM_U32(0x1026, gx->projType);
}


void GXSetProjection(const f32 mtx[4][4], GXProjectionType type) {
    gx->projType = type;
    gx->projMtx[0] = mtx[0][0];
    gx->projMtx[2] = mtx[1][1];
    gx->projMtx[4] = mtx[2][2];
    gx->projMtx[5] = mtx[2][3];

    if (type == GX_ORTHOGRAPHIC) {
        gx->projMtx[1] = mtx[0][3];
        gx->projMtx[3] = mtx[1][3];
    } 
    else {
        gx->projMtx[1] = mtx[0][2];
        gx->projMtx[3] = mtx[1][2];
    }

    gx->dirtyState |= 0x8000000;
}

void GXSetProjectionv(const f32 *ptr) {
    gx->projType = (ptr[0] == 0.0f ? GX_PERSPECTIVE  : GX_ORTHOGRAPHIC);
    Copy6Floats(gx->projMtx, &ptr[1]);
    gx->dirtyState |= 0x8000000;
}

void GXGetProjectionv(f32 *ptr) {
    ptr[0] = gx->projType ? 1.0f : 0.0f;
    Copy6Floats(&ptr[1], gx->projMtx);
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

inline void WriteMTXPS4x3(const register f32 src[3][4], register volatile void* dst) {
    register f32 ps_0, ps_1, ps_2, ps_3, ps_4, ps_5;

    // clang-format off
    asm volatile {
        psq_l  ps_0,  0(src), 0, 0
        psq_l  ps_1,  8(src), 0, 0
        psq_l  ps_2, 16(src), 0, 0
        psq_l  ps_3, 24(src), 0, 0
        psq_l  ps_4, 32(src), 0, 0
        psq_l  ps_5, 40(src), 0, 0

        psq_st ps_0, 0(dst),  0, 0
        psq_st ps_1, 0(dst),  0, 0
        psq_st ps_2, 0(dst),  0, 0
        psq_st ps_3, 0(dst),  0, 0
        psq_st ps_4, 0(dst),  0, 0
        psq_st ps_5, 0(dst),  0, 0
    }
    // clang-format on
}

void GXLoadPosMtxImm(const f32 mtx[3][4], u32 id) {
    u32 reg, addr;
    volatile void* wgpipe = (volatile void*)0xCC008000;
    addr = id << 2;
    reg = CP_XF_LOADREGS(addr, 11);
    GX_WRITE_U8(CP_OPCODE(0, CP_CMD_XF_LOADREGS));
    GX_WRITE_U32(reg);
    WriteMTXPS4x3(mtx, wgpipe);
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
