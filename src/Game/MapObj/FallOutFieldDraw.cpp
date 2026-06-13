#include "Game/MapObj/FallOutFieldDraw.hpp"
#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace MR {
    JUTTexture* getScreenAlphaTexture(s32);
    void loadScreenAlphaTexture(s32, GXTexMapID);
};  // namespace MR

namespace {
    static GXTevOp sAlphaTevOperater = GXTevOp(14);
    static GXBlendMode sFillBlendMode = GXBlendMode(1);
    static GXBlendFactor sFillSrcFactor = GXBlendFactor(4);
    static GXBlendFactor sFillDstFactor = GXBlendFactor(5);
    static GXLogicOp sFillLogicOp = GXLogicOp(5);
    // static const s32 sBlurCount = _;
    // static const f32 sBlurRadius = _;
    // static const f32 sBlurIntensity = _;
    static Color8 sFillColor = Color8(0, 0, 0, 255);
    static Color8 sEdgeColor = Color8(0, 46, 200, 128);
};  // namespace

FallOutFieldDraw::FallOutFieldDraw(const char* pName) : NameObj(pName), _C() {
    MR::createClipAreaHolder();
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_FallOutFieldDraw);
    MR::createScreenAlphaSceneObj(2, 0.5f);
    MR::createScreenAlphaSceneObj(3, 0.25f);
    MR::createScreenAlphaSceneObj(4, 0.25f);
}

void FallOutFieldDraw::setUpFillScreen() const {
    GXSetTevColorIn(GX_TEVSTAGE0, GXTevColorArg(2), GXTevColorArg(15), GXTevColorArg(15), GXTevColorArg(15));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(0), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetTevKColor(GXTevKColorID(0), Color8(0, 0, 0, 1));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GXTevKAlphaSel(28));

    GXTevAlphaArg alphaA = _C ? GXTevAlphaArg(4) : GXTevAlphaArg(6);
    GXTevAlphaArg alphaB = _C ? GXTevAlphaArg(6) : GXTevAlphaArg(4);

    GXSetTevAlphaIn(GX_TEVSTAGE0, alphaA, alphaB, GXTevAlphaArg(1), GXTevAlphaArg(7));
    GXSetTevAlphaOp(GX_TEVSTAGE0, ::sAlphaTevOperater, GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetAlphaCompare(GXCompare(4), 0, GXAlphaOp(0), GXCompare(7), 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZScaleOffset(0.0f, 1.0f);
    GXSetBlendMode(::sFillBlendMode, ::sFillSrcFactor, ::sFillDstFactor, ::sFillLogicOp);
}

void FallOutFieldDraw::setUpEdgeAndClearAlpha() const {
    GXSetTevColor(GXTevRegID(2), ::sEdgeColor);
    GXSetTevColorIn(GX_TEVSTAGE0, GXTevColorArg(4), GXTevColorArg(15), GXTevColorArg(15), GXTevColorArg(15));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(0), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetTevAlphaIn(GX_TEVSTAGE0, GXTevAlphaArg(7), GXTevAlphaArg(2), GXTevAlphaArg(4), GXTevAlphaArg(7));
    GXSetTevAlphaOp(GX_TEVSTAGE0, GXTevOp(0), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_TRUE, 0);
}

void FallOutFieldDraw::init(const JMapInfoIter& rIter) {
    if (!MR::isValidInfo(rIter)) {
        return;
    }

    bool arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);
    _C = arg0;

    if (MR::isExistStageSwitchAppear(rIter)) {
        MR::listenNameObjStageSwitchOnOffAppear(
            this,
            MR::createStageSwitchCtrl(this, rIter),
            MR::Functor(this, &FallOutFieldDraw::activate),
            MR::Functor(this, &FallOutFieldDraw::deactivate));
        MR::deactivateClipArea();
    } else {
        MR::activateClipArea();
    }
}

