#include "Game/MapObj/NormalMapBase.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXGeometry.h>
#include <cstring>

NormalMapBase::NormalMapBase(const char *pName) : LiveActor(pName) {
    mBtkPlayer = nullptr;
    _150 = 0;
    for (u32 i = 0; i < 0x10; i++) {
        PSMTXIdentity(_184[i]);
    }

    _13A = 0;
    nGradTexMode = 0;
    _E8 = 0;
    _EC = 0;
    mBackLightMode = 0;
    _4C5 = 0;
}

void NormalMapBase::setup(const char *pName) {
    _4C5 = 1;
    _4C4 = 1;
    initModelManagerWithAnm(pName, nullptr, false);
    MR::connectToScene(this, 0x22, 5, -1, 0x18);
    MR::initLightCtrlNoDrawMapObj(this);
    setupLighting();
    setupTexture();
    MR::invalidateClipping(this);
    mHeight = 128;
    mWidth = 128;
    _4B8 = new(0x20) JUTTexture(mHeight, mWidth, GX_TF_IA8);
    appear();
}

void NormalMapBase::initNormalMap() {
    _4C5 = 1;
    _4C4 = 0;
    setupLighting();
    setupTexture();
}

// ...

void NormalMapBase::control() {

}

void NormalMapBase::calcAnim() {
    LiveActor::calcAnim();
    updateBtkMtx();
    OSLockMutex(&MR::MutexHolder<0>::sMutex);
    MR::getJ3DModel(this)->viewCalc();
    OSUnlockMutex(&MR::MutexHolder<0>::sMutex);
}

// ...

void NormalMapBase::loadTexMtxBtk(J3DMaterial *pMaterial) const {
    if (mBtkPlayer != nullptr) {
        if (_150) {
            GXLoadTexMtxImm(_184[pMaterial->mIndex], 0x1E, GX_MTX2x4);
        }
    }
}

void NormalMapBase::draw() const {
    if (!MR::isDead(this)) {
        MR::loadActorLight(this);

        if (_EC == 2) {
            indirectCapture();
        }

        MR::loadViewMtx();
        MR::loadProjectionMtx();
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetDither(GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_POS_XY, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
        MtxPtr cameraViewMtx = MR::getCameraViewMtx();
        TPos3f v7;
        Mtx mtx;
        PSMTXIdentity(mtx);
        MR::makeMtxTRS(mtx, this);
        PSMTXConcat(cameraViewMtx, mtx, mtx);
        PSMTXCopy((const MtxPtr)mtx, (MtxPtr)_F4);
        
        MR::makeMtxTR(v7, this);
        MR::normalize(&v7);

        switch (_EC) {
            case 0:
                drawSetting(v7);
                break;
            case 1:
                drawSettingForDebug(v7);
                break;
            case 2:
                drawSettingForCapture(v7);
                break;
        }

        shapeAnalyzeDraw(MR::getJ3DModel(this));
        standardDraw(MR::getJ3DModel((this)));
        GXSetDither(GX_TRUE);
    }
}

// ...

bool NormalMapBase::isNormalMapMaterial(const char *pName) const {
    if (strstr(pName, "Normal")) {
        return true;
    }

    return strstr(pName, "Nrm") != 0;
}

void NormalMapBase::standardDraw(J3DModel *pModel) const {
    if ((_138 & 0x10) == 0) {
        for (u16 i = 0; i < pModel->mModelData->mMaterialTable.mMaterialCount; i++) {
            J3DMaterial* mat = pModel->mModelData->getMaterial(i);
            const char* matName = MR::getMaterialName(pModel->mModelData, mat->mIndex);
            if (!isNormalMapMaterial(matName)) {
                MR::simpleDraw(pModel, mat);
            }
        }
    }
}

// ...

static s16 width = 0x100;
static s16 height = 0x100;

void NormalMapBase::loadDiffuseGradTex(GXTexMapID mapID) const {
    GXTexObj obj;
    GXInitTexObj(&obj, mImagePtr, width, height, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);

    if (_138 & 1) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
    else {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }

    GXLoadTexObj(&obj, mapID);
}

void NormalMapBase::loadNormalTex(const JUTTexture *pTex, GXTexMapID id) const {
    GXTexObj obj;
    GXInitTexObj(&obj, pTex->mImage, pTex->mTIMG->mWidth, pTex->mTIMG->mHeight, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if ((_13A & 2) != 0 && (_13A & 0x4) != 0) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
    else if ((_13A & 0x2) != 0) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    }
    else if ((_13A & 0x4) != 0) {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_NEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    }
    else {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    }

    GXLoadTexObj(&obj, id);
}

void NormalMapBase::drawSetting(MtxPtr mtx) const {
    updateLightMtx(mtx);
    Mtx scale_mtx;
    PSMTXScale(scale_mtx, 0.0f, 0.0f, 0.0f);
    MR::setMtxTrans(scale_mtx, 0.5f, 0.5f, 0.0f);
    GXLoadTexMtxImm(scale_mtx, 0x21, GX_MTX2x4);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);

    if (mHardLightColorMask) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_VTX, mHardLightColorMask, GX_DF_CLAMP, GX_AF_NONE);
    }
    else if (mUseModelLightReg) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
    }
    else {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    }

    GXSetChanCtrl(GX_ALPHA0, 1, GX_SRC_REG, GX_SRC_REG, mHardLightAlphaMask, GX_DF_SIGN, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    setTevForObject();
    loadDiffuseGradTex(GX_TEXMAP1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
}

void NormalMapBase::setTevForObject_Material() const {
    if (mBtkPlayer && _150) {
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    }
    else {
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    }

    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x24, 0, 0x7D);
}

