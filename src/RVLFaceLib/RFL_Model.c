#include <RVLFaceLibInternal.h>
#include <revolution/OS.h>
#include <cstdio>
#include <cstddef>

#define NUM_VTX_POS(size) ((size) / VTX_POS_SIZE)
#define SIZE_VTX_POS(count) ((count) * VTX_POS_SIZE)

#define NUM_VTX_NRM(size) ((size) / VTX_NRM_SIZE)
#define SIZE_VTX_NRM(count) ((count) * VTX_NRM_SIZE)

#define NUM_VTX_TXC(size) ((size) / VTX_TXC_SIZE)
#define SIZE_VTX_TXC(count) ((count) * VTX_TXC_SIZE)

RFLiCoordinateData coordinateData = {1, 2, 0, FALSE, FALSE, FALSE};

const RFLDrawCoreSetting cDefaultDrawCoreSetting2Tev = {
    1,          GX_TEXCOORD0, GX_TEXMAP0, 2,    GX_TEV_SWAP0,
    GX_KCOLOR0, GX_TEVPREV,   GX_PNMTX0,  FALSE};

const RFLDrawCoreSetting cDefaultDrawCoreSetting1Tev = {
    1,          GX_TEXCOORD0, GX_TEXMAP0, 1,    GX_TEV_SWAP0,
    GX_KCOLOR0, GX_TEVPREV,   GX_PNMTX0,  FALSE};

static const GXColor cFacelineColor[RFLi_MAX_FACELINE_COLOR + 1] = {
    // clang-format off
    {240, 216, 196, 255}, 
    {255, 188, 128, 255}, 
    {216, 136, 80,  255},  
    {255, 176, 144, 255}, 
    {152, 80,  48,  255},   
    {82,  46,  28,  255},
    // clang-format on
};

static const GXColor cHairColor[RFLi_MAX_HAIR_COLOR + 1] = {
    // clang-format off
    {30,  26,  24,  255},    
    {56,  32,  21,  255},    
    {85,  38,  23,  255},    
    {112, 64,  36,  255},   
    {114, 114, 120, 255}, 
    {73,  54,  26,  255},    
    {122, 89,  40,  255},   
    {193, 159, 100, 255},
    // clang-format on
};

static const GXColor cBeardColor[RFLi_MAX_BEARD_COLOR + 1] = {
    // clang-format off
    {30,  26,  24,  255},    
    {56,  32,  21,  255},    
    {85,  38,  23,  255},    
    {112, 64,  36,  255},   
    {114, 114, 120, 255}, 
    {73,  54,  26,  255},    
    {122, 89,  40,  255},   
    {193, 159, 100, 255},
    // clang-format on
};

static const GXColor cGlassColor[RFLi_MAX_GLASS_COLOR + 1] = {
    // clang-format off
    {16,  16, 16, 255},  
    {96,  56, 16, 255},  
    {152, 24, 16, 255}, 
    {32,  48, 96, 255},  
    {144, 88, 0,  255},  
    {96,  88, 80, 255},
    // clang-format on
};

static const GXColor cFavoriteColor[RFLFavoriteColor_Max] = {
    // clang-format off
    {184, 64,  48,  255}, //!< RFLFavoriteColor_Red
    {240, 120, 40,  255}, //!< RFLFavoriteColor_Orange
    {248, 216, 32,  255}, //!< RFLFavoriteColor_Yellow
    {128, 200, 40,  255}, //!< RFLFavoriteColor_YellowGreen
    {0,   116, 40,  255}, //!< RFLFavoriteColor_Green
    {32,  72,  152, 255}, //!< RFLFavoriteColor_Blue
    {64,  160, 216, 255}, //!< RFLFavoriteColor_SkyBlue
    {232, 96,  120, 255}, //!< RFLFavoriteColor_Pink
    {112, 44,  168, 255}, //!< RFLFavoriteColor_Purple
    {72,  56,  24,  255}, //!< RFLFavoriteColor_Brown
    {224, 224, 224, 255}, //!< RFLFavoriteColor_White
    {24,  24,  20,  255}, //!< RFLFavoriteColor_Black
    // clang-format on
};

static const GXColor cWhite = {255, 255, 255, 255};

