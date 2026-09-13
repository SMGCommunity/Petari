#include "Game/Util/FurMulti.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/FurCtrl.hpp"
#include "Game/Util/FurDrawer.hpp"
#include "Game/Util/FurParam.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <cstring>

namespace {
    FurParam sFurParam = {6,
                          10.0f,
                          0.778809f,
                          0.0f,
                          1.0f,
                          0.0f,
                          0.0f,
                          0.6f,
                          255.0f,
                          0.0f,
                          1.0f,
                          80.0f,
                          0.0f,
                          5.419921875f,
                          16.2109375f,
                          1.0f,
                          {54, 48, 44, 0},
                          {0.8f, 0.6f, 0.3f, 0.09f},
                          {0.517f, 0.386f, 0.3645f, 0.1713f},
                          {26, 64, 98, 212}};
}

FurMulti::FurMulti(LiveActor* pActor, u32 count) {
    mActor = pActor;
    mModel = MR::getJ3DModel(pActor);
    mCount = count;
    mShapeIndices = new u8[count];
    mDrawOrders = new u8[count];
    mFurCtrls = new FurCtrl*[count];

    for (u32 i = 0; i < count; i++) {
        mDrawOrders[i] = 255;
    }

    mAddedToManager = 0;
    _1 = 1;
}

void FurMulti::setLayerDirect(u32 layer, u32 drawOrder, u32 shape, u32 numLayers, FurParam* pParam, ResTIMG* pBody, ResTIMG* pLength,
                              ResTIMG* pIndirect, ResTIMG* pDensity) {
    mShapeIndices[layer] = shape;
    mDrawOrders[layer] = drawOrder;
    mFurCtrls[layer] = new FurCtrl(mActor, pParam, false, 1);
    J3DModelData* pModelData = mModel->getModelData();
    FurMulti* pClone = MR::getFurDrawManager()->getFurBank()->check(pModelData, layer);

    if (pClone) {
        mFurCtrls[layer]->setupFurClone(mModel, pClone->mFurCtrls[layer]);
        return;
    }

    mFurCtrls[layer]->setupFur(mModel, pLength, pIndirect, pBody, shape, numLayers);
    MR::getFurDrawManager()->mBank->regist(this, layer);
    mFurCtrls[layer]->calcLayerForm();

    if (pDensity) {
        pDensity->mFormat = GX_TF_IA8;
        pDensity->mWidth = 32;
        pDensity->mHeight = 32;
        pDensity->mWrapS = GX_REPEAT;
        pDensity->mWrapT = GX_REPEAT;
        pDensity->mPaletteName = GX_TLUT0;
        pDensity->mPaletteFormat = GX_TL_IA8;
        pDensity->mPaletteNum = 0;
        pDensity->mPaletteDataOffset = 0;
        pDensity->mMipmap = false;
        pDensity->mDoEdgeLod = false;
        pDensity->mBiasClamp = false;
        pDensity->mMaxAnisotropy = GX_ANISO_1;
        pDensity->mMinType = GX_LINEAR;
        pDensity->mMagType = GX_LINEAR;
        pDensity->mMinLod = 0;
        pDensity->mMaxLod = 0;
        pDensity->mImageNum = 1;
        pDensity->mLodBias = 0;
        pDensity->mImageDataOffset = sizeof(ResTIMG);
        JUTTexture* pTexture = new JUTTexture(pDensity, 0);
        FurDrawer* pDrawer = mFurCtrls[layer]->mDrawer;
        pDrawer->mFurTexture = pTexture;
        pDrawer->mUseDensityMap = 1;
    } else {
        mFurCtrls[layer]->createFurMap();
    }

    mFurCtrls[layer]->mDrawer->update();
}

void FurMulti::addToManager() {
    for (u32 i = 0; i < mCount; i++) {
        MR::getFurDrawManager()->add(mFurCtrls[i], 1);
    }

    mAddedToManager = 1;
}

void FurMulti::offDraw(u32 mask) {
    for (u32 i = 0; i < mCount; i++) {
        if (mask & (1 << i)) {
            mFurCtrls[i]->mVisible = 0;
        }
    }
}