void NormalMapBase::setTevForObject() const {
    GXSetNumTexGens(3);
    GXSetNumIndStages(1);
    GXSetNumTevStages(4);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_FALSE, GX_FALSE, GX_FALSE);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_FALSE, GX_FALSE, GX_FALSE);
    GXSetTexCoordScaleManually(GX_TEXCOORD2, GX_FALSE, GX_FALSE, GX_FALSE);
    setTevForObject_Material();
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndirect(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_STU, GX_ITM_0, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);

    GXTevColorArg v3, v5, v4, v6;
    v3 = GX_CC_ONE;
    v4 = GX_CC_RASC;
    v5 = GX_CC_ONE;
    v6 = GX_CC_RASC;

    if ((mHardLightColorMask & 0x1) == 0) {
        v3 = GX_CC_ZERO;
    }

    if ((mHardLightColorMask & 0x2) == 0) {
        v5 = GX_CC_ZERO;
    }

    if (mLightAMode <= 1) {
        v3 = GX_CC_TEXC;
    }

    if (mLightBMode == 1) {
        v5 = GX_CC_APREV;
    }

    GXTevAlphaArg v8 = GX_CA_RASA;

    if (mLightBMode == 2) {
        v8 = GX_CA_APREV;
    }

    if (mLightBMode == 2) {
        v8 = GX_CA_APREV;
    }

    if (mBackLightMode == 4) {
        v4 = GX_CC_ONE;
        v6 = GX_CC_ONE;
    }

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, v3, v4, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, v5, v6, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVREG2);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_KONST, GX_CA_ZERO, v8, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C2, GX_CC_TEXC, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    Color8 color(255, 255, 255, 0);
    GXSetTevKColor(GX_KCOLOR0, color);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

    switch (mBackLightMode)
    {
        case 0:
            GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_ONE, GX_CC_CPREV, GX_CC_APREV);
            break;
        case 1:
            GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_APREV, GX_CC_CPREV, GX_CC_ZERO);
            break;
        case 2:
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C2, GX_CC_TEXC, GX_CC_APREV);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_C2, GX_CC_CPREV, GX_CC_C0);
            break;
        case 3:
            GXSetNumTevStages(3);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C1, GX_CC_C2, GX_CC_TEXC, GX_CC_APREV);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            break;
        case 4:
        case 7:
            GXSetNumTevStages(3);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C2, GX_CC_ONE, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            break;
        case 5:
            GXSetNumTevStages(3);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            break;
        case 6:
            GXSetNumTevStages(1);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            break;
        default:
            break;
    }

    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
}

void NormalMapBase::setTevForDebug() const {
    GXSetNumTexGens(1);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, 0, 0, 0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    Mtx mtx;
    PSMTXScale(mtx, 0.0f, 0.0f, 0.0f);
    MR::setMtxTrans(mtx, 0.5f, 0.5f, 0.0f);
    GXLoadTexMtxImm(mtx, 0x1E, GX_MTX2x4);
    loadDiffuseGradTex(GX_TEXMAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
}