static void RFLSetCoordinate(RFLCoordinateType u, RFLCoordinateType f) {
    RFLCoordinateType r;

    union {
        RFLCoordinateType c;
        u8 b[4];
    } uu;
    union {
        RFLCoordinateType c;
        u8 b[4];
    } uf;
    union {
        RFLCoordinateType c;
        u8 b[4];
    } ur;

    uu.c = u;
    uf.c = f;

    ur.b[0] = (uu.b[1] * uf.b[2]) - (uu.b[2] * uf.b[1]);
    ur.b[1] = (uu.b[2] * uf.b[0]) - (uu.b[0] * uf.b[2]);
    ur.b[2] = (uu.b[0] * uf.b[1]) - (uu.b[1] * uf.b[0]);

    r = ur.c;

    if (u & RFLCoordinateType_X) {
        coordinateData.uOff = 0; // U -> X
    } else if (u & RFLCoordinateType_Y) {
        coordinateData.uOff = 1; // U -> Y
    } else {
        coordinateData.uOff = 2; // U -> Z
    }

    if (f & RFLCoordinateType_X) {
        coordinateData.fOff = 0; // F -> X
    } else if (f & RFLCoordinateType_Y) {
        coordinateData.fOff = 1; // F -> Y
    } else {
        coordinateData.fOff = 2; // F -> Z
    }

    if (r & RFLCoordinateType_X) {
        coordinateData.rOff = 0; // R -> X
    } else if (r & RFLCoordinateType_Y) {
        coordinateData.rOff = 1; // R -> Y
    } else {
        coordinateData.rOff = 2; // R -> Z
    }

    coordinateData.uRev = (u & RFLCoordinateType_RevMask) != 0;
    coordinateData.fRev = (f & RFLCoordinateType_RevMask) != 0;
    coordinateData.rRev = (r & RFLCoordinateType_RevMask) != 0;
}

static u32 RFLiGetExpressionNum(u32 exprFlags) {
    int i;
    u32 num = 0;

    for (i = 0; i < RFLExp_Max; i++) {
        if (exprFlags & (1 << i)) {
            num++;
        }
    }

    return num;
}

u32 RFLGetModelBufferSize(RFLResolution res, u32 exprFlags) {
    const u32 exprNum = RFLiGetExpressionNum(exprFlags);
    const s32 texSize = RFLiGetMaskBufSize(res);

    return ROUND_UP(exprNum * sizeof(GXTexObj), 32) +
           ROUND_UP(sizeof(RFLiCharModelRes), 32) +
           ROUND_UP(texSize * exprNum, 32);
}

RFLErrcode RFLInitCharModel(RFLCharModel* model, RFLDataSource src,
                            RFLMiddleDB* db, u16 id, void* work,
                            RFLResolution res, u32 exprFlags) {
    RFLiCharInfo info;

    RFLErrcode err = RFLiPickupCharInfo(&info, src, db, id);
    if (err == RFLErrcode_Success) {
        RFLiInitCharModel(model, &info, work, res, exprFlags);
    }

    return err;
}

void RFLiInitCharModel(RFLCharModel* model, RFLiCharInfo* info, void* work,
                       RFLResolution res, u32 exprFlags) {
    RFLiCharModel* imodel;
    u32 maskSize;
    u8* maskImages[RFLExp_Max];
    u32 maskRes;
    GXTexObj* exprTexObj;
    u8* image;
    BOOL setExpr;
    f32 maxLod;
    u8 mipmap;
    int i;

    imodel = (RFLiCharModel*)model;

    imodel->resolution = res;
    maskSize = RFLiGetMaskBufSize(res);
    maskRes = RFLiGetMaxMaskRsl(res);
    imodel->res = work;

    // Expression texobjs
    exprTexObj =
        (GXTexObj*)ROUND_UP_PTR((u8*)work + sizeof(RFLiCharModelRes), 32);
    for (i = 0; i < RFLExp_Max; i++) {
        if (exprFlags & (1 << i)) {
            imodel->maskTexObj[i] = exprTexObj;
            exprTexObj++;
        } else {
            imodel->maskTexObj[i] = NULL;
        }
    }

    // Expression images
    image = (u8*)ROUND_UP_PTR(exprTexObj, 32);
    for (i = 0; i < RFLExp_Max; i++) {
        if (exprFlags & (1 << i)) {
            maskImages[i] = image;
            image = (u8*)image + maskSize;
        } else {
            maskImages[i] = NULL;
        }
    }

    RFLiInitCharModelRes(imodel->res, info);

    setExpr = FALSE;
    mipmap = FALSE;
    maxLod = 0.0f;

    switch (res) {
    case RFLResolution_64M:
        maxLod = 1.0f;
        mipmap = TRUE;
        break;
    case RFLResolution_128M:
        maxLod = 2.0f;
        mipmap = TRUE;
        break;
    case RFLResolution_256M:
        maxLod = 3.0f;
        mipmap = TRUE;
        break;
    }

    for (i = 0; i < RFLExp_Max; i++) {
        if (imodel->maskTexObj[i] != NULL) {
            GXInitTexObj(imodel->maskTexObj[i], maskImages[i], maskRes, maskRes,
                         GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, mipmap);

            if (mipmap) {
                GXInitTexObjLOD(imodel->maskTexObj[i], GX_LIN_MIP_LIN,
                                GX_LINEAR, 0.0f, maxLod, 0.0f, FALSE, FALSE,
                                GX_ANISO_1);
            } else {
                GXInitTexObjLOD(imodel->maskTexObj[i], GX_LINEAR, GX_LINEAR,
                                0.0f, 0.0f, 0.0f, FALSE, FALSE, GX_ANISO_1);
            }

            if (!setExpr) {
                imodel->expression = (RFLExpression)i;
                setExpr = TRUE;
            }
        }
    }

    RFLiMakeTexture(info, maskImages, res);
}

