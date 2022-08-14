#include "Game/MapObj/SpinDriverPathDrawer.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/Util.h"
#include "Game/Util/DirectDraw.h"


SpinDriverPathDrawInit::SpinDriverPathDrawInit() : NameObj("スピンドライバーレール描画初期化"),
    mOrangeTexture(NULL), mGreenTexture(NULL), mPinkTexture(NULL), mMaskTexture(NULL), mIsPathAtOpa(false) {
    
    void (SpinDriverPathDrawInit::*drawFunc)(void) = &SpinDriverPathDrawInit::initDraw;
    MR::registerPreDrawFunction(MR::Functor(this, drawFunc), 18);
    mOrangeTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "NormalColor.bti"), 0);
    mGreenTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Green.bti"), 0);
    mPinkTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Pink.bti"), 0);
    mMaskTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Mask.bti"), 0);
}

SpinDriverPathDrawer::SpinDriverPathDrawer(SpinDriverShootPath *pShootPath) : LiveActor("パス描画") {
    mShootPath = pShootPath;
    _90 = 0;
    _94 = 0;
    _98 = 0;
    _9C = 0;
    mPositionCount = 0;
    _A4 = 0;
    _A8 = 0;
    _AC = 0.0f;
    _B0 = 0.0f;
    mColor = -1;
    _B8 = 0.0f;
    mFadeScale = 1.0f;
    mMaskLength = 5000.0f;
    MR::createSceneObj(SceneObj_SpinDrawerPathDrawInit);
}

#ifdef NON_MATCHING
void SpinDriverPathDrawInit::initDraw() {
    TDDraw::cameraInit3D();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2u);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, 0, 0x7D);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    Color8 color = Color8(0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor&)color); 
    GXSetFogRangeAdj(GX_FALSE, 0, NULL);
    mMaskTexture->load(GX_TEXMAP1);
}
#endif

namespace MR {
    void setSpinDriverPathColorNormal() {
        MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mOrangeTexture->load(GX_TEXMAP0);
    }

    void setSpinDriverPathColorGreen() {
        MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mGreenTexture->load(GX_TEXMAP0);
    }
    
    void setSpinDriverPathColorPink() {
        MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mPinkTexture->load(GX_TEXMAP0);
    }

    bool isDrawSpinDriverPathAtOpa() {
        if (!MR::isExistSceneObj(SceneObj_SpinDrawerPathDrawInit)) {
            return false;
        }

        return MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mIsPathAtOpa;
    }

    void onDrawSpinDriverPathAtOpa() {
        MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mIsPathAtOpa = true;
    }

    void offDrawSpinDriverPathAtOpa() {
        MR::getSceneObj<SpinDriverPathDrawInit*>(SceneObj_SpinDrawerPathDrawInit)->mIsPathAtOpa = false;
    }
};

void SpinDriverPathDrawer::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, -1, -1, -1, 0x12);
    initPositionList(75.0f, 20.0f);
    initClipping();
    initPathEnd();
    makeActorDead();
}

// SpinDriverPathDrawer::calcPositionCount
// SpinDriverPathDrawer::initPositionList

void SpinDriverPathDrawer::initClipping() {
    f32 v3 = 0.0f;
    mShootPath->calcClippingInfo(&mPosition, &v3, 150.0f, 150.0f);
    MR::setClippingTypeSphere(this, v3);
    MR::validateClipping(this);
    MR::setClippingFarMax(this);
}

void SpinDriverPathDrawer::initPathEnd() {
    s32 arg = -1;
    MR::getRailArg1WithInit(mShootPath->mRailRider, &arg);

    if (arg >= 0) {
        _B8 = arg / 1000.0f;
    }
    else {
        _B8 = 0.0f;
    }
}

f32 SpinDriverPathDrawer::calcDrawCode() const {
    if (_B8 <= 0.0f) {
       return _AC;
    }

    f32 val = 1.0f - _B8;
    f32 ret = (_AC < val) ? _AC : val;
    return ret;
}

f32 SpinDriverPathDrawer::calcDrawBaseCode() const {
    if (_B8 <= 0.0f) {
       return _B0;
    }

    f32 val = _B0;

    if (val < (1.0f - _B8)) {
        return val;
    }

    return 1.0f - _B8;
}

void SpinDriverPathDrawer::setCoord(f32 coord) {
    _AC = coord;
    if (_B0 < coord) {
        _B0 = coord;
    }
}

void SpinDriverPathDrawer::setColorNormal() {
    mColor = -1;
}

void SpinDriverPathDrawer::setColorGreen() {
    mColor = 0;
}

void SpinDriverPathDrawer::setColorPink() {
    mColor = 1;
}

void SpinDriverPathDrawer::setMaskLength(f32 mask) {
    if (mask < 1.0f) {
        mMaskLength = 1.0f;
    }
    else {
        mMaskLength = mask;
    }
}

void SpinDriverPathDrawer::setFadeScale(f32 fade) {
    if (fade < 0.001f) {
        mFadeScale = 0.001f;
    }
    else {
        mFadeScale = fade;
    }
}

// SpinDriverPathDrawer::draw

void SpinDriverPathDrawer::sendPoint(const TVec3f &a1, f32 a2, f32 a3) {
    GXWGFifo.f32 = a1.x;
    GXWGFifo.f32 = a1.y;
    GXWGFifo.f32 = a1.z;
    GXWGFifo.f32 = a2;
    GXWGFifo.f32 = a3;
}

SpinDriverPathDrawer::~SpinDriverPathDrawer() {

}

SpinDriverPathDrawInit::~SpinDriverPathDrawInit() {

}