#include "Game/Screen/WaterCameraFilter.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/MissLayout.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"
#include "revolution/gx/GXBump.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXRegs.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/gx/GXTransform.h"

namespace NrvWaterCameraFilter {
    NEW_NERVE(WaterCameraFilterNrvAir, WaterCameraFilter, Air);
    NEW_NERVE(WaterCameraFilterNrvAirToWater, WaterCameraFilter, AirToWater);
    NEW_NERVE(WaterCameraFilterNrvWater, WaterCameraFilter, Water);
    NEW_NERVE(WaterCameraFilterNrvWaterToAir, WaterCameraFilter, WaterToAir);
};  // namespace NrvWaterCameraFilter

static GXColor sDefaultColor;

WaterCameraFilter::WaterCameraFilter() : LiveActor("水中カメラフィルター") {
    mWaterAlpha = 0.0f;
    mWavePhase = 0.0f;
    _94 = sDefaultColor;
    mScreenTex = nullptr;
    mFilterTex = nullptr;
}

void WaterCameraFilter::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_WaterCameraFilter);
    MR::invalidateClipping(this);
    initNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvAir::sInstance);
    initScreenTex();
    JUTTexture* filterTex = new JUTTexture(MR::loadTexFromArc("WaterCameraFilter.arc", "WaterCameraFilter.bti"), 0);
    mFilterTex = filterTex;
    makeActorAppeared();
}

void WaterCameraFilter::initScreenTex() {
    MR::CurrentHeapRestorer restorer(MR::getSceneHeapGDDR3());
    mScreenTex = new JUTTexture(MR::getFrameBufferWidth(), MR::getScreenHeight(), GX_TF_RGB565);
}

void WaterCameraFilter::exeAirToWater() {
    if (!MR::isCameraInWater()) {
        setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvWaterToAir::sInstance);
    } else {
        mWaterAlpha += 20.0f;
        if (MR::isDemoActive() || mWaterAlpha >= 255.0f) {
            mWaterAlpha = 255.0f;
            setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvWater::sInstance);
        }
    }
}

void WaterCameraFilter::exeWaterToAir() {
    if (MR::isCameraInWater()) {
        setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvAirToWater::sInstance);
    } else {
        mWaterAlpha -= 20.0f;
        if (MR::isDemoActive() || mWaterAlpha <= 0.0f) {
            mWaterAlpha = 0.0f;
            setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvAir::sInstance);
        }
    }
}

static GXColor sWaterColor = {0x32, 0x32, 0, 0};

void WaterCameraFilter::control() {
    if (MR::isCameraInWater()) {
        f32 v2 = mWavePhase + 0.5f;
        mWavePhase += 0.5f;

        if (v2 > 360.0f) {
            mWavePhase = v2 - 360.0f;
        }

        f32 depth = WaterAreaFunction::getCameraWaterDepth();
        f32 v4 = 0.0f;
        if (depth < 0.0f) {
            v4 = v4;
        } else {
            v4 = 3000.0f;
            if (depth > 3000.0f) {
                v4 = v4;
            } else {
                v4 = depth;
            }
        }

        GXColor in = sWaterColor;
        GXColor out = sDefaultColor;
        GXColor final = MR::lerp(in, out, v4 / 3000.0f);
        in = final;
        _94 = final;
    }
}

