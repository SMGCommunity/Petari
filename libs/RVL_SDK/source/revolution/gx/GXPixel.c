#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>

// GXSetFog

void GXSetFogRangeAdj(GXBool enable, u16 c, const GXFogAdjTable *pTable) {
    u32 i;
    u32 v0, v1;

    if (enable) {
        /* this loop is unrolled */
        for (i = 0; i < 0xA; i += 2) {
            v0 = 0;
            SET_FLAG(v0, pTable->r[i], 0, 12);
            SET_FLAG(v0, pTable->r[i + 1], 12, 12);
            SET_FLAG(v0, 0xE9 + (i / 2), 24, 8);
            GX_WRITE_REG(v0);
        }
    }

    v1 = 0;
    SET_FLAG(v1, c + 0x156, 0, 10);
    SET_FLAG(v1, enable, 10, 1);
    SET_FLAG(v1, 0xE8, 24, 8);
    GX_WRITE_REG(v1);
    gx->bpSentNot = GX_FALSE;
}

void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src_factor, GXBlendFactor dest_factor, GXLogicOp operation) {
    u32 reg;
    u32 blend;

    reg = gx->cmode0;
    blend = (u32)(mode & 1);
    SET_FLAG(reg, (mode == 3), 11, 1);
    SET_FLAG(reg, blend, 0, 1);
    SET_FLAG(reg, mode == 2, 1, 1);
    SET_FLAG(reg, operation, 12, 4); 
    SET_FLAG(reg, src_factor, 8, 3);
    SET_FLAG(reg, dest_factor, 5, 3);
    GX_WRITE_REG(reg);
    gx->cmode0 = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetColorUpdate(GXBool update) {
    u32 reg;

    reg = gx->cmode0;
    SET_FLAG(reg, update, 3, 1);
    GX_WRITE_REG(reg);
    gx->cmode0 = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetAlphaUpdate(GXBool update) {
    u32 reg;

    reg = gx->cmode0;
    SET_FLAG(reg, update, 4, 1);
    GX_WRITE_REG(reg);
    gx->cmode0 = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetZMode(GXBool compare, GXCompare func, GXBool update) {
    u32 reg;

    reg = gx->zmode;
    SET_FLAG(reg, compare, 0, 1);
    SET_FLAG(reg, func, 1, 3);
    SET_FLAG(reg, update, 4, 1);
    GX_WRITE_REG(reg);
    gx->zmode = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetZCompLoc(GXBool before) {
    SET_FLAG(gx->peCtrl, before, 6, 1);
    GX_WRITE_REG(gx->peCtrl);
    gx->bpSentNot = GX_FALSE;
}

void GXSetPixelFmt(GXPixelFmt format, GXZFmt16 z_format) {
    u32 ctrl;
    GXBool aa;

    static u32 p2f[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    ctrl = gx->peCtrl;
    SET_FLAG(gx->peCtrl, p2f[format], 0, 3);
    SET_FLAG(gx->peCtrl, z_format, 3, 3);

    if (ctrl != gx->peCtrl) {
        GX_WRITE_REG(gx->peCtrl);

        aa = ((format == 2) ? GX_TRUE : GX_FALSE);
        SET_FLAG(gx->genMode, aa, 9, 1);
        gx->dirtyState |= 4;
    }

    if (p2f[format] == 4) {
        SET_FLAG(gx->cmode1, (format - 4) & 3, 9, 2);
        SET_FLAG(gx->cmode1, 0x42, 24, 8);
        GX_WRITE_REG(gx->cmode1);
    }

    gx->bpSentNot = GX_FALSE;
}

void GXSetDither(GXBool dither) {
    u32 reg;

    reg = gx->cmode0;
    SET_FLAG(reg, dither, 2, 1);
    GX_WRITE_REG(reg);
    gx->cmode0 = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetDstAlpha(GXBool enable, u8 alpha) {
    u32 reg;

    reg = gx->cmode1;
    SET_FLAG(reg, alpha, 0, 8);
    SET_FLAG(reg, enable, 8, 1);
    GX_WRITE_REG(reg);
    gx->cmode1 = reg;
    gx->bpSentNot = GX_FALSE;
}

void GXSetFieldMask(GXBool odd, GXBool even) {
    u32 reg;

    reg = 0;
    SET_FLAG(reg, even, 0, 1);
    SET_FLAG(reg, odd, 1, 1);
    SET_FLAG(reg, 0x44, 24, 8);
    GX_WRITE_REG(reg);
    gx->bpSentNot = GX_FALSE;
}

void GXSetFieldMode(GXBool mode, GXBool ratio) {
    u32 reg;

    SET_FLAG(gx->lpSize, ratio, 22, 1);
    GX_WRITE_REG(gx->lpSize);

    __GXFlushTextureState();
    reg = mode | 0x68000000;
    GX_WRITE_REG(reg);
    __GXFlushTextureState();
}