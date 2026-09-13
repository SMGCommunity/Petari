#include "Game/Util/FurCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/FurDrawer.hpp"
#include "Game/Util/FurMulti.hpp"
#include "Game/Util/FurShader.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>

class J3DModel2 : public J3DModel {
public:
    J3DModel2(J3DModel* pModel);

    virtual ~J3DModel2() {
    }

    void setVtxShader(J3DVtxShader* pShader, s32 componentSize) {
        mUnkCalc1 = pShader;

        if (pShader) {
            mUnkCalc1->setup(getModelData());

            if (!mVertexBuffer.mTransformedVtxPosArray[0] || !mVertexBuffer.mTransformedVtxPosArray[1]) {
                u32 size = (componentSize * 3 * getVertexBuffer()->getVertexData()->getVtxNum() + 31) & ~31;
                mVertexBuffer.mTransformedVtxPosArray[0] = new (32) u8[size];

                if (mVertexBuffer.mTransformedVtxPosArray[0]) {
                    mVertexBuffer.mTransformedVtxPosArray[1] = mVertexBuffer.mTransformedVtxPosArray[0];
                }
            }

            for (s32 j = 0; j < 2; j++) {
                memcpy(mVertexBuffer.mTransformedVtxPosArray[j], getVertexBuffer()->getVertexData()->getVtxPosArray(),
                       componentSize * getVertexBuffer()->getVertexData()->getVtxNum() * 3);
                DCStoreRange(mVertexBuffer.mTransformedVtxPosArray[j], componentSize * 3 * getVertexBuffer()->getVertexData()->getVtxNum());
            }
        }
    }
};

FurMulti* FurBank::check(J3DModelData* pModelData, u32 layer) {
    for (u32 i = 0; i < mCount; i++) {
        if (mEntries[i]->mModel->getModelData() == pModelData && ((1 << layer) & mLayerMasks[i])) {
            return mEntries[i];
        }
    }

    return nullptr;
}

void FurDrawManager::add(FurCtrl* pCtrl, u8 index) {
    if (mCapacity != mCounts[index]) {
        mFurCtrls[index][mCounts[index]] = pCtrl;
        mCounts[index]++;
    }
}

void FurDrawManager::draw() const {
    for (u32 i = 0; i < mCounts[1]; i++) {
        if (!MR::isClipped(mFurCtrls[1][i]->mActor) && !MR::isDead(mFurCtrls[1][i]->mActor) && !MR::isHiddenModel(mFurCtrls[1][i]->mActor)) {
            if (mFurCtrls[1][i]->mVisible == 1) {
                mFurCtrls[1][i]->drawFur();
            }
        }
    }

    GXSetClipMode(GX_CLIP_ENABLE);
}

FurCtrl::FurCtrl(LiveActor* pActor, FurParam* pParam, bool addToManager, u8 drawOrder) : mParam(pParam), mVisible(), mFurUVScale(1.0f) {
    if (addToManager) {
        MR::getFurDrawManager()->add(this, drawOrder);
    }

    mActor = pActor;
    mModel = nullptr;
    mShapeIndex = -1;
    mNumLayers = 0;
    mDrawer = nullptr;
    mShader = nullptr;
    mLayerModels = nullptr;
    mLengthMap = nullptr;
    mBodyMap = nullptr;
    mIndirectMap = nullptr;
    mIsClone = 0;
    mDynamicParam.mFogCtrl = nullptr;
    mDynamicParam.mLightParam = nullptr;
}

void FurCtrl::calcLayerForm() {
    mDrawer->mFurUVScale = mParam->mFurUVScale;
    mDrawer->mBodyUVScale = mParam->mBodyUVScale;
    mDrawer->mLength.mTip = mParam->mLength;
    mDrawer->mLength.mRoot = 0.0f;
    mDrawer->mLength.mCurve = mParam->mLengthCurve;
    mDrawer->mNumLayers = mParam->mNumLayers;

    if (mDrawer->mNumLayers > mNumLayers) {
        mDrawer->mNumLayers = mNumLayers;
    }

    for (s32 i = 0; i < mDrawer->mNumLayers; i++) {
        f32 length = mDrawer->mLength.calcValue(i, mDrawer->mNumLayers);
        mShader->_8 = pow(1.0f * i / mDrawer->mNumLayers, 4.0);
        mLayerModels[i]->mVertexBuffer.frameInit();
        mShader->_1C = length;
        mShader->calc(mLayerModels[i]);
    }
}

