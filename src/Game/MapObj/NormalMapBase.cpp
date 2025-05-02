#include "Game/MapObj/NormalMapBase.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <cstring>

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

void NormalMapBase::loadDiffuseGradTex(_GXTexMapID mapID) const {
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
