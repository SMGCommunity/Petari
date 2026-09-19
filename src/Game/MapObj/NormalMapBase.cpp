#include "Game/MapObj/NormalMapBase.hpp"
#include "Game/Animation/BtkPlayer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphBase/J3DShapeDraw.hpp>
#include <JSystem/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cmath>
#include <cstring>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXGeometry.h>

s16 sgrad_tex_thresh;

static s16 width = 0x100;
static s16 height = 0x100;

void NormalMapBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

NormalMapBase::NormalMapBase(const char* pName) : LiveActor(pName) {
    mBtkPlayer = nullptr;
    _150 = 0;
    for (u32 i = 0; i < ARRAY_SIZE(_184); i++) {
        PSMTXIdentity(_184[i]);
    }

    _13A = 0;
    mGradTexMode = 0;
    _E8 = 0;
    _EC = 0;
    mBackLightMode = 0;
    _4C5 = 0;
}

void NormalMapBase::setup(const char* pName) {
    _4C5 = 1;
    _4C4 = 1;
    initModelManagerWithAnm(pName, nullptr, false);
    MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_None, MR::DrawType_WarpPodPath);
    MR::initLightCtrlNoDrawMapObj(this);
    setupLighting();
    setupTexture();
    MR::invalidateClipping(this);
    mHeight = 128;
    mWidth = 128;
    _4B8 = new (0x20) JUTTexture(mHeight, mWidth, GX_TF_IA8);
    appear();
}

void NormalMapBase::initNormalMap() {
    _4C5 = 1;
    _4C4 = 0;
    setupLighting();
    setupTexture();
}

void NormalMapBase::setupLighting() {
    mTevColor0.r = 0;
    mTevColor0.g = 0;
    mTevColor0.b = 0;
    mTevColor0.a = 0;
    mTevColor1.r = 0;
    mTevColor1.g = 0;
    mTevColor1.b = 0;
    mTevColor1.a = 0;
    _138 = 0;
    mLightAMode = 0;
    mLightBMode = 1;
    mHardLightColorMask = 0;
    mHardLightAlphaMask = 4;
    mUseModelTevReg = 0;
    mUseModelLightReg = 0;
    mLightingHighLevel = 255;
    mLightingLowLevel = 0;
    mAmbColor = 64;
    mMatColor = 255;
    mAmbAlpha = 128;
    mMatAlpha = 255;
    char fileName[256];
    strcpy(fileName, MR::getModelResName(this));
    strcat(fileName, ".nrm.txt");
    if (MR::getResourceHolder(this)->mFileInfoTable->isExistRes(fileName)) {
        const char* pData = static_cast< const char* >(MR::getResourceHolder(this)->mFileInfoTable->getRes(fileName));
        loadData(pData, MR::getResourceHolder(this)->mFileInfoTable->findFileInfo(fileName)->_4);
    }
}

void NormalMapBase::setupTexture() {
    _90 = nullptr;
    mNormalTex = nullptr;
    J3DTexture* pTexture = MR::getJ3DModelData(this)->getTexture();
    for (u16 i = 0; i < pTexture->getNum(); i++) {
        const ResTIMG* pImage = MR::getResTIMG(this, i);
        const char* pName = MR::getTextureName(this, i);
        if (strstr(pName, "Nswap_")) {
            char fileName[64];
            strcpy(fileName, pName + 6);
            strcat(fileName, ".bti");
            pImage = MR::getTexFromArc(fileName, this);
            mNormalTex = new JUTTexture(pImage, 0);
            mTextures[i] = mNormalTex;
            mTextures[i]->mMinType = GX_LINEAR;
            mTextures[i]->mMagType = GX_LINEAR;
            mTextures[i]->init();
        } else if (strstr(pName, "ormal") && !mNormalTex) {
            mNormalTex = new JUTTexture(pImage, 0);
            mTextures[i] = mNormalTex;
        } else {
            _90 = new JUTTexture(pImage, 0);
            mTextures[i] = _90;
        }

        if (strstr(pName, "Zormal")) {
            swapColorGB(mTextures[i]);
        }
    }

    mImagePtr = new (MR::getSceneHeapGDDR3(), 0x20) u16[width * height];
    createGradTexture();
}

