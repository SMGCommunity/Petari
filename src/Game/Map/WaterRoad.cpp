#include "Game/Map/WaterRoad.hpp"
#include "Game/Util.hpp"

void WaterRoadModelInfo::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts scheduler(false);

    s32 val = 0x18;

    if (mIsLow) {
        val = 0x10;
    }

    mList = new u8[0x20];
    //DCInvalidateRange(mList, )
}

void WaterRoadModelInfo::drawGD() const {
    if (mIsLow) {
        loadMaterialLow();
    }
    else {
        loadMaterialHigh(0);
    }

    GXCallDisplayList(mList, mDLLength);
}

s32 WaterRoadModelInfo::calcDemoDrawPointNum(const WaterRoad *pRoad) const {
    s32 ret = 1;

    for (; ret < _8; ret++) {
        if (MR::getRailCoord(pRoad) < _18[ret]) {
            return ret;
        }
    }

    return _8;
}

void WaterRoadModelInfo::loadMaterialHigh(const WaterRoad *pRoad) const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_S16, 0x10);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
    GXSetArray(GX_VA_POS, _10, 0xC);
    GXSetArray(GX_VA_NRM, _14, 6);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXLoadNrmMtxImm(MR::getCameraViewMtx(), 0); 
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D); 
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x24, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_POS, 0x27, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_NRM, 0x2A, 0, 0x7D);
    MR::loadTexProjectionMtx(0x27);

    TPos3f pos;
    pos.identity();
    pos.set(MR::getCameraViewMtx());
    pos.zeroTrans();

    TMtx34f mtx;
    mtx.identity();
    mtx.scale(1.0f);

    PSMTXConcat((const MtxPtr)&pos.mMtx, (const MtxPtr)&mtx, (MtxPtr)&pos.mMtx);
    PSMTXConcat((const MtxPtr)&pos.mMtx, qMtx2_1, (MtxPtr)&pos.mMtx);
    GXLoadTexMtxImm((const MtxPtr)&pos.mMtx, 0x2A, GX_MTX2x4);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP1);
    GXSetTevIndWarp(GX_TEVSTAGE3, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
 
    f32 offset[2][3];
    offset[0][1] = 0.0f;
    offset[0][0] = 0.2f;
    offset[0][2] = 0.0f;
    offset[1][0] = 0.0f; 
    offset[1][1] = 0.2f;
    offset[1][2] = 0.0f;

    GXSetIndTexMtx(GX_ITM_0, offset, 0);
    GXSetNumTevStages(5);

    const GXColor color_0 = { 0x28, 0x28, 0x28, 0x14 };
    GXSetTevColor(GX_TEVREG0, color_0);
    const GXColor color_1 = { 0x55, 0x96, 0xBE, 0xFF };
    GXSetTevColor(GX_TEVREG1, color_1);

    if (pRoad) {
        const GXColor color_2 = { 0xFF, 0xFF, 0xFF, pRoad->_9C };
        GXSetTevColor(GX_TEVREG2, color_2); 
    }

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_A0, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_COMP_R8_EQ, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP3, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE4, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE4, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

    if (pRoad && pRoad->_9C != 0xFF) {
        GXSetTevAlphaIn(GX_TEVSTAGE4, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    }
    else {
        GXSetTevAlphaIn(GX_TEVSTAGE4, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    }

    GXSetTevAlphaOp(GX_TEVSTAGE4, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

void WaterRoadModelInfo::loadMaterialLow() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_S16, 0x10);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetArray(GX_VA_POS, _10, 0xC);
    GXSetArray(GX_VA_NRM, _14, 6);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXLoadNrmMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX3x4, GX_TG_POS, 0x24, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_NRM, 0x27, 0, 0x7D);

    MR::loadTexProjectionMtx(0x24);

    TPos3f pos;
    pos.identity();
    pos.set(MR::getCameraViewMtx());
    pos.zeroTrans();

    TMtx34f mtx;
    mtx.identity();
    mtx.scale(1.0f);

    PSMTXConcat((const MtxPtr)&pos.mMtx, (const MtxPtr)&mtx, (MtxPtr)&pos.mMtx);
    PSMTXConcat((const MtxPtr)&pos.mMtx, qMtx2_1, (MtxPtr)&pos.mMtx);
    GXLoadTexMtxImm((const MtxPtr)&pos.mMtx, 0x27, GX_MTX2x4);
    GXSetNumIndStages(0);
    GXSetNumTevStages(4);
    const GXColor color_0 = { 0x28, 0x28, 0x28, 0x14 };
    GXSetTevColor(GX_TEVREG0, color_0);
    const GXColor color_1 = { 0x55, 0x96, 0xBE, 0xFF };
    GXSetTevColor(GX_TEVREG1, color_1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}