void RFLSetMtx(RFLCharModel* model, const Mtx mvMtx) {
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    PSMTXCopy(mvMtx, imodel->posMtx);
    PSMTXInvXpose(mvMtx, imodel->nrmMtx);
}

void RFLSetExpression(RFLCharModel* model, RFLExpression expr) {
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    imodel->expression = expr;
}

static RFLExpression RFLGetExpression(const RFLCharModel* model) {
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    return imodel->expression;
}

GXColor RFLGetFavoriteColor(RFLFavoriteColor color) {
    return cFavoriteColor[color];
}

GXColor RFLiGetFacelineColor(const RFLiCharInfo* info) {
    int color = 0;

    if (info->faceline.color < 6) {
        color = info->faceline.color;
    }

    return cFacelineColor[color];
}

void RFLLoadDrawSetting(const RFLDrawSetting* setting) {
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_NEVER, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    GXSetZCompLoc(setting->compLoc);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(TRUE);
    GXSetDither(FALSE);
    GXSetDstAlpha(FALSE, 0);

    if (setting->lightEnable) {
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                      GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC,
                        GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
                        GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                        GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
                        GX_TEVPREV);

        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting2Tev);
        RFLLoadVertexSetting(&cDefaultDrawCoreSetting2Tev);
        GXSetNumChans(1);

        GXSetChanCtrl(GX_COLOR0, TRUE, GX_SRC_REG, GX_SRC_REG,
                      setting->lightMask, setting->diffuse, setting->attn);
        GXSetChanCtrl(GX_ALPHA0, FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                      GX_DF_NONE, GX_AF_NONE);
        GXSetChanAmbColor(GX_COLOR0, setting->ambColor);
        GXSetChanMatColor(GX_COLOR0, cWhite);
    } else {
        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting1Tev);
        RFLLoadVertexSetting(&cDefaultDrawCoreSetting1Tev);
        GXSetNumChans(0);
    }
}

void RFLDrawOpa(const RFLCharModel* model) {
    RFLDrawOpaCore(model, &cDefaultDrawCoreSetting2Tev);
}

void RFLDrawXlu(const RFLCharModel* model) {
    RFLDrawXluCore(model, &cDefaultDrawCoreSetting2Tev);
}

void RFLLoadVertexSetting(const RFLDrawCoreSetting* setting) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_NRM, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);
    GXSetVtxAttrFmt(0, GX_VA_POS, 1, 3, 8);
    GXSetVtxAttrFmt(0, GX_VA_NRM, 0, 3, 14);
    GXSetVtxAttrFmt(0, GX_VA_TEX0, 1, 3, 13);
    GXSetNumTexGens(setting->txcGenNum);
}

void RFLLoadMaterialSetting(const RFLDrawCoreSetting* setting) {
    GXSetTevSwapModeTable(setting->tevSwapTable, GX_CH_RED, GX_CH_GREEN,
                          GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(setting->tevSwapTable + 1, GX_CH_RED, GX_CH_ALPHA,
                          GX_CH_BLUE, GX_CH_GREEN);
    GXSetNumTevStages(setting->tevStageNum);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
                    setting->tevOutRegID);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
                    setting->tevOutRegID);
    GXSetTevKColorSel(GX_TEVSTAGE0,
                      (GXTevKColorSel)(setting->tevKColorID + GX_TEV_KCSEL_K0));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_8_8);
}

void RFLDrawOpaCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting) {
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    RFLiCharModelRes* res = imodel->res;

    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_KONST);

    // @bug Copy-paste error
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);

    GXSetCullMode(setting->reverseCulling ? GX_CULL_FRONT : GX_CULL_BACK);

    GXLoadPosMtxImm(imodel->posMtx, setting->posNrmMtxID);
    GXLoadNrmMtxImm(imodel->nrmMtx, setting->posNrmMtxID);
    GXSetCurrentMtx(setting->posNrmMtxID);

    GXSetTexCoordGen2(setting->txcID, GX_TG_MTX2x4, GX_TG_POS, 60, 0, 0x7D);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_PNMTXIDX);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                  GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_KONST);

    if (res->beardDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cBeardColor[res->beardColor]);
        GXSetArray(GX_VA_POS, res->beardVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->beardVtxNrm, 6);
        GXCallDisplayList(res->beardDl, res->beardDlSize);
    }

    GXSetTevKColor(setting->tevKColorID, cFacelineColor[res->facelineColor]);

    if (res->noseDlSize > 0) {
        GXSetArray(GX_VA_POS, res->noseVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noseVtxNrm, 6);
        GXCallDisplayList(res->noseDl, res->noseDlSize);
    }

    if (res->flipHair) {
        GXSetCullMode(setting->reverseCulling ? GX_CULL_BACK : GX_CULL_FRONT);
    }

    if (res->foreheadDlSize > 0) {
        GXSetArray(GX_VA_POS, res->foreheadVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->foreheadVtxNrm, 6);
        GXCallDisplayList(res->foreheadDl, res->foreheadDlSize);
    }

    if (res->hairDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cHairColor[res->hairColor]);
        GXSetArray(GX_VA_POS, res->hairVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->hairVtxNrm, 6);
        GXCallDisplayList(res->hairDl, res->hairDlSize);
    }

    GXSetTevOrder(GX_TEVSTAGE0, setting->txcID, setting->texMapID,
                  GX_COLOR_NULL);
    GXSetTexCoordGen2(setting->txcID, GX_TG_MTX2x4, GX_TG_TEX0, 60, 0, 0x7D);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);

    if (res->capDlSize > 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC,
                        GX_CC_KONST);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, 1,
                        setting->tevOutRegID);
        GXSetTevKColor(setting->tevKColorID,
                       cFavoriteColor[res->favoriteColor]);

        GXLoadTexObj(&res->capTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->capVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->capVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->capVtxTxc, 4);
        GXCallDisplayList(res->capDl, res->capDlSize);

        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
                        setting->tevOutRegID);
    }

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_TEXC, GX_CC_TEXA,
                    GX_CC_ZERO);
    GXSetTevKColor(setting->tevKColorID, cFacelineColor[res->facelineColor]);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     (GXTevSwapSel)(setting->tevSwapTable + 1));

    if (res->flipHair) {
        GXSetCullMode(setting->reverseCulling ? GX_CULL_FRONT : GX_CULL_BACK);
    }

    GXLoadTexObj(&res->faceTexObj, setting->texMapID);
    GXSetArray(GX_VA_POS, res->faceVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->faceVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->faceVtxTxc, 4);
    GXCallDisplayList(res->faceDl, res->faceDlSize);
}

void RFLDrawXluCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting) {
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    RFLiCharModelRes* res = imodel->res;

    GXSetTevOrder(GX_TEVSTAGE0, setting->txcID, setting->texMapID,
                  GX_COLOR_NULL);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);

    GXLoadPosMtxImm(imodel->posMtx, setting->posNrmMtxID);
    GXLoadNrmMtxImm(imodel->nrmMtx, setting->posNrmMtxID);
    GXSetCurrentMtx(setting->posNrmMtxID);

    GXSetTexCoordGen2(setting->txcID, GX_TG_MTX2x4, GX_TG_TEX0, 60, 0 , 0x7D);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_TEXC);
    GXSetCullMode(setting->reverseCulling ? GX_CULL_FRONT : GX_CULL_BACK);

    GXLoadTexObj(imodel->maskTexObj[imodel->expression], setting->texMapID);
    GXSetArray(GX_VA_POS, res->maskVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->maskVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->maskVtxTxc, 4);
    GXCallDisplayList(res->maskDl, res->maskDlSize);

    if (res->noselineDlSize > 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                        GX_CC_ZERO);
        GXLoadTexObj(&res->noseTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->noselineVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noselineVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->noselineVtxTxc, 4);
        GXCallDisplayList(res->noselineDl, res->noselineDlSize);
    }

    if (res->glassesDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cGlassColor[res->glassesColor]);
        GXSetCullMode(GX_CULL_NONE);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC,
                        GX_CC_ZERO);
        GXLoadTexObj(&res->glassesTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->glassesVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->glassesVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->glassesVtxTxc, 4);
        GXCallDisplayList(res->glassesDl, res->glassesDlSize);
    }
}