void NormalMapBase::control() {
}

void NormalMapBase::calcAnim() {
    LiveActor::calcAnim();
    updateBtkMtx();
    OSLockMutex(&MR::MutexHolder< 0 >::sMutex);
    MR::getJ3DModel(this)->viewCalc();
    OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);
}

void NormalMapBase::updateBtkMtx() {
    if (mBtkPlayer && _150) {
        mBtkPlayer->reflectFrame();
        mBtkPlayer->update();
        J3DAnmTextureSRTKey* pAnm = static_cast< J3DAnmTextureSRTKey* >(mBtkPlayer->mAnmRes);
        if (pAnm) {
            for (u32 i = 0; i < mBtkMtxCount; i++) {
                J3DTextureSRTInfo transform;
                pAnm->getTransform(mBtkMtxIndices[i], &transform);
                J3DGetTextureMtxMayaOld(transform, _184[i]);
            }
        }
    }
}

void NormalMapBase::loadTexMtxBtk(J3DMaterial* pMaterial) const {
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
        const TPos3f& rCameraViewMtx = MR::getCameraViewMtx();
        TPos3f lightMtx;
        Mtx mtx;
        PSMTXIdentity(mtx);
        MR::makeMtxTRS(mtx, this);
        MR::multMtx(mtx, mtx, rCameraViewMtx);
        PSMTXCopy((const MtxPtr)mtx, (MtxPtr)_F4);

        MR::makeMtxTR(lightMtx, this);
        MR::normalize(&lightMtx);

        switch (_EC) {
        case 0:
            drawSetting(lightMtx);
            break;
        case 1:
            drawSettingForDebug(lightMtx);
            break;
        case 2:
            drawSettingForCapture(lightMtx);
            break;
        }

        shapeAnalyzeDraw(MR::getJ3DModel(this));
        standardDraw(MR::getJ3DModel(this));
        GXSetDither(GX_TRUE);
    }
}

void NormalMapBase::updateLightMtx(MtxPtr pMtx) const {
    TPos3f inverse;
    f32 indMtx[2][3];
    TVec3f lightA;
    TVec3f lightB;
    TVec3f localA;
    TVec3f localB;
    switch (mLightAMode) {
    case 0:
        MR::calcLightPos0(&lightA, this);
        break;
    case 1:
        MR::calcLightPos1(&lightA, this);
        break;
    default:
        lightA.zero();
        break;
    }

    MR::normalizeOrZero(&lightA);
    switch (mLightBMode) {
    case 1:
        MR::calcLightPos1(&lightB, this);
        break;
    case 2:
        lightB = mPosition - MR::getCamZdir() * 100000.0f;
        break;
    default:
        lightB.zero();
        break;
    }

    MR::normalizeOrZero(&lightB);
    localA = lightA;
    localB = lightB;
    PSMTXInverse(getBaseMtx(), inverse);
    MR::normalize(&inverse);
    PSMTXMultVecSR(inverse, &localA, &localA);
    PSMTXMultVecSR(inverse, &localB, &localB);
    if (pMtx) {
        TVec3f light(localA * 0.5f);
        light *= 1.0077f;
        indMtx[0][0] = light.x * pMtx[0][0] + light.y * pMtx[1][0] + light.z * pMtx[2][0];
        indMtx[0][1] = light.x * pMtx[0][1] + light.y * pMtx[1][1] + light.z * pMtx[2][1];
        indMtx[0][2] = light.x * pMtx[0][2] + light.y * pMtx[1][2] + light.z * pMtx[2][2];
        light = localB * 0.5f;
        light *= 1.0077f;
        indMtx[1][0] = light.x * pMtx[0][0] + light.y * pMtx[1][0] + light.z * pMtx[2][0];
        indMtx[1][1] = light.x * pMtx[0][1] + light.y * pMtx[1][1] + light.z * pMtx[2][1];
        indMtx[1][2] = light.x * pMtx[0][2] + light.y * pMtx[1][2] + light.z * pMtx[2][2];
    } else {
        indMtx[0][0] = 0.5f * -localA.x;
        indMtx[0][1] = 0.5f * -localA.y;
        indMtx[0][2] = 0.5f * -localA.z;
        indMtx[1][0] = 0.5f * -localB.x;
        indMtx[1][1] = 0.5f * -localB.y;
        indMtx[1][2] = 0.5f * -localB.z;
    }

    GXSetIndTexMtx(GX_ITM_0, indMtx, 0);
}

