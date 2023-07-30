#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

void __SetSURegs(u32 tmap, u32 tcoord) NO_INLINE {
    u32 w, h;
    GXBool s_bias, t_bias;

    w = ((gx->tImage0[tmap] & 0x3FF));
    h = ((gx->tImage0[tmap] & 0xFFC00) >> 10);
    FAST_FLAG_SET(gx->suTs0[tcoord], w, 0, 16);
    FAST_FLAG_SET(gx->suTs1[tcoord], h, 0, 16);

    s_bias = ((gx->tMode0[tmap] & 0x3)) == GX_REPEAT;
    t_bias = ((gx->tMode0[tmap] & 0xC) >> 2) == GX_REPEAT;
    FAST_FLAG_SET(gx->suTs0[tcoord], s_bias, 16, 1);
    FAST_FLAG_SET(gx->suTs1[tcoord], t_bias, 16, 1);

    GX_WRITE_RA_REG(gx->suTs0[tcoord]);
    GX_WRITE_RA_REG(gx->suTs1[tcoord]);
    gx->bpSentNot = GX_FALSE;
}

void __GXSetSUTexRegs(void) {
    u32 nStages, nIndStages, i, map, tmap, coord;
    u32* ptref;

    if (gx->tcsManEnab == 0xFF) {
        return;
    }

    nStages = ((gx->genMode & 0x3C00) >> 10) + 1;
    nIndStages = ((gx->genMode & 0x70000) >> 16);

    for (i = 0; i < nIndStages; i++) {
        switch (i + GX_INDTEXSTAGE0) {
            case GX_INDTEXSTAGE0:
                tmap = (gx->iref) & 0x7;
                coord = ((gx->iref & 0x38) >> 3);
                break;
            case GX_INDTEXSTAGE1:
                tmap = ((gx->iref & 0x1C0) >> 6);
                coord = ((gx->iref & 0xE00) >> 9);
                break;
            case GX_INDTEXSTAGE2:
                tmap = ((gx->iref & 0x7000) >> 12);
                coord = ((gx->iref & 0x38000) >> 15);
                break;
            case GX_INDTEXSTAGE3:
                tmap = ((gx->iref & 0x1C0000) >> 18);
                coord = ((gx->iref & 0xE00000) >> 21);
                break;
        }

        if (!(gx->tcsManEnab & (1 << coord))) {
            __SetSURegs(tmap, coord);
        }
    }

    for (i = 0; i < nStages; i++) {
        ptref = &gx->tref[i / 2];
        map = gx->texmapId[i];
        tmap = map & ~256;

        if (i & 1) {
            coord = ((*ptref & 0x38000) >> 15);
        }
        else {
            coord = ((*ptref & 0x38) >> 3);
        }

        if ((tmap != GX_TEXMAP_NULL) && !(gx->tcsManEnab & (1 << coord)) && (gx->tevTcEnab & (1 << i))) {
            __SetSURegs(tmap, coord);
        }
    }
}