void RFLiInitCharModelRes(RFLiCharModelRes* res, const RFLiCharInfo* info) {
    Vec noseTrans;
    Vec beardTrans;
    Vec hairTrans;

    GXSetMisc(1, 0);
    GXSetMisc(2, 1);

    /**
     * Faceline shape
     */
    {
        RFLiShapeRes arg;
        arg.part = RFLiPartsShp_Faceline;
        arg.file = info->faceline.type;
        arg.vtxPosBuf = res->faceVtxPos;
        arg.vtxNrmBuf = res->faceVtxNrm;
        arg.vtxTxcBuf = res->faceVtxTxc;
        arg.dlBuf = res->faceDl;
        arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->faceVtxPos));
        arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->faceVtxNrm));
        arg.vtxTxcBufSize = NUM_VTX_TXC(sizeof(res->faceVtxTxc));
        arg.dlBufSize = sizeof(res->faceDl);
        arg.noseTrans = &noseTrans;
        arg.beardTrans = &beardTrans;
        arg.hairTrans = &hairTrans;
        arg.flipX = FALSE;
        arg.transform = FALSE;
        RFLiInitShapeRes(&arg);

        res->faceDlSize = arg.dlSize;
    }

    /**
     * Cap shape
     */
    {
        RFLiShapeRes arg;
        arg.part = RFLiPartsShp_Cap;
        arg.file = info->hair.type;
        arg.vtxPosBuf = res->capVtxPos;
        arg.vtxNrmBuf = res->capVtxNrm;
        arg.vtxTxcBuf = res->capVtxTxc;
        arg.dlBuf = res->capDl;
        arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->capVtxPos));
        arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->capVtxNrm));
        arg.vtxTxcBufSize = NUM_VTX_TXC(sizeof(res->capVtxTxc));
        arg.dlBufSize = sizeof(res->capDl);
        arg.flipX = info->hair.flip;
        arg.transform = TRUE;
        arg.posScale = 1.0f;
        arg.posTrans = &hairTrans;
        RFLiInitShapeRes(&arg);

        res->capDlSize = arg.dlSize;

        res->hairVtxPos = (s16*)((u8*)res->capVtxPos +
                                 (arg.numVtxPos * 4 - arg.numVtxPos) * 2);
        res->hairVtxNrm = (s16*)((u8*)res->capVtxNrm +
                                 (arg.numVtxNrm * 4 - arg.numVtxNrm) * 2);

        res->hairDl = res->noseDl + ROUND_UP(arg.dlSize, 32) +
                      offsetof(RFLiCharModelRes, capDl);
    }

    /**
     * Hair shape
     */
    {
        RFLiShapeRes arg;
        int temp = 4;

        arg.part = RFLiPartsShp_Hair;
        arg.file = info->hair.type;
        arg.vtxPosBuf = res->hairVtxPos;
        arg.vtxNrmBuf = res->hairVtxNrm;
        arg.dlBuf = res->hairDl;

        arg.vtxPosBufSize =
            NUM_VTX_POS(sizeof(res->capVtxPos)) -
            (((uintptr_t)res->hairVtxPos - (uintptr_t)res->capVtxPos) /
             VTX_COORD_SIZE) /
                VTX_COORDS_IN_POS;

        arg.vtxNrmBufSize =
            NUM_VTX_POS(sizeof(res->capVtxNrm)) -
            (((uintptr_t)res->hairVtxNrm - (uintptr_t)res->capVtxNrm) /
             VTX_COORD_SIZE) /
                VTX_COORDS_IN_NRM;

        arg.dlBufSize = sizeof(res->capDl) -
                        ((uintptr_t)res->hairDl - (uintptr_t)res->capDl);

        arg.flipX = info->hair.flip;
        arg.transform = TRUE;
        arg.posScale = 1.0f;
        arg.posTrans = &hairTrans;
        RFLiInitShapeRes(&arg);

        res->hairDlSize = arg.dlSize;

        res->foreheadVtxPos =
            (s16*)((u8*)res->hairVtxPos +
                   (arg.numVtxPos * temp - arg.numVtxPos) * 2);

        res->foreheadVtxNrm =
            (s16*)((u8*)res->hairVtxNrm +
                   (arg.numVtxNrm * temp - arg.numVtxNrm) * 2);

        res->foreheadDl = res->hairDl + ROUND_UP(arg.dlSize, 32);

        res->flipHair = info->hair.flip;
    }

    /**
     * Forehead shape
     */
    {
        RFLiShapeRes arg;
        arg.part = RFLiPartsShp_Forehead;
        arg.file = info->hair.type;
        arg.vtxPosBuf = res->foreheadVtxPos;
        arg.vtxNrmBuf = res->foreheadVtxNrm;
        arg.dlBuf = res->foreheadDl;

        arg.vtxPosBufSize =
            NUM_VTX_POS(sizeof(res->capVtxPos)) -
            (((uintptr_t)res->foreheadVtxPos - (uintptr_t)res->capVtxPos) /
             VTX_COORD_SIZE) /
                VTX_COORDS_IN_POS;

        arg.vtxNrmBufSize =
            NUM_VTX_POS(sizeof(res->capVtxNrm)) -
            (((uintptr_t)res->foreheadVtxNrm - (uintptr_t)res->capVtxNrm) /
             VTX_COORD_SIZE) /
                VTX_COORDS_IN_NRM;

        arg.dlBufSize = sizeof(res->capDl) -
                        ((uintptr_t)res->foreheadDl - (uintptr_t)res->capDl);

        arg.flipX = info->hair.flip;
        arg.transform = TRUE;
        arg.posScale = 1.0f;
        arg.posTrans = &hairTrans;
        RFLiInitShapeRes(&arg);

        res->foreheadDlSize = arg.dlSize;
    }

    /**
     * Beard shape
     */
    {
        RFLiShapeRes arg;
        arg.part = RFLiPartsShp_Beard;
        arg.file = info->beard.type;
        arg.vtxPosBuf = res->beardVtxPos;
        arg.vtxNrmBuf = res->beardVtxNrm;
        arg.dlBuf = res->beardDl;
        arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->beardVtxPos));
        arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->beardVtxNrm));
        arg.dlBufSize = sizeof(res->beardDl);
        arg.flipX = FALSE;
        arg.transform = TRUE;
        arg.posScale = 1.0f;
        arg.posTrans = &beardTrans;
        RFLiInitShapeRes(&arg);

        res->beardDlSize = arg.dlSize;
    }

    {
        f32 scale;
        Vec trans;

        /**
         * Nose shape
         */
        {
            RFLiShapeRes arg;

            scale = 0.4f + 0.175f * info->nose.scale;

            trans.x = noseTrans.x;
            trans.y = +noseTrans.y + -1.5f * (info->nose.y - 8);
            trans.z = noseTrans.z;

            arg.part = RFLiPartsShp_Nose;
            arg.file = info->nose.type;
            arg.vtxPosBuf = res->noseVtxPos;
            arg.vtxNrmBuf = res->noseVtxNrm;
            arg.dlBuf = res->noseDl;
            arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->noseVtxPos));
            arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->noseVtxNrm));
            arg.dlBufSize = sizeof(res->noseDl);
            arg.flipX = FALSE;
            arg.transform = TRUE;
            arg.posScale = scale;
            arg.posTrans = &trans;
            RFLiInitShapeRes(&arg);

            res->noseDlSize = arg.dlSize;
        }

        /**
         * Noseline shape
         */
        {
            RFLiShapeRes arg;

            arg.part = RFLiPartsShp_Noseline;
            arg.file = info->nose.type;
            arg.vtxPosBuf = res->noselineVtxPos;
            arg.vtxNrmBuf = res->noselineVtxNrm;
            arg.vtxTxcBuf = res->noselineVtxTxc;
            arg.dlBuf = res->noselineDl;
            arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->noselineVtxPos));
            arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->noselineVtxNrm));
            arg.vtxTxcBufSize = NUM_VTX_TXC(sizeof(res->noselineVtxTxc));
            arg.dlBufSize = sizeof(res->noselineDl);
            arg.flipX = FALSE;
            arg.transform = TRUE;
            arg.posScale = scale;
            arg.posTrans = &trans;
            RFLiInitShapeRes(&arg);

            res->noselineDlSize = arg.dlSize;
        }
    }

    /**
     * Mask shape
     */
    {
        RFLiShapeRes arg;
        arg.part = RFLiPartsShp_Mask;
        arg.file = info->faceline.type;
        arg.vtxPosBuf = res->maskVtxPos;
        arg.vtxNrmBuf = res->maskVtxNrm;
        arg.vtxTxcBuf = res->maskVtxTxc;
        arg.dlBuf = res->maskDl;
        arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->maskVtxPos));
        arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->maskVtxNrm));
        arg.vtxTxcBufSize = NUM_VTX_TXC(sizeof(res->maskVtxTxc));
        arg.dlBufSize = sizeof(res->maskDl);
        arg.flipX = FALSE;
        arg.transform = FALSE;
        RFLiInitShapeRes(&arg);

        res->maskDlSize = arg.dlSize;
    }

    /**
     * Glasses shape
     */
    {
        RFLiShapeRes arg;
        f32 scale;
        Vec trans;

        scale = 0.15f * info->glass.scale + 0.4f;

        trans.x = noseTrans.x;
        trans.y = 5.0f + noseTrans.y + -1.5f * (info->glass.y - 11);
        trans.z = 2.0f + noseTrans.z;

        arg.part = RFLiPartsShp_Glass;
        arg.file = 0;
        arg.vtxPosBuf = res->glassesVtxPos;
        arg.vtxNrmBuf = res->glassesVtxNrm;
        arg.vtxTxcBuf = res->glassesVtxTxc;
        arg.dlBuf = res->glassesDl;
        arg.vtxPosBufSize = NUM_VTX_POS(sizeof(res->glassesVtxPos));
        arg.vtxNrmBufSize = NUM_VTX_NRM(sizeof(res->glassesVtxNrm));
        arg.vtxTxcBufSize = NUM_VTX_TXC(sizeof(res->glassesVtxTxc));
        arg.dlBufSize = sizeof(res->glassesDl);
        arg.flipX = FALSE;
        arg.transform = TRUE;
        arg.posScale = scale;
        arg.posTrans = &trans;
        RFLiInitShapeRes(&arg);

        res->glassesDlSize = arg.dlSize;
    }

    RFLiInitTexRes(&res->faceTexObj, RFLiPartsShpTex_Face,
                   info->faceline.texture, res->faceTex);

    if (res->capDlSize > 0) {
        RFLiInitTexRes(&res->capTexObj, RFLiPartsShpTex_Cap, info->hair.type,
                       res->capTex);
    }

    if (res->noselineDlSize > 0) {
        RFLiInitTexRes(&res->noseTexObj, RFLiPartsShpTex_Noseline,
                       info->nose.type, res->noseTex);
    }

    RFLiInitTexRes(&res->glassesTexObj, RFLiPartsShpTex_Glass, info->glass.type,
                   res->glassesTex);

    res->facelineColor = info->faceline.color;
    res->hairColor = info->hair.color;
    res->beardColor = info->beard.color;
    res->glassesColor = info->glass.color;
    res->favoriteColor = info->personal.color;

    DCFlushRange(res, sizeof(RFLiCharModelRes));
}

