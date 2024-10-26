#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util.hpp"
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>

MaterialCtrl::MaterialCtrl(J3DModelData *pModelData, const char *pMaterialName) {
    mModelData = pModelData;
    mMaterial = nullptr;

    if (pMaterialName) {
        mMaterial = MR::getMaterial(pModelData, pMaterialName);
    }
}

void MaterialCtrl::update() {
    if (mMaterial) {
        updateMaterial(mMaterial);
    }
    else {
        u16 i = 0;
        while (i < mModelData->mMaterialTable.mMaterialCount) {
            updateMaterial(mModelData->mMaterialTable.mMaterials[i]);
            i++;
        }
    }
}

FogCtrl::FogCtrl(J3DModelData *pModelData, bool a3) : MaterialCtrl(pModelData, nullptr) {
    mNumMaterials = 0;
    mMaterials = nullptr;

    J3DMaterial* nextMat = nullptr;
    for (u16 i = 0; i < pModelData->getMaterialCount(); i++) {
        J3DMaterial* mat = pModelData->getMaterial(i);
        if (a3 || mat->mPEBlock->getFog().mFogType) {
            if (nextMat == nullptr) {
                nextMat = mat;
            }

            mNumMaterials++;
        }
    }

    if (nextMat == nullptr) {
        nextMat = *pModelData->mMaterialTable.mMaterials;
    }

    mFogInfo = nextMat->mPEBlock->getFog();

    if (mNumMaterials > 0) {
        mMaterials = new J3DMaterial*[mNumMaterials];
        s32 curMaterial = 0;

        for (u16 i = 0; i < pModelData->getMaterialCount(); i++) {
            J3DMaterial* mat = pModelData->getMaterial(i);
            if (a3 || mat->mPEBlock->getFog().mFogType) {
                mMaterials[curMaterial++] = mat;
            }
        }
    }
}

#ifdef NON_MATCHING
void FogCtrl::update() {
    for (s32 i = 0; i < mNumMaterials; i++) {
        J3DPEBlockFull& block = *(J3DPEBlockFull*)mMaterials[i]->mPEBlock;
        mMaterials[i]->mPEBlock->getFog() = block.mFogInfo;
    }
}
#endif

MatColorCtrl::MatColorCtrl(J3DModelData *pModelData, const char *pName, u32 color, const J3DGXColor *pColor) : MaterialCtrl(pModelData, pName) {
    mColorChoice = color;
    mColor = pColor; 
} 

void MatColorCtrl::updateMaterial(J3DMaterial *pMaterial) {
    pMaterial->mColorBlock->setMatColor(mColorChoice, mColor);
}