bool NormalMapBase::isNormalMapMaterial(const char* pName) const {
    if (strstr(pName, "Normal")) {
        return true;
    }

    return strstr(pName, "Nrm") != 0;
}

void NormalMapBase::standardDraw(J3DModel* pModel) const {
    if ((_138 & 0x10) == 0) {
        for (u16 i = 0; i < pModel->mModelData->mMaterialTable.getMaterialNum(); i++) {
            J3DMaterial* pMaterial = pModel->mModelData->getMaterialNodePointer(i);
            const char* pMaterialName = MR::getMaterialName(pModel->mModelData, pMaterial->mIndex);
            if (!isNormalMapMaterial(pMaterialName)) {
                MR::simpleDraw(pModel, pMaterial);
            }
        }
    }
}

void NormalMapBase::shapeAnalyzeDraw(J3DModel* pModel) const {
    for (u16 i = 0; i < pModel->mModelData->getShapeNum(); i++) {
        J3DShape* pShape = pModel->mModelData->getShapeNodePointer(i);
        bool found = false;
        for (u16 j = 0; j < pModel->mModelData->getJointNum(); j++) {
            J3DJoint* pJoint = pModel->mModelData->getJointNodePointer(j);
            J3DMaterial* pMaterial = pJoint->getMesh();
            while (pMaterial) {
                const char* pName = MR::getMaterialName(pModel->mModelData, pMaterial->mIndex);
                if (!isNormalMapMaterial(pName)) {
                    pMaterial = pMaterial->mNext;
                    continue;
                }

                if (pMaterial->mShape == pShape) {
                    setTevForObject_Material();
                    TPos3f localMtx;
                    TPos3f viewMtx;
                    const J3DTransformInfo& rTransform = pJoint->getTransformInfo();
                    f32 rotationZ = rTransform.mRotation.z * (180.0f / 32768.0f);
                    f32 rotationY = rTransform.mRotation.y * (180.0f / 32768.0f);
                    f32 rotationX = rTransform.mRotation.x * (180.0f / 32768.0f);
                    MR::makeMtxTRS(localMtx, rTransform.mTranslate.x, rTransform.mTranslate.y, rTransform.mTranslate.z, rotationX, rotationY,
                                   rotationZ, rTransform.mScale.x, rTransform.mScale.y, rTransform.mScale.z);
                    PSMTXConcat(_F4, localMtx, viewMtx);
                    if (pJoint->getMtxType() == 1) {
                        J3DCalcBBoardMtx(viewMtx);
                        TPos3f inverse;
                        PSMTXInverse(_F4, inverse);
                        PSMTXConcat(inverse, viewMtx, localMtx);
                    } else if (pJoint->getMtxType() == 2) {
                        J3DCalcYBBoardMtx(viewMtx);
                        TPos3f inverse;
                        PSMTXInverse(_F4, inverse);
                        PSMTXConcat(inverse, viewMtx, localMtx);
                    }

                    GXLoadPosMtxImm(viewMtx, GX_PNMTX0);
                    GXLoadNrmMtxImm(viewMtx, GX_PNMTX0);
                    MR::normalize(&localMtx);
                    updateLightMtx(localMtx);
                    drawMaterialSetting(pMaterial);
                    pMaterial->mCurrentMtx.load();
                    found = true;
                    break;
                }

                pMaterial = pMaterial->mNext;
            }

            if (found) {
                break;
            }
        }

        if (found) {
            drawShape(pModel, pShape);
        }
    }
}

void NormalMapBase::drawShape(J3DModel* pModel, J3DShape* pShape) const {
    GXCallDisplayList(pShape->getVcdVatCmd(), J3DShape::kVcdVatDLSize);
    u16 groupCount = pShape->getMtxGroupNum();
    for (u16 i = 0; i < groupCount; i++) {
        if (pShape->getShapeDraw(i)) {
            J3DShapeMtx* pShapeMtx = pShape->getShapeMtx(i);
            u16 count = pShapeMtx->getUseMtxNum();
            for (u16 j = 0; j < count; j++) {
                u16 index = pShapeMtx->getUseMtxIndex(j);
                if (index != 0xFFFF) {
                    MtxPtr pMtx = pModel->getDrawMtx(index);
                    GXLoadPosMtxImm(pMtx, static_cast< u16 >(j * 3));
                    GXLoadNrmMtxImm(pMtx, static_cast< u16 >(j * 3));
                }
            }

            pShape->getShapeDraw(i)->draw();
        }
    }
}

