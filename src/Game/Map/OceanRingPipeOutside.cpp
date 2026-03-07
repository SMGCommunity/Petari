#include "Game/Map/OceanRingPipe.hpp"
#include "Game/Map/OceanRingPipeOutside.hpp"

#include "Game/Scene/SceneFunction.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    const f32 sTexRateU = 0.08f;
    const f32 sTexRateV = 1.0f;
    const f32 sTexSpeedU = -0.004f;
    const f32 sIndirectScale = 0.15f;
    const f32 sEnvMapScale = 0.6f;


    static GXColor sTevReg0 = {0x14, 0x65, 0xFF, 0xB9};
}  // namespace

OceanRingPipeOutside::OceanRingPipeOutside(const OceanRingPipe* pPipe) : LiveActor("オーシャンリングの内側"),
    mRingPipe(pPipe), mTexU(0.0f), mWaterPipeIndirectTex(nullptr), mWaterPipeHighLightTex(nullptr), mDispListLength(0), mDispList(nullptr) {
}

void OceanRingPipeOutside::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_OceanRingPipeOutside);
    initDisplayList();
    MR::invalidateClipping(this);
    mWaterPipeIndirectTex = new JUTTexture(MR::loadTexFromArc("OceanRing.arc", "WaterPipeIndirect.bti"), 0);
    mWaterPipeHighLightTex = new JUTTexture(MR::loadTexFromArc("OceanRing.arc", "WaterPipeHighLight.bti"), 0);
    makeActorAppeared();
}

void OceanRingPipeOutside::movement() {
    mTexU = MR::repeat(mTexU + sTexSpeedU, 0.0f, 1.0f);
}

void OceanRingPipeOutside::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u32 x = (mRingPipe->_9C - 1) * (24 * mRingPipe->_98 + 3);
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

void OceanRingPipeOutside::draw() const {
    if (!MR::isValidDraw(mRingPipe))
        return;

    loadMaterial();
    GXCallDisplayList(mDispList, mDispListLength);
    GXSetTevDirect(GX_TEVSTAGE0);
}

const GXColor sFogCol = {0xFF, 0xFF, 0xFF, 0xFF};
const Mtx sMtx = { 0.5f,  0.0f, 0.0f, 0.5f,
                   0.0f, -0.5f, 0.0f, 0.5f,
                   0.0f,  0.0f, 1.0f, 0.0f }; // this is being put into .rodata and not .data as intended
void OceanRingPipeOutside::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 0x10);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetArray(GX_VA_POS, mRingPipe->_A0, 12);
    GXSetArray(GX_VA_NRM, mRingPipe->_A4, 6);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXLoadNrmMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, 4, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_NRM, 0x21, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = mTexU;
    mtx.mMtx[1][2] = 0.0f;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);

    TPos3f mtx2;
    mtx2.identity();
    mtx2.setInline(MR::getCameraViewMtx());
    // this should be mtx2.zeroTrans(), but its refusing to inline
    mtx2[0][3] = 0.0f;
    mtx2[1][3] = 0.0f;
    mtx2[2][3] = 0.0f;
    TMtx34f mtx3;
    mtx3.identity();
    TMtx34f* ptrmtx = &mtx2;
    mtx3.scale(sEnvMapScale);
    PSMTXConcat(*ptrmtx, mtx3, *ptrmtx);
    PSMTXConcat(*ptrmtx, sMtx, mtx2);
    GXLoadTexMtxImm(ptrmtx->toMtxPtr(), 0x21, GX_MTX2x4);

    mWaterPipeIndirectTex->load(GX_TEXMAP0);
    mWaterPipeHighLightTex->load(GX_TEXMAP1);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
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

    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_APREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetZCompLoc(1);
    if (mRingPipe->_90) {
        GXSetCullMode(GX_CULL_BACK);
    } else {
        GXSetCullMode(GX_CULL_NONE);
    }

    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, MR::getNearZ(), MR::getFarZ(), sFogCol);
}

void OceanRingPipeOutside::sendGD() const {
    u8 something = 0x98;
    s32 vv;
    s32 v;
    f32 f29;
    f32 f28;
    f29 = 1.0f;
    f28 = 0.0f;

    for (s32 i = 0; i < mRingPipe->_9C - 1; i++) {
        f32 f27 = 0.0f;
        vv = mRingPipe->_98;
        GDWrite_u8(something);
        GDWrite_u16((u16)(vv << 1));
        for (s32 j = 0; j < mRingPipe->_98; j++) {
            v = j * mRingPipe->_9C;
            u16 cl2 = i + v + 1;
            u16 cl = i + v;
            GDWrite_u16(cl2);
            GDWrite_u16(cl2);
            GDWrite_f32(f27);
            GDWrite_f32(f29);

            GDWrite_u16(cl);
            GDWrite_u16(cl);
            GDWrite_f32(f27);
            GDWrite_f32(f28);

            f27 += sTexRateU;
        }
        f28 = f29;
        f29 += sTexRateV;
    }
}

OceanRingPipeOutside::~OceanRingPipeOutside() {
}