void FurCtrl::drawFur() {
    J3DModelData* pModelData = mModel->getModelData();

    if (MR::getJ3DModel(mActor)->getShapePacket(mShapeIndex)->checkFlag(0x10)) {
        return;
    }

    if (pModelData->getShapeNodePointer(mShapeIndex)->checkFlag(1)) {
        return;
    }

    if (mDynamicParam.mLightParam->mLightType == -1) {
        if (MR::getLightCtrl(mActor)) {
            MR::loadActorLight(mActor);
        }
    } else {
        MR::loadLight(mDynamicParam.mLightParam->mLightType);
    }

    mDrawer->mFurUVScale = mParam->mFurUVScale * mFurUVScale;
    mDrawer->mBodyUVScale = mParam->mBodyUVScale;
    mDrawer->mColor = mParam->mColor;
    mDrawer->mIndirect.mTip = mParam->mIndirect;
    mDrawer->mIndirect.mRoot = 0.0f;
    mDrawer->mIndirect.mCurve = mParam->mIndirectCurve;
    mDrawer->mBrightness.mTip = mParam->mBrightnessTip;
    mDrawer->mBrightness.mRoot = mParam->mBrightnessRoot;
    mDrawer->mBrightness.mCurve = mParam->mBrightnessCurve;
    mDrawer->mAlpha.mTip = mParam->mAlphaTip;
    mDrawer->mAlpha.mRoot = mParam->mAlphaRoot;
    mDrawer->mAlpha.mCurve = mParam->mAlphaCurve;
    mDrawer->mColorBlend.mTip = mParam->mSkinAlphaTip;
    mDrawer->mColorBlend.mRoot = mParam->mSkinAlphaRoot;
    mDrawer->mColorBlend.mCurve = mParam->mSkinAlphaCurve;
    mDrawer->update();
    mDrawer->setupMaterial(&mDynamicParam);

    if (mDrawer->mMixFog) {
        mDrawer->mMixFog--;
    }

    J3DShape* pShape = pModelData->getShapeNodePointer(mShapeIndex);
    pShape->mCurrentMtx.mMtxIdxRegA = 0x3C921780;
    pShape->mCurrentMtx.mMtxIdxRegB = 0x00F3CF3C;
    J3DShape::sOldVcdVatCmd = nullptr;
    J3DShapePacket* pPacket;

    for (s32 i = 0; i < mDrawer->mNumLayers; i++) {
        mDrawer->setupLayerMaterial(i);
        pPacket = &mLayerModels[i]->mShapePacket[mShapeIndex];
        pPacket->setModel(mLayerModels[i]);
        pPacket->prepareDraw();
        pPacket->getShape()->drawFast();
    }

    mModel->mShapePacket[mShapeIndex].setModel(mModel);
    GXColor fogColor = {0, 0, 0, 0};
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fogColor);
    J3DShape::sOldVcdVatCmd = nullptr;
    GXSetClipMode(GX_CLIP_ENABLE);
}

void FurCtrl::createFurMap() {
    if (mIsClone) {
        return;
    }

    for (u32 i = 0; i < 4; i++) {
        mDrawer->mDensity[i] = mParam->mDensity[i];
        mDrawer->mThickness[i] = mParam->mThickness[i];
        mDrawer->mLengthMap[i] = mParam->mLengthMap[i];
    }

    mDrawer->createFurMap();
}

namespace MR {
    FurDrawManager* getFurDrawManager() {
        return static_cast< FurDrawManager* >(getSceneObjHolder()->getObj(SceneObj_FurDrawManager));
    }
}  // namespace MR

FurDrawManager::FurDrawManager(u8 capacity) : NameObj("ファー描画マネージャ") {
    mCounts[0] = 0;
    mCounts[1] = 0;
    mFurCtrls[0] = new FurCtrl*[capacity];
    mFurCtrls[1] = new FurCtrl*[capacity];
    mCapacity = capacity;
    mBank = new FurBank;
    MR::connectToScene(this, -1, -1, -1, 0x31);
}

void FurCtrl::setupFur(J3DModel* pModel, ResTIMG* pLength, ResTIMG* pIndirect, ResTIMG* pBody, u16 shape, u8 numLayers) {
    J3DModelData* pData = pModel->getModelData();
    mModel = pModel;
    mLengthMap = pLength;
    mBodyMap = pBody;
    mIndirectMap = pIndirect;
    mShapeIndex = shape;
    mShader = new CShader(pData, pLength);
    mShader->makeIndexData(pData->getShapeNodePointer(shape));
    mShader->checkBorderVtx(pData, shape);
    mDrawer = new FurDrawer(numLayers, pBody, pIndirect);
    s32 componentSize;

    switch (mShader->_24) {
    case GX_S16:
        componentSize = sizeof(s16);
        break;
    case GX_F32:
        componentSize = sizeof(f32);
        break;
    }

    mNumLayers = numLayers;
    mLayerModels = new J3DModel*[numLayers];

    for (s32 i = 0; i < mNumLayers; i++) {
        char name[16];
        sprintf(name, "レイヤ %d", i);
        J3DModel2* pLayer = new J3DModel2(mModel);
        pLayer->setVtxShader(mShader, componentSize);
        mLayerModels[i] = pLayer;
    }

    mVisible = 1;
}