void NormalMapBase::loadDiffuseGradTex(GXTexMapID mapID) const {
    GXTexObj obj;
    GXInitTexObj(&obj, mImagePtr, width, height, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);

    if (_13A & 1) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    } else {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }

    GXLoadTexObj(&obj, mapID);
}

void NormalMapBase::loadNormalTex(const JUTTexture* pTex, GXTexMapID id) const {
    GXTexObj obj;
    GXInitTexObj(&obj, pTex->mImage, pTex->mTIMG->mWidth, pTex->mTIMG->mHeight, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if ((_13A & 2) != 0 && (_13A & 0x4) != 0) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    } else if ((_13A & 0x2) != 0) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    } else if ((_13A & 0x4) != 0) {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_NEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    } else {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    }

    GXLoadTexObj(&obj, id);
}

void NormalMapBase::loadTexture1(J3DMaterial* pMaterial) const {
    J3DTevBlock* pBlock = pMaterial->getTevBlock();
    u32 count;
    switch (pBlock->getType()) {
    case 'TVB1':
        count = 1;
        break;
    case 'TVB2':
        count = 2;
        break;
    case 'TVB4':
        count = 4;
        break;
    case 'TV16':
        count = 8;
        break;
    }

    u16 textures[8];
    for (u32 i = 0; i < count; i++) {
        textures[i] = pBlock->getTexNo(i);
    }

    mTextures[textures[0]]->load(GX_TEXMAP2);
    if (_EC != 2) {
        loadNormalTex(mTextures[textures[1]], GX_TEXMAP0);
    }
}

void NormalMapBase::drawMaterialSetting(J3DMaterial* pMaterial) const {
    loadTexture1(pMaterial);
    loadTexMtxBtk(pMaterial);
    GXLoadTexMtxImm(pMaterial->getTexMtx(0)->getMtx(), GX_TEXMTX2, GX_MTX2x4);
    Color8 ambient;
    Color8 material;
    ambient.set(mAmbColor, mAmbColor, mAmbColor, mAmbAlpha);
    material.set(mMatColor, mMatColor, mMatColor, mMatAlpha);
    if (mUseModelLightReg & 1) {
        GXColor* pColor = pMaterial->getColorBlock()->getAmbColor(0);
        ambient.r = pColor->r;
        ambient.g = pColor->g;
        ambient.b = pColor->b;
    }

    if (mUseModelLightReg & 2) {
        ambient.a = pMaterial->getColorBlock()->getAmbColor(0)->a;
    }

    if (mUseModelLightReg & 4) {
        GXColor* pColor = pMaterial->getColorBlock()->getMatColor(0);
        material.r = pColor->r;
        material.g = pColor->g;
        material.b = pColor->b;
    }

    if (mUseModelLightReg & 8) {
        material.a = pMaterial->getColorBlock()->getMatColor(0)->a;
    }

    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    GXSetChanMatColor(GX_COLOR0A0, material);
    if (mUseModelTevReg) {
        GXColorS10* pColor0 = pMaterial->getTevBlock()->getTevColor(0);
        GXColorS10* pColor1 = pMaterial->getTevBlock()->getTevColor(1);
        GXSetTevColorS10(GX_TEVREG0, *pColor0);
        GXSetTevColorS10(GX_TEVREG1, *pColor1);
    } else {
        GXSetTevColorS10(GX_TEVREG0, mTevColor0);
        GXSetTevColorS10(GX_TEVREG1, mTevColor1);
    }
}

