#include "Game/Map/OceanRingPipe.hpp"
#include "Game/Map/OceanRingPipeInside.hpp"

#include "Game/Scene/SceneFunction.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    const f32 sTexRateU0 = 0.08f;
    const f32 sTexRateV0 = 0.3f;
    const f32 sTexRateU1 = 0.04f;
    const f32 sTexRateV1 = 0.3f;
    const f32 sTexSpeedU0 = -0.006f;
    const f32 sTexSpeedU1 = -0.006f;
    const f32 sIndirectScale = 0.3f;
    const f32 sTexSpeedV0 = 0.0f;
    const f32 sTexSpeedV1 = 0.0f;

    static GXColor sTevReg0 = {0x00, 0x1C, 0x00, 0xFF};
    static GXColor sTevReg1 = {0x78, 0xA0, 0xF6, 0xFF};
}

OceanRingPipeInside::OceanRingPipeInside(const OceanRingPipe* pPipe) : LiveActor("オーシャンリングの内側"),
	mRingPipe(pPipe), mTexU0(0.0f), mTexV0(0.0f), mTexU1(0.0f), mTexV1(0.0f), mWaterPipeInsideTex(nullptr), mDispListLength(0), mDispList(nullptr) {

}

void OceanRingPipeInside::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_OceanRingPipeInside);
    initDisplayList();
    MR::invalidateClipping(this);
    mWaterPipeInsideTex = new JUTTexture(MR::loadTexFromArc("OceanRing.arc", "WaterPipeInside.bti"), 0);
    makeActorAppeared();
}

void OceanRingPipeInside::movement() {
    mTexU0 = MR::repeat(mTexU0 + sTexSpeedU0, 0.0f, 1.0f);
    mTexV0 = MR::repeat(mTexV0 + sTexSpeedV0, 0.0f, 1.0f);
    mTexU1 = MR::repeat(mTexU1 + sTexSpeedU1, 0.0f, 1.0f);
    mTexV1 = MR::repeat(mTexV1 + sTexSpeedV1, 0.0f, 1.0f);
}

void OceanRingPipeInside::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u32 x = (mRingPipe->_9C - 1) * (36 * mRingPipe->_98 + 3);
    u32 length = 0x20 * ((x >> 5) + 2);
    mDispList = new (0x20) u8[length];
    DCInvalidateRange(mDispList, length);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispList, length);
    __GDCurrentDL = &obj;
    sendGD();
    GDPadCurr32();
    mDispListLength = obj.ptr - obj.start;
    DCStoreRange(mDispList, length);
}

void OceanRingPipeInside::draw() const {
    if (!MR::isValidDraw(mRingPipe) || !mRingPipe->_90)
        return;

    loadMaterial();
    GXCallDisplayList(mDispList, mDispListLength);
}

const GXColor sFogCol = {0xFF, 0xFF, 0xFF, 0xFF};
void OceanRingPipeInside::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetArray(GX_VA_POS, mRingPipe->_A0, 12);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXLoadNrmMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = mTexU0;
    mtx.mMtx[1][2] = mTexV0;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);
    mtx.mMtx[0][2] = mTexU1;
    mtx.mMtx[1][2] = mTexV1;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x21, GX_MTX2x4);

    mWaterPipeInsideTex->load(GX_TEXMAP0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP0);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
    Mtx23 new_mtx;
    new_mtx[0][0] = sIndirectScale;
    new_mtx[0][1] = 0.0f;
    new_mtx[0][2] = 0.0f;
    new_mtx[1][0] = 0.0f;
    new_mtx[1][1] = sIndirectScale;
    new_mtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, new_mtx, 0);

    GXSetTevColor(GX_TEVREG0, sTevReg0);
    GXSetTevColor(GX_TEVREG1, sTevReg1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, MR::getNearZ(), MR::getFarZ(), sFogCol);
}

void OceanRingPipeInside::sendGD() const {
    u8 something = 0x98;
    s32 vv;
    s32 v;
    f32 f30;
    f32 f29;
    f32 f28;
    f32 f27;
    f32 f26;
    f32 f25;
    f29 = sTexRateU0;
    f30 = sTexRateU1;
    f26 = 0.0f;
    f25 = 0.0f;
    f28 = sTexRateV0;
    f27 = sTexRateV1;

    for (s32 i = 0; i < mRingPipe->_9C - 1; i++) {
        f32 f24 = 0.0f;
        f32 f23 = 0.0f;
        vv = mRingPipe->_98;
        GDWrite_u8(something);
        GDWrite_u16((u16)(vv << 1));
        for (s32 j = 0; j < mRingPipe->_98; j++) {
            v = j * mRingPipe->_9C;
            GDWrite_u16(i + v);
            GDWrite_f32(f24);
            GDWrite_f32(f26);
            GDWrite_f32(f23);
            GDWrite_f32(f25);

            GDWrite_u16(i + (v + 1));
            GDWrite_f32(f24);
            GDWrite_f32(f28);
            GDWrite_f32(f23);
            GDWrite_f32(f27);

            f24 += f29;
            f23 += f30;
        }
        f26 = f28;
        f25 = f27;
        f28 += sTexRateV0;
        f27 += sTexRateV1;
    }
}

OceanRingPipeInside::~OceanRingPipeInside() {
}