void FurCtrl::setupFurClone(J3DModel* pModel, FurCtrl* pOther) {
    mModel = pModel;
    mIsClone = 1;
    mLengthMap = pOther->mLengthMap;
    mBodyMap = pOther->mBodyMap;
    mIndirectMap = pOther->mIndirectMap;
    mShapeIndex = pOther->mShapeIndex;
    mShader = nullptr;
    mDrawer = pOther->mDrawer;
    mNumLayers = pOther->mNumLayers;
    mLayerModels = new J3DModel*[mNumLayers];

    for (u32 i = 0; i < mNumLayers; i++) {
        char name[32];
        sprintf(name, "レイヤ(コピー) %d", i);
        J3DModel2* pLayer = new J3DModel2(mModel);
        *pLayer->getVertexBuffer() = *pOther->mLayerModels[i]->getVertexBuffer();
        mLayerModels[i] = pLayer;
    }

    mVisible = 1;
}

namespace MR {
    void initFurParamFromDvd(FurParam* pParam, DynamicFurParam* pDynamic, char* pText, u32 size) {
        char line[256];
        FurLightParam* pLight = pDynamic->mLightParam;
        s32 lightMask = 0;
        s32 matIntensity = 0;
        s32 ambIntensity = 0;
        s32 specularLightMask = 0;
        s32 specularMatIntensity = 0;
        s32 specularAmbIntensity = 0;
        s32 colorSource = 0;
        char* pRead = pText;
        u32 position = 0;

        while (true) {
            if (position >= size) {
                break;
            }

            u32 length = 0;

            while (true) {
                if (*pRead == '\n' || *pRead == '\r') {
                    break;
                }

                position++;
                line[length] = *pRead;
                length++;
                pRead++;

                if (position >= size) {
                    break;
                }
            }

            line[length] = '\0';

            while (true) {
                if (position >= size) {
                    break;
                }

                if (*pRead != '\n' && *pRead != '\r') {
                    break;
                }

                pRead++;
                position++;
            }

            scan32(line, "レイヤ数", &pParam->mNumLayers);
            scanf32(line, "毛長さ", &pParam->mLength);
            scanf32(line, "長さ偏差", &pParam->mLengthCurve);
            scanf32(line, "ズレ(indirect)", &pParam->mIndirect);
            scanf32(line, "ズレ偏差", &pParam->mIndirectCurve);
            scanf32(line, "明るさ(毛先)", &pParam->mBrightnessTip);
            scanf32(line, "明るさ(毛元)", &pParam->mBrightnessRoot);
            scanf32(line, "明るさ偏差", &pParam->mBrightnessCurve);
            scanf32(line, "透明度(毛先)", &pParam->mAlphaTip);
            scanf32(line, "透明度(毛元)", &pParam->mAlphaRoot);
            scanf32(line, "透明度偏差", &pParam->mAlphaCurve);
            scanf32(line, "透明度・地肌(毛先)", &pParam->mSkinAlphaTip);
            scanf32(line, "透明度・地肌(毛元)", &pParam->mSkinAlphaRoot);
            scanf32(line, "透明度・地肌偏差", &pParam->mSkinAlphaCurve);
            scanf32(line, "密度マップスケール", &pParam->mFurUVScale);
            scanf32(line, "ベースマップスケール", &pParam->mBodyUVScale);
            scanu8x4(line, "混合カラー", &pParam->mColor.r);
            scanf32x4(line, "植毛密度", pParam->mDensity);
            scanf32x4(line, "植毛太さ", pParam->mThickness);
            scanu8x4(line, "混合比", pParam->mLengthMap);
            scan32(line, "ライト0スイッチ", &lightMask);
            scan32(line, "ライト0マテリアル", &matIntensity);
            scan32(line, "ライト0アンビエント", &ambIntensity);
            scan32(line, "ライト1スイッチ", &specularLightMask);
            scan32(line, "ライト1マテリアル", &specularMatIntensity);
            scan32(line, "ライト1アンビエント", &specularAmbIntensity);
            scan32(line, "ライトカラーソース", &colorSource);
            pLight->mLightMask = lightMask;
            pLight->mMatIntensity = matIntensity;
            pLight->mAmbIntensity = ambIntensity;
            pLight->mSpecularLightMask = specularLightMask;
            pLight->mSpecularMatIntensity = specularMatIntensity;
            pLight->mSpecularAmbIntensity = specularAmbIntensity;
            pLight->mColorSource = colorSource;
        }
    }
}  // namespace MR

J3DModel2::J3DModel2(J3DModel* pModel) {
    mModelData = pModel->mModelData;
    mMtxBuffer = pModel->mMtxBuffer;
    mShapePacket = pModel->mShapePacket;
    mMatPacket = pModel->mMatPacket;
    mVertexBuffer.setVertexData(&mModelData->mVertexData);
    mFlags |= J3DMdlFlag_UseDefaultJ3D;
}

FurDrawManager::~FurDrawManager() {
}