void NormalMapBase::drawSetting(MtxPtr pMtx) const {
    updateLightMtx(pMtx);
    Mtx scale_mtx;
    PSMTXScale(scale_mtx, 0.0f, 0.0f, 0.0f);
    MR::setMtxTrans(scale_mtx, 0.5f, 0.5f, 0.0f);
    GXLoadTexMtxImm(scale_mtx, 0x21, GX_MTX2x4);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);

    if (mHardLightColorMask) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_VTX, mHardLightColorMask, GX_DF_CLAMP, GX_AF_NONE);
    } else if (mUseModelLightReg) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
    } else {
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
    } else {
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

    GXTevColorArg lightAColor, lightBColor, lightARaster, lightBRaster;
    lightAColor = GX_CC_ONE;
    lightARaster = GX_CC_RASC;
    lightBColor = GX_CC_ONE;
    lightBRaster = GX_CC_RASC;

    if ((mHardLightColorMask & 0x1) == 0) {
        lightAColor = GX_CC_ZERO;
    }

    if ((mHardLightColorMask & 0x2) == 0) {
        lightBColor = GX_CC_ZERO;
    }

    if (mLightAMode <= 1) {
        lightAColor = GX_CC_TEXC;
    }

    if (mLightBMode == 1) {
        lightBColor = GX_CC_APREV;
    }

    GXTevAlphaArg lightBAlpha = GX_CA_RASA;

    if (mLightBMode == 2) {
        lightBAlpha = GX_CA_APREV;
    }

    if (mLightBMode == 2) {
        lightBAlpha = GX_CA_APREV;
    }

    if (mBackLightMode == 4) {
        lightARaster = GX_CC_ONE;
        lightBRaster = GX_CC_ONE;
    }

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, lightAColor, lightARaster, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, lightBColor, lightBRaster, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVREG2);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_KONST, GX_CA_ZERO, lightBAlpha, GX_CA_A0);
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

    switch (mBackLightMode) {
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

void NormalMapBase::drawSettingForDebug(MtxPtr pMtx) const {
    updateLightMtx(pMtx);
    Mtx texMtx;
    PSMTXScale(texMtx, 0.0f, 0.0f, 0.0f);
    MR::setMtxTrans(texMtx, 0.0f, 0.0f, 0.0f);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);
    GXColor ambient = {128, 128, 128, 128};
    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    GXColor material = {255, 255, 255, 255};
    GXSetChanMatColor(GX_COLOR0A0, material);
    if (_138 & 8) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    }

    GXSetChanCtrl(GX_ALPHA0, 1, GX_SRC_REG, GX_SRC_REG, 2, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    loadDiffuseGradTex(GX_TEXMAP2);
    GXSetNumTexGens(3);
    GXSetNumIndStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE2);
    if (_138 & 4) {
        GXSetNumTevStages(3);
    } else {
        GXSetNumTevStages(2);
    }

    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    if (mBtkPlayer && _150) {
        GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    } else {
        GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    }

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorS10(GX_TEVREG0, mTevColor0);
    GXSetTevColorS10(GX_TEVREG1, mTevColor1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_TEXC, GX_CC_RASC, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    if (_138 & 1) {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    } else if (_138 & 2) {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_TEXA, GX_CC_CPREV, GX_CC_ZERO);
    } else {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    }

    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    if (_138 & 4) {
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_ALPHA0);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ONE, GX_CC_ZERO, GX_CC_RASA, GX_CC_CPREV);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    }

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
}

