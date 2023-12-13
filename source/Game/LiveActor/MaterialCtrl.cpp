#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util.hpp"

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