void RFLiInitShapeRes(RFLiShapeRes* shape) {
    static const u32 csHeader[RFLiPartsShp_Max] = {
        'nose', 'frhd', 'face', 'hair', 'cap_', 'berd', 'nsln', 'mask', 'glas'};

    void* res;
    u8* ptr8;

    // Not all shapes have texture coordinates
    BOOL skipTxc = shape->part == RFLiPartsShp_Forehead ||
                   shape->part == RFLiPartsShp_Hair ||
                   shape->part == RFLiPartsShp_Beard ||
                   shape->part == RFLiPartsShp_Nose;

    u32 fileSize = RFLiGetShapeSize(shape->part, shape->file);
    res = RFLiAlloc32(fileSize);
    RFLiLoadShape(shape->part, shape->file, res);

    ptr8 = (u8*)res;
    ptr8 += sizeof(u32);

    // Faceline extra data
    if (shape->part == RFLiPartsShp_Faceline) {
        memcpy(shape->noseTrans, ptr8, sizeof(Vec));
        ptr8 += sizeof(Vec);
        memcpy(shape->beardTrans, ptr8, sizeof(Vec));
        ptr8 += sizeof(Vec);
        memcpy(shape->hairTrans, ptr8, sizeof(Vec));
        ptr8 += sizeof(Vec);
    }

    /**
     * Vertex positions
     */

    if (*(u16*)ptr8 == 0) {
        shape->numVtxPos = 0;
        shape->numVtxNrm = 0;
        shape->numVtxTxc = 0;
        shape->dlSize = 0;
        RFLiFree(res);
        return;
    }

    shape->numVtxPos = *(u16*)ptr8;
    ptr8 += sizeof(u16);

    {
        u32 byteSize = SIZE_VTX_POS(shape->numVtxPos);
        s16* ptr16 = (s16*)ptr8;
        int i;

        if (shape->transform) {
            s32 s = 256.0f * shape->posScale;
            s32 tx = 256.0f * shape->posTrans->x;
            s32 ty = 256.0f * shape->posTrans->y;
            s32 tz = 256.0f * shape->posTrans->z;

            for (i = 0; i < shape->numVtxPos; i++) {
                s16 temp[3];

                if (shape->flipX) {
                    temp[0] = tx + ((-ptr16[0] * s) >> 8);
                } else {
                    temp[0] = tx + ((ptr16[0] * s) >> 8);
                }

                temp[1] = ty + ((ptr16[1] * s) >> 8);
                temp[2] = tz + ((ptr16[2] * s) >> 8);

                RFLiTransformCoordinate(
                    &shape->vtxPosBuf[i * VTX_COORDS_IN_POS], temp);
                ptr16 += VTX_COORDS_IN_POS;
            }

        } else if (shape->flipX != 0) {
            for (i = 0; i < shape->numVtxPos; i++) {
                s16 temp[3];

                temp[0] = -ptr16[0];
                temp[1] = ptr16[1];
                temp[2] = ptr16[2];

                RFLiTransformCoordinate(
                    &shape->vtxPosBuf[i * VTX_COORDS_IN_POS], temp);
                ptr16 += VTX_COORDS_IN_POS;
            }
        } else {
            for (i = 0; i < shape->numVtxPos; i++) {
                RFLiTransformCoordinate(
                    &shape->vtxPosBuf[i * VTX_COORDS_IN_POS], ptr16);
                ptr16 += VTX_COORDS_IN_POS;
            }
        }

        ptr8 += byteSize;
    }

    /**
     * Vertex normals
     */

    shape->numVtxNrm = *(u16*)ptr8;
    ptr8 += sizeof(u16);

    {
        s16* ptr16 = (s16*)ptr8;
        u32 byteSize = SIZE_VTX_NRM(shape->numVtxNrm);

        if (shape->flipX != 0) {
            int i;
            for (i = 0; i < shape->numVtxNrm; i++) {
                s16 temp[3];

                temp[0] = -ptr16[0];
                temp[1] = ptr16[1];
                temp[2] = ptr16[2];

                RFLiTransformCoordinate(
                    &shape->vtxNrmBuf[i * VTX_COORDS_IN_NRM], temp);
                ptr16 += VTX_COORDS_IN_NRM;
            }
        } else {
            int i;
            for (i = 0; i < shape->numVtxNrm; i++) {
                RFLiTransformCoordinate(
                    &shape->vtxNrmBuf[i * VTX_COORDS_IN_NRM], ptr16);
                ptr16 += VTX_COORDS_IN_NRM;
            }
        }

        ptr8 += byteSize;
    }

    /**
     * Vertex texcoords
     */

    {
        u32 byteSize;

        if (skipTxc) {
            shape->numVtxTxc = 0;
        } else {
            shape->numVtxTxc = *(u16*)ptr8;
            ptr8 += sizeof(u16);

            byteSize = SIZE_VTX_TXC(shape->numVtxTxc);
            memcpy(shape->vtxTxcBuf, ptr8, byteSize);
            ptr8 += byteSize;
        }
    }

    /**
     * Display list
     */

    {
        int i;
        int j;
        s32 primitiveNum = *ptr8++;

        DCInvalidateRange(shape->dlBuf, shape->dlBufSize);
        GXBeginDisplayList(shape->dlBuf, shape->dlBufSize);

        for (i = 0; i < primitiveNum; i++) {
            u16 vtxNum = *ptr8++;
            GXPrimitive prim = *ptr8++;

            GXBegin(prim, 0, vtxNum);
            {
                for (j = 0; j < vtxNum; j++) {
                    GXPosition1x8(*ptr8++);
                    GXNormal1x8(*ptr8++);

                    if (!skipTxc) {
                        GXTexCoord1x8(*ptr8++);
                    }
                }
            }
            GXEnd();
        }
    }

    shape->dlSize = GXEndDisplayList();
    RFLiFree(res);
}

