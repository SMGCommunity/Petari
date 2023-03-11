#include "RFL_Model.h"

const GXColor cWhite = { 0xFF, 0xFF, 0xFF, 0xFF };

const RFLDrawCoreSetting cDefaultDrawCoreSetting2Tev = {
    1,
    GX_TEXCOORD0,
    GX_TEXMAP0,
    2,
    GX_TEV_SWAP0,
    GX_KCOLOR0,
    GX_TEVPREV,
    GX_PNMTX0,
    FALSE
};

const RFLDrawCoreSetting cDefaultDrawCoreSetting1Tev = {
    1,
    GX_TEXCOORD0,
    GX_TEXMAP0,
    1,
    GX_TEV_SWAP0,
    GX_KCOLOR0,
    GX_TEVPREV,
    GX_PNMTX0,
    FALSE
};

const GXColor cFacelineColor[6] = {
    { 0xF0, 0xD8, 0xC4, 0xFF },
    { 0xFF, 0xBC, 0x80, 0xFF },
    { 0xD8, 0x88, 0x50, 0xFF },
    { 0xFF, 0xB0, 0x90, 0xFF },
    { 0x90, 0x50, 0x30, 0xFF },
    { 0x52, 0x2E, 0x1C, 0xFF }
};

const GXColor cHairColor[8] = {
    { 0x1E, 0x1A, 0x18, 0xFF },
    { 0x38, 0x20, 0x15, 0xFF },
    { 0x55, 0x26, 0x17, 0xFF },
    { 0x70, 0x40, 0x24, 0xFF },
    { 0x72, 0x72, 0x78, 0xFF },
    { 0x49, 0x36, 0x1A, 0xFF },
    { 0x7A, 0x59, 0x28, 0xFF },
    { 0xC1, 0x9F, 0x64, 0xFF }
};

const GXColor cBeardColor[8] = {
    { 0x1E, 0x1A, 0x18, 0xFF },
    { 0x38, 0x20, 0x15, 0xFF },
    { 0x55, 0x26, 0x17, 0xFF },
    { 0x70, 0x40, 0x24, 0xFF },
    { 0x72, 0x72, 0x78, 0xFF },
    { 0x49, 0x36, 0x1A, 0xFF },
    { 0x7A, 0x59, 0x28, 0xFF },
    { 0xC1, 0x9F, 0x64, 0xFF }
};

const GXColor cGlassColor[6] = {
    { 0x10, 0x10, 0x10, 0xFF },
    { 0x60, 0x38, 0x10, 0xFF },
    { 0x98, 0x18, 0x10, 0xFF },
    { 0x20, 0x30, 0x60, 0xFF },
    { 0x90, 0x58, 0x00, 0xFF },
    { 0x60, 0x58, 0x50, 0xFF}
};

const GXColor cFavoriteColor[12] = {
    { 0xB8, 0x40, 0x30, 0xFF },
    { 0xF0, 0x78, 0x28, 0xFF },
    { 0xF8, 0xD8, 0x20, 0xFF },
    { 0x80, 0xC8, 0x28, 0xFF },
    { 0x00, 0x74, 0x28, 0xFF },
    { 0x20, 0x48, 0x98, 0xFF },
    { 0x40, 0xA0, 0xD8, 0xFF },
    { 0xE8, 0x60, 0x78, 0xFF },
    { 0x70, 0x2C, 0xA8, 0xFF },
    { 0x48, 0x38, 0x18, 0xFF },
    { 0xE0, 0xE0, 0xE0, 0xFF },
    { 0x18, 0x18, 0x14, 0xFF }
};

GXColor RFLGetFavoriteColor(RFLFavoriteColor color) {
    return cFavoriteColor[color];
}

void RFLLoadDrawSetting(const RFLDrawSetting *pSettings) {
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_NEVER, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetZCompLoc(pSettings->zCompLoc);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetDither(0);
    GXSetDstAlpha(0, 0);

    if (pSettings->lightEnable) {
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting2Tev);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_S16, 0xE);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S16, 0xD);
        GXSetNumTexGens(1);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_REG, pSettings->lightMask, pSettings->diffFn, pSettings->attnFn);
        GXSetChanCtrl(GX_ALPHA0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);

        GXSetChanAmbColor(GX_COLOR0, pSettings->ambientColor);
        GXSetChanMatColor(GX_COLOR0, cWhite);
    }
    else {
        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting1Tev);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_S16, 0xE);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S16, 0xD);
        GXSetNumTexGens(1);
        GXSetNumChans(0);
    }
}

void RFLDrawOpa(const RFLCharModel *pModel) {
    RFLDrawOpaCore(pModel, &cDefaultDrawCoreSetting2Tev);
}

void RFLDrawXlu(const RFLCharModel *pModel) {
    RFLDrawXluCore(pModel, &cDefaultDrawCoreSetting2Tev);
}

void RFLLoadVertexSetting(const RFLDrawCoreSetting *pSettings) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_S16, 0xE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S16, 0xD);
    GXSetNumTexGens(pSettings->txcGenNum);
}

void RFLLoadMaterialSetting(const RFLDrawCoreSetting *pSettings) {
    GXSetTevSwapModeTable(pSettings->tevSwapTable, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable((pSettings->tevSwapTable + 1), GX_CH_RED, GX_CH_ALPHA, GX_CH_BLUE, GX_CH_GREEN);
    GXSetNumTevStages(pSettings->tevStageNum);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, pSettings->tevOutRegID);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, pSettings->tevOutRegID);
    GXSetTevKColorSel(GX_TEVSTAGE0, (pSettings->tevKColorID + 12));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_8_8);
}

// RFLDrawOpaCore
// RFLDrawXluCore
