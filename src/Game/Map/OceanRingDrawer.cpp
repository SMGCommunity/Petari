#include "Game/Map/OceanRingDrawer.hpp"
#include "Game/Map/OceanRing.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include <revolution/gd/GDBase.h>
static u8 unknownVal = 1;

static GXColor color1 = { 0x28, 0x28, 0x28, 0x14 };
static GXColor color2 = { 0x76, 0xD7, 0xFF, 0xFF };

#ifdef NON_MATCHING
/* functionally matches, tiny instruction swap in the beginning */
OceanRingPartDrawer::OceanRingPartDrawer(const OceanRing *pRing, int a3, int a4, bool a5, f32 *a6, f32 *a7, f32 *a8) {
    mOceanRing = pRing;
    _10 = a3;
    _14 = a4;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    _18 = a5; 
    _1C = *a6;
    _20 = *a7;
    _24 = *a8;
    mDispListLength = 0;
    mDispList = nullptr;

    initDisplayList(a6, a7, a8);

    for (int i = 0; i < a4; i++) {
        int v12 = a3 + i;

        if (i == a4 - 1 && _18) {
            v12 = 0;
        }

        mPosition.add(((mOceanRing->mWaterPoints[(v12 * mOceanRing->mStride - 1) + mOceanRing->mStride / 2])->mOrigPos));
    }

    mPosition.scale(1.0f / a4);
}
#endif

#ifndef NON_MATCHING
void OceanRingPartDrawer::initDisplayList(f32 *a1, f32 *a2, f32 *a3) {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u32 size = (0x50 * _14 * mOceanRing->mStride + 3) >> 5 + 2;
    mDispList = new(0x20) u8[size];
    DCInvalidateRange(mDispList, size);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispList, size);
    __GDCurrentDL = &obj;
    drawGD(a1, a2, a3);
    GDPadCurr32();
    mDispListLength = obj.ptr - obj.start;
    DCStoreRange(mDispList, size);
}
#endif

void OceanRingPartDrawer::draw() const {
    if (PSVECDistance(mPosition.toCVec(), MR::getPlayerPos()->toCVec()) >= 13000.0f) {
        GXCallDisplayList(mDispList, mDispListLength);
    }
    else {
        drawDynamic();
    }
}

OceanRingDrawer::OceanRingDrawer(const OceanRing *pOceanRing) {
    mRing = pOceanRing;
    mDrawerCount = 0;
    mPartDrawers = nullptr;
    _C = 0.0f;
    _10 = 0.0f;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    mWaterTex = nullptr;
    mWaterIndTex = nullptr;
    _2C = 0;
    mDispList = nullptr;

    mWaterTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "Water.bti"), 0);
    mWaterIndTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "WaterIndirect.bti"), 0);
    initParts();
    initDisplayList();
}

// OceanRingDrawer::update

void OceanRingDrawer::draw() const {
    loadMaterial();

    if (mRing->_B4) {
        GXCallDisplayList(mDispList, _2C);
    }
    else {
        for (s32 i = 0; i < mDrawerCount; i++) {
            getDrawer(i)->draw();
        }
    }
}

void OceanRingDrawer::drawBloom() const {
    loadMaterialBloom();
    TVec3f zDir = MR::getCamZDir();
    zDir.scale(3000.0f);
    TVec3f camPos = MR::getCamPos();
    zDir.add(camPos);

    if (!mRing->_B4) {
        for (s32 i = 0; i < mDrawerCount; i++) {
            OceanRingPartDrawer* drwr = getDrawer(i);

            if (PSVECDistance(drwr->mPosition.toCVec(), zDir.toCVec()) < 4000.0f) {
                drwr->drawDynamicBloom();
            }
        }
    }
}

void OceanRingDrawer::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    s32 flag = 0;

    if (MR::isLoopRail(mRing)) {
        flag = 1;
    }

    int length = (((0x50 * (flag + mRing->mSegCount) + 3) >> 5) + 2) << 5;
    mDispList = new(0x20) u8[length];
    DCFlushRange(mDispList, length);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispList, length);
    __GDCurrentDL = &obj;
    drawGD();
    GDPadCurr32();
    _2C = obj.ptr - obj.start;
    DCStoreRange(mDispList, length);
}