void NormalMapBase::createGradTexture() {
    f32 step = 256.0f / width;
    for (u32 x = 0; x < width; x++) {
        s32 value = x * step;
        if (value < mLightingLowLevel) {
            value = 0;
        } else if (value >= mLightingHighLevel) {
            value = 255;
        } else {
            f32 rate = 1.0f - static_cast< f32 >(mLightingHighLevel - value) / (mLightingHighLevel - mLightingLowLevel);
            switch (mGradTexMode) {
            case 0:
                value = 255.0f * rate;
                break;
            case 1:
                value = 255.0f * (rate * rate);
                break;
            case 2:
                value = 255.0f * MR::sqrt(rate);
                break;
            }
        }

        f32 ratio = value / 255.0f;
        value = mAmbColor + ratio * (mMatColor - mAmbColor);
        for (u32 y = 0; y < height; y++) {
            static_cast< u16* >(mImagePtr)[y * width + x] = value;
        }
    }

    if (!(_138 & 1)) {
        f32 step = 256.0f / height;
        for (u32 y = 0; y < height; y++) {
            s32 value = y * step;
            if (value < mLightingLowLevel) {
                value = 0;
            } else if (value >= mLightingHighLevel) {
                value = 255;
            } else {
                f32 rate = 1.0f - static_cast< f32 >(mLightingHighLevel - value) / (mLightingHighLevel - mLightingLowLevel);
                switch (mGradTexMode) {
                case 0:
                    value = 255.0f * rate;
                    break;
                case 1:
                    value = 255.0f * (rate * rate);
                    break;
                case 2:
                    value = 255.0f * MR::sqrt(rate);
                    break;
                }
            }

            f32 ratio = value / 255.0f;
            value = mAmbColor + ratio * (mMatColor - mAmbColor);
            for (u32 x = 0; x < width; x++) {
                static_cast< u16* >(mImagePtr)[y * width + x] |= value << 8;
            }
        }
    }

    TDDraw::tileConversion16(static_cast< u16* >(mImagePtr), width, height);
    DCStoreRange(mImagePtr, width * height * 2);
}

void NormalMapBase::swapColorGB(JUTTexture* pTexture) {
    for (u32 y = 0; y < pTexture->getHeight(); y++) {
        for (u32 x = 0; x < pTexture->getWidth(); x++) {
            u32 color = TDDraw::getTexel32(pTexture, x, y);
            TVec3f normal;
            normal.set(static_cast< s8 >((color & 0xFF) - 128) / 128.0f, static_cast< s8 >(((color >> 8) & 0xFF) - 128) / 128.0f,
                       static_cast< s8 >(((color >> 16) & 0xFF) - 128) / 128.0f);
            u8 r = 128.0f + 127.0f * normal.x;
            u8 g = 128.0f + 127.0f * normal.y;
            u8 b = 128.0f + 127.0f * normal.z;
            TDDraw::setTexel32(pTexture, x, y, (g << 16) | (b << 8) | r);
        }
    }

    DCStoreRange(pTexture->mImage, pTexture->getWidth() * pTexture->getHeight() * 4);
}