void FurMulti::onDraw(u32 mask) {
    for (u32 i = 0; i < mCount; i++) {
        if (mask & (1 << i)) {
            mFurCtrls[i]->mVisible = 1;
        }
    }
}

namespace MR {
    FurMulti* initMultiFur(LiveActor* pActor, s32 lightType) {
        u16 i;
        u16 count;
        u16 shape;
        FurMulti* pMulti;
        CurrentHeapRestorer heapRestorer(getSceneHeapGDDR3());
        J3DModelData* pModelData = getJ3DModelData(pActor);
        count = 0;

        for (i = 0; i < pModelData->getMaterialNum(); i++) {
            if (strstr(pModelData->getMaterialName()->getName(i), "Fur")) {
                count++;
            }
        }

        if (count == 0) {
            return nullptr;
        }

        pMulti = new FurMulti(pActor, count);
        DynamicFurParam dynamicParam;
        dynamicParam.mFogCtrl = new FogCtrl(pModelData, true);
        dynamicParam.mLightParam = new FurLightParam;
        dynamicParam.mLightParam->mLightType = lightType;
        u32 layer = 0;

        for (u16 i = 0; i < pModelData->getMaterialNum(); i++) {
            if (strstr(pModelData->getMaterialName()->getName(i), "Fur")) {
                shape = pModelData->getMaterialNodePointer(i)->getShape()->getIndex();
                char name[256];
                strcpy(name, pModelData->getMaterialName()->getName(i));
                strcat(name, "Body");
                ResTIMG* pBody;

                if (isExistTexture(pActor, name)) {
                    pBody = getTexFromModel(name, pActor);
                } else {
                    pBody = getResTIMG(pActor, 0);
                }

                strcpy(name, pModelData->getMaterialName()->getName(i));
                strcat(name, "Length.bti");
                ResTIMG* pLength = nullptr;

                if (getResourceHolder(pActor)->mFileInfoTable->isExistRes(name)) {
                    pLength = static_cast< ResTIMG* >(getResourceHolder(pActor)->mFileInfoTable->getRes(name));
                }

                strcpy(name, pModelData->getMaterialName()->getName(i));
                strcat(name, "Indirect.bti");
                ResTIMG* pIndirect = nullptr;

                if (getResourceHolder(pActor)->mFileInfoTable->isExistRes(name)) {
                    pIndirect = static_cast< ResTIMG* >(getResourceHolder(pActor)->mFileInfoTable->getRes(name));
                }

                strcpy(name, pModelData->getMaterialName()->getName(i));
                strcat(name, "Density.bti");
                ResTIMG* pDensity = nullptr;

                if (getResourceHolder(pActor)->mFileInfoTable->isExistRes(name)) {
                    pDensity = static_cast< ResTIMG* >(getResourceHolder(pActor)->mFileInfoTable->getRes(name));
                }

                FurParam* pParam = new FurParam;
                *pParam = sFurParam;
                strcpy(name, pModelData->getMaterialName()->getName(i));
                strcat(name, ".fur.txt");

                if (getResourceHolder(pActor)->mFileInfoTable->isExistRes(name)) {
                    char* pText = static_cast< char* >(getResourceHolder(pActor)->mFileInfoTable->getRes(name));
                    ResFileInfo* pInfo = getResourceHolder(pActor)->mFileInfoTable->findFileInfo(name);
                    initFurParamFromDvd(pParam, &dynamicParam, pText, pInfo->_4);
                }

                pMulti->setLayerDirect(layer, 0, shape, pParam->mNumLayers, pParam, pBody, pLength, pIndirect, pDensity);
                pMulti->mFurCtrls[layer]->mDynamicParam.mLightParam = dynamicParam.mLightParam;
                pMulti->mFurCtrls[layer]->mDynamicParam.mFogCtrl = dynamicParam.mFogCtrl;
                layer++;
            }
        }

        pMulti->addToManager();
        return pMulti;
    }
}  // namespace MR

void FurBank::regist(FurMulti* pMulti, u32 layer) {
    mEntries[mCount] = pMulti;
    mLayerMasks[mCount] |= 1 << layer;
    mCount++;
}