void RFLiInitTexRes(GXTexObj* texObj, RFLiPartsShpTex part, u16 file,
                    void* buffer) {
    u32 texSize;
    RFLiTexture* tex;

    tex = (RFLiTexture*)RFLiAlloc32(RFLiGetShpTexSize(part, file));
    RFLiLoadShpTexture(part, file, tex);

    switch (part) {
    case RFLiPartsShpTex_Face:
        texSize = tex->height * tex->width * 2;
        break;
    case RFLiPartsShpTex_Cap:
    case RFLiPartsShpTex_Noseline:
        texSize = tex->height * tex->width / 2;
        break;
    case RFLiPartsShpTex_Glass:
        texSize = tex->height * tex->width;
        break;
    }

    memcpy(buffer, RFLiGetTexImage(tex), texSize);
    GXInitTexObj(texObj, buffer, tex->width, tex->height, tex->format,
                 tex->wrapS, tex->wrapT, FALSE);
    GXInitTexObjLOD(texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, FALSE,
                    FALSE, GX_ANISO_1);
    RFLiFree(tex);
}

static void RFLiTransformCoordinate(s16* to, const s16* from) {
    to[coordinateData.rOff] = coordinateData.rRev ? -from[0] : from[0];
    to[coordinateData.uOff] = coordinateData.uRev ? -from[1] : from[1];
    to[coordinateData.fOff] = coordinateData.fRev ? -from[2] : from[2];
}

