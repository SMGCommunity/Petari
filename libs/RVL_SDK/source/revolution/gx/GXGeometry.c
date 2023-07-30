#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

inline void __GXSetAmbMat(u32 amState) {
    if (amState & 0x100) {
        GX_WRITE_XF_REG(0x100A, gx->ambColor[0], 0);
    }

    if (amState & 0x200) {
        GX_WRITE_XF_REG(0x100B, gx->ambColor[1], 0);
    }

    if (amState & 0x400) {
        GX_WRITE_XF_REG(0x100C, gx->matColor[0], 0);
    }

    if (amState & 0x800) {
        GX_WRITE_XF_REG(0x100D, gx->matColor[1], 0);
    }
}

inline void __GXSetLightChan(u32 chState) {
    u32 d, i, chIndx = 0x100E;

    if (chState & 0x1000000) {
        d = ((gx->genMode & 0x70) >> 4);
        GX_WRITE_XF_REG(0x1009, d, 0);
    }

    d = (chState & 0xF000) >> 12;
    i = 0;

    while (d) {
        if (d & 0x1) {
            GX_WRITE_XF_REG(chIndx, gx->chanCtrl[i], 0);
        }

        i++;
        chIndx++;
        d >>= 1;
    }
}

inline void __GXSetTexGen(u32 tgState) {
    u32 d, i, tgIndx = 0x1040;
    u32 dtgIndx = 0x1050;

    if (tgState & 0x2000000) {
        d = gx->genMode & 0xF;
        GX_WRITE_XF_REG(0x103F, d, 0);
    }

    d = (tgState & 0xFF0000) >> 16;
    i = 0;
    while (d) {
        if (d & 0x1) {
            GX_WRITE_XF_REG(tgIndx, gx->texGenCtrl[i], 0);
            GX_WRITE_XF_REG(dtgIndx, gx->dualTexGenCtrl[i], 0);
        }
    
        dtgIndx++;
        tgIndx++;
        i++;
        d >>= 1;    
    }
}

void __GXSetDirtyState() {
    u32 dState = gx->dirtyState;

    if (dState & 0x00000001) {
        __GXSetSUTexRegs();
    }

    if (dState & 0x00000002) {
        __GXUpdateBPMask();
    }

    if (dState & 0x00000004) {
        __GXSetGenMode();
    }

    if (dState & 0x00000008) {
        __GXSetVCD();
    }

    if (dState & 0x00000010) {
        __GXSetVAT();
    }
        
    if (dState & (0x00000008|0x00000010)) {
        __GXCalculateVLim();
    }

    dState &= 0xFFFFFF00;

    if (dState) {
        u32 dStateLocal;

        dStateLocal = dState & 0x00000F00;

        if (dStateLocal) {
            __GXSetAmbMat(dStateLocal);
        }
        
        dStateLocal = dState & (0x01000000|0x0000F000);

        if (dStateLocal) {
            __GXSetLightChan(dStateLocal);
        }

        dStateLocal = dState & (0x02000000|0x00FF0000);

        if (dStateLocal) {
            __GXSetTexGen(dStateLocal);
        }

        dStateLocal = dState & 0x04000000;
        if (dStateLocal) {
            __GXSetMatrixIndex(GX_VA_PNMTXIDX);
            __GXSetMatrixIndex(GX_VA_TEX4MTXIDX);
        }

        dStateLocal = dState & 0x10000000;
        if (dStateLocal) {
            __GXSetViewport();
        }

        dStateLocal = dState & 0x8000000;
        if (dStateLocal) {
            __GXSetProjection();
        } 

        gx->bpSentNot = GX_TRUE;
    }

    gx->dirtyState = 0;
}

void __GXSetGenMode(void) {
    GX_WRITE_RA_REG(gx->genMode);
    gx->bpSentNot = GX_FALSE;
}