void FallOutFieldDraw::activate() {
    MR::activateClipArea();
}

void FallOutFieldDraw::deactivate() {
    MR::deactivateClipArea();
}

void FallOutFieldDraw::draw() const {
    if (!MR::isActiveClipArea()) {
        return;
    }

    ImageEffectLocalUtil::setupDrawTexture();

    JUTTexture* screenAlphaTexture2 = MR::getScreenAlphaTexture(2);
    JUTTexture* screenAlphaTexture3 = MR::getScreenAlphaTexture(3);
    JUTTexture* screenAlphaTexture4 = MR::getScreenAlphaTexture(4);

    ImageEffectLocalUtil::capture(screenAlphaTexture2, 0, 1, GXTexFmt(39), true, 0);

    GXSetTevKColor(GXTevKColorID(0), Color8(0, 0, 0, 1));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GXTevKAlphaSel(28));
    GXSetTevKColorSel(GX_TEVSTAGE0, GXTevKColorSel(28));
    GXSetTevColor(GXTevRegID(1), Color8(255, 255, 255, 255));

    GXTevColorArg colorA = _C ? GXTevColorArg(9) : GXTevColorArg(14);
    GXTevColorArg colorB = _C ? GXTevColorArg(14) : GXTevColorArg(9);

    GXSetTevColorIn(GX_TEVSTAGE0, colorA, colorB, GXTevColorArg(15), GXTevColorArg(15));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(14), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));

    GXTevAlphaArg alphaA = _C ? GXTevAlphaArg(4) : GXTevAlphaArg(6);
    GXTevAlphaArg alphaB = _C ? GXTevAlphaArg(6) : GXTevAlphaArg(4);

    GXSetTevAlphaIn(GX_TEVSTAGE0, alphaA, alphaB, GXTevAlphaArg(1), GXTevAlphaArg(7));
    GXSetTevAlphaOp(GX_TEVSTAGE0, GXTevOp(14), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_FALSE, 0);
    GXSetZScaleOffset(0.0f, 0.0f);

    ImageEffectLocalUtil::drawTexture(screenAlphaTexture2, 2, 0, 255, ImageEffectLocalUtil::UNK_0);
    ImageEffectLocalUtil::capture(screenAlphaTexture3, 2, 0, GXTexFmt(39), true, 0);

    GXSetTevColorIn(GX_TEVSTAGE0, GXTevColorArg(9), GXTevColorArg(15), GXTevColorArg(15), GXTevColorArg(15));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(0), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));
    GXSetTevAlphaIn(GX_TEVSTAGE0, GXTevAlphaArg(4), GXTevAlphaArg(7), GXTevAlphaArg(7), GXTevAlphaArg(7));
    GXSetTevAlphaOp(GX_TEVSTAGE0, GXTevOp(0), GXTevBias(0), GXTevScale(0), GX_TRUE, GXTevRegID(0));

    ImageEffectLocalUtil::blurTexture(screenAlphaTexture3, 4, 4, 5, 0.004f, 1.0f);
    ImageEffectLocalUtil::capture(screenAlphaTexture4, 4, 4, GXTexFmt(39), false, 0);
    setUpEdgeAndClearAlpha();
    ImageEffectLocalUtil::drawTexture(screenAlphaTexture4, 1, 0, 255, ImageEffectLocalUtil::UNK_2);
    MR::loadScreenAlphaTexture(2, GXTexMapID(0));
    setUpFillScreen();

    GXSetTevColor(GXTevRegID(1), ::sFillColor);

    ImageEffectLocalUtil::sendTextureVertex(1, 0);
    MR::loadViewMtx();
    MR::loadProjectionMtx();

    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_TRUE, 0);
    GXSetZScaleOffset(1.0f, 0.0f);
}

namespace MR {
    NameObj* createFallOutFieldDraw(const char*) {
        return MR::createSceneObj(SceneObj_FallOutFieldDraw);
    }
};