static void RFLDrawShape(const RFLCharModel* model) {
    GXCullMode cullMode;
    RFLiCharModel* imodel = (RFLiCharModel*)model;
    RFLiCharModelRes* res = imodel->res;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_NRM, GX_VA_TEX1MTXIDX);
    GXSetVtxAttrFmt(0, GX_VA_POS, 1, 3, 8);
    GXSetVtxAttrFmt(0, GX_VA_NRM, 0, 3, 14);
    GXSetVtxAttrFmt(0, GX_VA_TEX0, 1, 3, 13);

    GXLoadPosMtxImm(imodel->posMtx, 0);
    GXLoadNrmMtxImm(imodel->nrmMtx, 0);
    GXSetCurrentMtx(0);

    if (res->beardDlSize > 0) {
        GXSetArray(GX_VA_POS, res->beardVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->beardVtxNrm, 6);
        GXCallDisplayList(res->beardDl, res->beardDlSize);
    }

    if (res->noseDlSize > 0) {
        GXSetArray(GX_VA_POS, res->noseVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noseVtxNrm, 6);
        GXCallDisplayList(res->noseDl, res->noseDlSize);
    }

    if (res->flipHair) {
        GXGetCullMode(&cullMode);
        switch (cullMode) {
        case GX_CULL_BACK:
            GXSetCullMode(GX_CULL_FRONT);
            break;
        case GX_CULL_FRONT:
            GXSetCullMode(GX_CULL_BACK);
            break;
        }
    }

    if (res->foreheadDlSize > 0) {
        GXSetArray(GX_VA_POS, res->foreheadVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->foreheadVtxNrm, 6);
        GXCallDisplayList(res->foreheadDl, res->foreheadDlSize);
    }

    if (res->hairDlSize > 0) {
        GXSetArray(GX_VA_POS, res->hairVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->hairVtxNrm, 6);
        GXCallDisplayList(res->hairDl, res->hairDlSize);
    }

    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);
    if (res->capDlSize > 0) {
        GXSetArray(GX_VA_POS, res->capVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->capVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->capVtxTxc, 4);
        GXCallDisplayList(res->capDl, res->capDlSize);
    }

    if (res->flipHair) {
        switch (cullMode) {
        case GX_CULL_BACK:
            GXSetCullMode(GX_CULL_BACK);
            break;
        case GX_CULL_FRONT:
            GXSetCullMode(GX_CULL_FRONT);
            break;
        }
    }

    GXSetArray(GX_VA_POS, res->faceVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->faceVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->faceVtxTxc, 4);
    GXCallDisplayList(res->faceDl, res->faceDlSize);
}

void RFLSetModelDrawDoneCallback(RFLCallback cb) {
    RFLiGetManager()->modelDrawCb = cb;
}