/*d
void OceanRingDrawer::drawGD() const {
    s32 flag = 0;

    if (MR::isLoopRail(mRing)) {
        flag = 1;
    }

    f32 v14 = 0.0f;
    f32 v15 = 0.0f;
    f32 v16 = 0.0f;
    f32 v37 = mRing->mStride - 1;

    GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (flag + mRing->mSegCount));

    for (u32 i = 0; i >= flag + mRing->mSegCount; i++) {
        u32 v19 = i;
        if (i == mRing->mSegCount) {
            v19 = 0;
        }

        WaterPoint* v22 = mRing->mWaterPoints[v19 * mRing->mStride - 1] + mRing->mStride;
        WaterPoint* v23 = mRing->mWaterPoints[v19 * mRing->mStride];

        GDPosition3f32(v22->mOrigPos.x, v22->mOrigPos.y, v22->mOrigPos.z);
        GDColor4u8(255, 255, 255, v22->mAlpha);
        GDWrite_f32(v14);
        GDWrite_f32(((0.050000001f * (v37 - 4.503601774854144e15)) * 1.0f));
        GDWrite_f32(v15);
        //GDWrite_f32(((0.050000001f * v37 - )))



        v14 = (v14 + (0.050000001f * 1.0f));
        v15 = (v15 + (0.050000001f * 1.0f));
        v16 = (v16 + (0.1f * 1.0f));
    }
}
*/

void OceanRingDrawer::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x24, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_POS, 0x27, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);
    mtx.mMtx[0][2] = _14;
    mtx.mMtx[1][2] = _18;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x21, GX_MTX2x4);
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x24, GX_MTX2x4);
    MR::loadTexProjectionMtx(39);

    mWaterTex->load(GX_TEXMAP0);
    JUTTexture tex(MR::getScreenResTIMG(), 0);
    tex.load(GX_TEXMAP1);
    mWaterIndTex->load(GX_TEXMAP2);

    if (unknownVal != 0 && MR::isCameraInWater()) {
        GXSetNumIndStages(0);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevDirect(GX_TEVSTAGE2);
        GXSetTevDirect(GX_TEVSTAGE3);
    }
    else {
        GXSetNumIndStages(1);
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
        GXSetTevIndWarp(GX_TEVSTAGE3, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);

        Mtx23 new_mtx;
        new_mtx[0][0] = 0.1;
        new_mtx[0][1] = 0.0;
        new_mtx[0][2] = 0.0;
        new_mtx[1][0] = 0.0;
        new_mtx[1][1] = 0.1;
        new_mtx[1][2] = 0.0;

        GXSetIndTexMtx(GX_ITM_0, new_mtx, 0);
    }

    GXSetNumTevStages(4);
    GXSetTevColor(GX_TEVREG0, (GXColor&)color1);
    GXSetTevColor(GX_TEVREG1, (GXColor&)color2);
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
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

void OceanRingDrawer::loadMaterialBloom() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);
    mtx.mMtx[0][2] = _14;
    mtx.mMtx[1][2] = _18;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x21, GX_MTX2x4);
    mWaterTex->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    TVec3f zDir = MR::getCamZDir();
    f32 v6 = ((MR::clamp(zDir.y, -0.2f, 0.1f) - -0.2f) / 0.30000001f);
    f32 v8 = MR::clamp(WaterAreaFunction::getCameraWaterDepth(), 150.0f, 300.0f);
    f32 v9 = MR::divideFromDiff(v8, 150.0f, 150.0f);

    if (v6 >= v9) {
        v9 = v9;
    }
    else {
        v9 = v6;
    }

    f32 v18 = (150.0f + (105.0f * v9));
    u8 v19 = v18;
    GXSetNumTevStages(2);
    Color8 v14(80, 80, 80, v19);
    Color8 v13(80, 80, 80, 255);
    GXSetTevColor(GX_TEVREG0, (GXColor&)v14);
    GXSetTevColor(GX_TEVREG1, (GXColor&)v13);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1u, GX_LEQUAL, 1u);
    GXSetZCompLoc(1u);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}
