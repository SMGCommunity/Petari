#include "Game/MapObj/ClipFieldFillDraw.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

ClipFieldFillDraw::ClipFieldFillDraw(const char* pName)
    : NameObj(pName), mFillBlendMode(GX_BM_BLEND), mFillSrcFactor(GX_BL_SRCALPHA), mFillDstFactor(GX_BL_ONE), mFillLogicOp(GX_LO_NOOP),
      _1C(255, 230, 80, 200), _20(255, 255, 255, 255), _24(), _25() {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_ClipFieldFillDraw);
    MR::createScreenAlphaSceneObj(1, 1.0f);
}

void ClipFieldFillDraw::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        return;
    }
}

void ClipFieldFillDraw::setModeSubColor() {
    mFillBlendMode = GX_BM_SUBTRACT;
    mFillDstFactor = GX_BL_ONE;
    mFillSrcFactor = GX_BL_ONE;
    mFillLogicOp = GX_LO_NOOP;
}

void ClipFieldFillDraw::setUpFillScreen() const {
    Mtx44 projectionMtx;
    C_MTXOrtho(projectionMtx, 0.0f, MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), -1.0f, 1.0f);
    GXSetProjection(projectionMtx, GX_ORTHOGRAPHIC);

    TMtx34f posMtx;
    posMtx.identity();
    GXLoadPosMtxImm((MtxPtr)&posMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXTevAlphaArg alphaA;
    GXTevAlphaArg alphaB;

    if (_25) {
        alphaA = GX_CA_TEXA;
        alphaB = GX_CA_KONST;
    } else {
        alphaA = GX_CA_KONST;
        alphaB = GX_CA_TEXA;
    }

    GXSetTevKColor(GX_KCOLOR0, Color8(0, 0, 0, 1));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevAlphaIn(GX_TEVSTAGE0, alphaA, alphaB, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetDither(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetBlendMode(mFillBlendMode, mFillSrcFactor, mFillDstFactor, mFillLogicOp);
}

void ClipFieldFillDraw::sendFillScreen(GXColor color) const {
    GXSetTevColor(GX_TEVREG0, color);

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    {
        u16 screenWidth = MR::getScreenWidth();
        u16 screenHeight = MR::getScreenHeight();

        GXPosition2u16(0, 0);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition2u16(screenWidth, 0);
        GXTexCoord2f32(1.0f, 0.0f);

        GXPosition2u16(0, screenHeight);
        GXTexCoord2f32(0.0f, 1.0f);

        GXPosition2u16(screenWidth, screenHeight);
        GXTexCoord2f32(1.0f, 1.0f);
    }
    GXEnd();
}

void ClipFieldFillDraw::draw() const {
    MR::loadScreenAlphaTexture(1, GX_TEXMAP0);
    setUpFillScreen();
    sendFillScreen(_1C);
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    GXSetZScaleOffset(1.0f, 0.0f);
    MR::clearAlphaBuffer(0);
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    GXSetAlphaUpdate(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);

    if (_24) {
        TDDraw::setup(0, 1, 0);
        GXSetCullMode(GX_CULL_FRONT);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    }
}

namespace MR {
    NameObj* createClipFieldFillDraw(const char* pParam1) {
        return static_cast< ClipFieldFillDraw* >(MR::createSceneObj(SceneObj_ClipFieldFillDraw));
    }

    NameObj* createVolumeDrawSpotLight(const char* pParam1) {
        ClipFieldFillDraw* clipFieldFillDraw;

        clipFieldFillDraw = static_cast< ClipFieldFillDraw* >(createClipFieldFillDraw(pParam1));
        clipFieldFillDraw->_25 = false;
        clipFieldFillDraw->_24 = false;
        clipFieldFillDraw->setModeSubColor();
        clipFieldFillDraw->_1C = Color8(70, 35, 11, 80);
        clipFieldFillDraw->_20 = Color8(255, 255, 128, 80);

        return clipFieldFillDraw;
    }
};  // namespace MR