void WaterCameraFilter::draw() const {
    if (!isNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvAir::sInstance)) {
        GXRenderModeObj* renderObj = JUTVideo::sManager->mRenderModeObj;
        GXSetCopyFilter(GX_FALSE, renderObj->sample_pattern, GX_FALSE, renderObj->vfilter);
        mScreenTex->capture(0, 0, GX_TF_RGB565, false, 0);
        GXSetCopyFilter(GX_FALSE, renderObj->sample_pattern, GX_TRUE, renderObj->vfilter);
        loadMaterial();
        TVec2f pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        TVec3f* playerCenter = MR::getPlayerCenterPos();
        MR::calcScreenPosition(&pos, *playerCenter);
        pos.x /= MR::getScreenWidth();
        pos.y /= MR::getScreenHeight();

        f32 f1 = 1.0f;
        f32 f31 = 0.5f;
        f32 centerU = 1.0f - pos.x;
        f32 centerV = 1.0f - pos.y;

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);

        // top left
        GX_WRITE_U16(0);
        GX_WRITE_U16(0);
        GX_WRITE_F32(centerU - f31);
        GX_WRITE_F32(centerV - f31);
        GX_WRITE_F32(0.0f);
        GX_WRITE_F32(0.0f);

        // top right
        GX_WRITE_S16(MR::getFrameBufferWidth());
        GX_WRITE_U16(0);
        GX_WRITE_F32(centerU + f31);
        GX_WRITE_F32(centerV - f31);
        GX_WRITE_F32(1.0f);
        GX_WRITE_F32(0.0f);

        // bottom left
        s32 h = MR::getScreenHeight();
        GX_WRITE_S16(0);
        GX_WRITE_S16(h);
        GX_WRITE_F32(centerU - f31);
        GX_WRITE_F32(centerV + f31);
        GX_WRITE_F32(0.0f);
        GX_WRITE_F32(1.0f);

        // bottom right
        s32 height = MR::getScreenHeight();
        GX_WRITE_S16(MR::getFrameBufferWidth());
        GX_WRITE_S16(height);
        GX_WRITE_F32(centerU + f31);
        GX_WRITE_F32(centerV + f31);
        GX_WRITE_F32(1.0f);
        GX_WRITE_F32(1.0f);
        MR::loadProjectionMtx();
        MR::loadViewMtx();
        GXEnd();
    }
}

void WaterCameraFilter::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    Mtx v21;
    s32 height = MR::getScreenHeight();
    C_MTXOrtho(v21, 0.0f, height, 0.0f, MR::getFrameBufferWidth(), -10000.0f, 10000.0f);
    GXSetProjection(v21, GX_ORTHOGRAPHIC);
    TMtx34f v20;
    v20.identity();
    GXLoadPosMtxImm(v20, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, 0x1Eu, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, 0x3Cu, 0, 0x7D);
    TVec2f screenPos;
    screenPos.x = 0.0f;
    screenPos.y = 0.0f;
    MR::calcScreenPosition(&screenPos, *MR::getPlayerCenterPos());
    screenPos.x /= MR::getScreenWidth();
    screenPos.y /= MR::getScreenHeight();
    TVec3f screenPosNorm;
    MR::calcNormalizedScreenPosition(&screenPosNorm, *MR::getPlayerCenterPos());
    TMtx34f v19;
    v19.identity();
    TPos3f v18;
    v18.identity();
    v18.setTrans(TVec3f(-0.5f, -0.5f, 0.0f));
    TPos3f v17;
    v17.identity();
    MR::makeMtxRotate(v18, 0.0f, 0.0f, mWavePhase);
    PSMTXConcat(v18, v17, v19);
    v17.setTrans(TVec3f(0.5f, 0.5f, 0.0f));
    PSMTXConcat(v17, v19, v19);
    GXLoadTexMtxImm(v19, 0x1E, GX_MTX3x4);
    mFilterTex->load(GX_TEXMAP0);
    mScreenTex->load(GX_TEXMAP1);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
    f32 ind[2][3];
    ind[0][1] = 0.0f;
    ind[0][0] = 0.05f;
    ind[0][2] = 0.0f;
    ind[1][0] = 0.0f;
    ind[1][1] = 0.05f;
    ind[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, ind, 0);
    GXColor color;
    color.r = _94.r;
    color.g = _94.g;
    color.b = _94.b;
    color.a = mWaterAlpha;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetNumTevStages(1u);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetZCompLoc(0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

void WaterCameraFilter::exeWater() {
    if (!MR::isCameraInWater()) {
        setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvWaterToAir::sInstance);
    }
}

void WaterCameraFilter::exeAir() {
    if (MR::isCameraInWater()) {
        setNerve(&NrvWaterCameraFilter::WaterCameraFilterNrvAirToWater::sInstance);
    }
}

WaterCameraFilter::~WaterCameraFilter() {}
