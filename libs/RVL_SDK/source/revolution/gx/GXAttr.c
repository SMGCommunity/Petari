#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

static void __GXXfVtxSpecs(void) {
    u32 nCols, nNrm, nTex, reg;

    nNrm = (gx->hasBiNrms) ? 2 : ((gx->hasNrms) ? 1 : 0);
    nCols = (u32)__cntlzw((gx->vcdLo & (0x6000 | 0x18000)) >> 13);
    nCols = (33 - nCols) >> 1;

    nTex = (u32)__cntlzw((gx->vcdHi & (1 << 16) - 1));
    nTex = (33 - nTex) >> 1;

    reg = (nCols) | (nNrm << 2) | (nTex << 4);
    GX_WRITE_XF_REG(0x1008, reg, 0);
    gx->bpSentNot = GX_TRUE;
}

void __GXSetVCD(void) {
    GX_WRITE_CP_STRM_REG(5, 0, gx->vcdLo);
    GX_WRITE_CP_STRM_REG(6, 0, gx->vcdHi);
    __GXXfVtxSpecs();
}

void __GXCalculateVLim(void) {
    if (gx->vNum > 0) {
        static u8 tbl1[4] = { 0,  4, 1, 2 };
        static u8 tbl2[4] = { 0,  8, 1, 2 };
        static u8 tbl3[4] = { 0, 12, 1, 2 };
        GXCompCnt nc;
        u32 vlm, b;
        u32 vl, vh, va;
        
        vl = gx->vcdLo;
        vh = gx->vcdHi;
        va = gx->vatA[GX_VTXFMT0];

        nc = (GXCompCnt)((va & 0x00000200) >> 9);

        vlm  = ((vl >> 0) & 0x1);
        vlm += ((vl >> 1) & 0x1);
        vlm += ((vl >> 2) & 0x1);
        vlm += ((vl >> 3) & 0x1);
        vlm += ((vl >> 4) & 0x1);
        vlm += ((vl >> 5) & 0x1);
        vlm += ((vl >> 6) & 0x1);
        vlm += ((vl >> 7) & 0x1);
        vlm += ((vl >> 8) & 0x1);
        
        vlm += tbl3[((vl >> 9) & 0x3)];

        b = (u32)((nc == GX_NRM_NBT) ? 3 : 1);
        vlm += tbl3[((vl >> 11) & 0x3)] * b;

        vlm += tbl1[((vl >> 13) & 0x3)];
        vlm += tbl1[((vl >> 15) & 0x3)];
        
        vlm += tbl2[((vh >> (0 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (1 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (2 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (3 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (4 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (5 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (6 * 2)) & 0x3)];
        vlm += tbl2[((vh >> (7 * 2)) & 0x3)];

        gx->vLim = (u16) vlm;
    }
}

void __GXSetVAT(void) {
    s32 i;
    u32 dirty;

    dirty = (u32)(gx->dirtyVAT);
    i = 0;

    do {
        if (dirty & 0x1) {
            GX_WRITE_CP_STRM_REG(7, i, gx->vatA[i]);
            GX_WRITE_CP_STRM_REG(8, i, gx->vatB[i]);
            GX_WRITE_CP_STRM_REG(9, i, gx->vatC[i]);
        }

        dirty >>= 1;
        ++i;
    } while (dirty);

    GX_WRITE_U8(0);
    gx->dirtyVAT = 0;
}