void NormalMapBase::indirectCapture() const {
    TDDraw::setup(1, 0, 2);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetDither(0);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    Mtx texMtx;
    PSMTXIdentity(texMtx);
    updateLightMtx(texMtx);
    PSMTXScale(texMtx, 0.0f, 0.0f, 0.0f);
    f32 exponent = sgrad_tex_thresh - 1;
    f32 threshold = pow(2.0, exponent);
    MR::setMtxTrans(texMtx, 0.5f, 1.0f - threshold, 0.0f);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 1, GX_SRC_REG, GX_SRC_REG, 2, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    loadDiffuseGradTex(GX_TEXMAP0);
    GXSetNumTexGens(2);
    GXSetNumIndStages(1);
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndirect(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_STU, GX_ITM_0, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    u16 captured[32];
    for (u32 i = 0; i < 32; i++) {
        captured[i] = 0;
    }

    for (u16 i = 0; i < MR::getJ3DModelData(this)->getMaterialNum(); i++) {
        J3DMaterial* pMaterial = MR::getJ3DModelData(this)->getMaterialNodePointer(i);
        const char* pName = MR::getMaterialName(MR::getJ3DModelData(this), pMaterial->mIndex);
        if (isNormalMapMaterial(pName)) {
            J3DTevBlock* pBlock = pMaterial->getTevBlock();
            u32 count;
            switch (pBlock->getType()) {
            case 'TVB1':
                count = 1;
                break;
            case 'TVB2':
                count = 2;
                break;
            case 'TVB4':
                count = 4;
                break;
            case 'TV16':
                count = 8;
                break;
            }

            u16 textures[8];
            for (u32 j = 0; j < count; j++) {
                textures[j] = pBlock->getTexNo(j);
            }

            if (!captured[textures[1]]) {
                JUTTexture* pTexture = mTextures[textures[1]];
                loadNormalTex(pTexture, GX_TEXMAP1);
                s32 screenWidth = JUTVideo::getManager()->getFbWidth();
                f32 scaleX = static_cast< f32 >(MR::getScreenWidth()) / screenWidth;
                s32 screenHeight = MR::getScreenHeight();
                f32 scaleY = static_cast< f32 >(MR::getScreenHeight()) / screenHeight;
                TVec3f position(0.0f, 0.0f, 0.0f);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(position.x, position.y, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(position.x + scaleX * pTexture->getWidth(), position.y, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(position.x + scaleX * pTexture->getWidth(), position.y + scaleY * pTexture->getHeight(), 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(position.x, position.y + scaleY * pTexture->getHeight(), 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                GXEnd();
                _4B8->capture(0, 0, GX_CTF_RA8, false, 0);
                break;
            }
        }
    }
}

void NormalMapBase::loadDirectLightTex(GXTexMapID id) const {
    GXTexObj obj;
    GXInitTexObj(&obj, _4B8->mImage, _4B8->mTIMG->mWidth, _4B8->mTIMG->mHeight, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (_13A & 1) {
        GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    } else {
        GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }

    GXLoadTexObj(&obj, id);
}

void NormalMapBase::drawSettingForCapture(MtxPtr pMtx) const {
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);
    GXColor ambient = {128, 128, 128, 128};
    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    GXColor material = {255, 255, 255, 255};
    GXSetChanMatColor(GX_COLOR0A0, material);
    if (_138 & 8) {
        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    }

    GXSetChanCtrl(GX_ALPHA0, 1, GX_SRC_REG, GX_SRC_REG, 2, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    loadDirectLightTex(GX_TEXMAP1);
    GXSetNumTexGens(2);
    GXSetNumIndStages(0);
    GXSetNumTevStages(2);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorS10(GX_TEVREG0, mTevColor0);
    GXSetTevColorS10(GX_TEVREG1, mTevColor1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_TEXC, GX_CC_RASC, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXTevColorArg alpha = GX_CC_TEXA;
    switch (_E8) {
    case 0:
        alpha = GX_CC_TEXA;
        break;
    case 1:
        alpha = GX_CC_ZERO;
        break;
    case 2:
        alpha = GX_CC_ONE;
        break;
    }

    if (_138 & 1) {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, alpha, GX_CC_TEXC);
    } else if (_138 & 2) {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, alpha, GX_CC_CPREV, GX_CC_ZERO);
    } else {
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, alpha, GX_CC_ZERO);
    }

    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
}

void NormalMapBase::loadData(const char* pData, u32 size) {
    char line[256];
    const char* pCursor = pData;
    u32 offset = 0;
    while (true) {
        if (offset >= size) {
            break;
        }

        u32 length = 0;
        do {
            if (*pCursor == '\n' || *pCursor == '\r') {
                break;
            }

            offset++;
            line[length] = *pCursor;
            length++;
            pCursor++;
        } while (offset < size);
        line[length] = '\0';
        while (true) {
            if (offset >= size) {
                break;
            }

            if (*pCursor != '\n' && *pCursor != '\r') {
                break;
            }

            pCursor++;
            offset++;
        }

        MR::scan8(line, "mLightingHighLevel", &mLightingHighLevel);
        MR::scan8(line, "mLightingLowLevel", &mLightingLowLevel);
        MR::scan8(line, "mLightAMode", &mLightAMode);
        MR::scan8(line, "mLightBMode", &mLightBMode);
        MR::scan16(line, "mUseModelLightReg", &mUseModelLightReg);
        MR::scan16(line, "mUseModelTevReg", &mUseModelTevReg);
        MR::scan16(line, "mHardLightColorMask", &mHardLightColorMask);
        MR::scan16(line, "mHardLightAlphaMask", &mHardLightAlphaMask);
        MR::scan8(line, "mBackLightMode", &mBackLightMode);
        MR::scan32(line, "mGradTexMode", &mGradTexMode);
        MR::scan8(line, "mAmbColor", &mAmbColor);
        MR::scan8(line, "mMatColor", &mMatColor);
        MR::scan8(line, "mAmbAlpha", &mAmbAlpha);
        MR::scan8(line, "mMatAlpha", &mMatAlpha);
        MR::scans16x4(line, "mTevColor0", reinterpret_cast< s16* >(&mTevColor0));
        MR::scans16x4(line, "mTevColor1", reinterpret_cast< s16* >(&mTevColor1));
    }
}

NormalMapBase::~NormalMapBase() {
}
