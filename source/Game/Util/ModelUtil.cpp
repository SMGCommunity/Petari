#include "Game/Util/ModelUtil.hpp"

#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp"
#include "JSystem/J3DGraphBase/J3DShapePacket.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"

namespace MR {
    u32 getMaterialNo(J3DModelData *pModelData, const char *pMaterialName) {
        return pModelData->mMaterialTable.mNameTable->getIndex(pMaterialName);
    }

    u32 getMaterialNo(J3DModel *pModel, const char *pMaterialName) {
        return pModel->mModelData->mMaterialTable.mNameTable->getIndex(pMaterialName);
    }

    J3DMaterial* getMaterial(J3DModelData *pModelData, const char *pMaterialName) {
        u16 idx = pModelData->mMaterialTable.mNameTable->getIndex(pMaterialName);
        return pModelData->mMaterialTable.mMaterials[idx];
    }

    J3DMaterial* getMaterial(J3DModelData *pModelData, int idx) {
        return pModelData->mMaterialTable.mMaterials[(u16)idx];
    }

    J3DMaterial* getMaterial(J3DModel* pModel, int idx) {
        return pModel->mModelData->mMaterialTable.mMaterials[(u16)idx];
    }

    // getMaterial(const LiveActor *, int)

    u16 getMaterialNum(J3DModel *pModel) {
        return pModel->mModelData->mMaterialTable.mMaterialCount;
    }

    // updateModelDiffDL
    // isEnvelope

    #ifdef NON_MATCHING
    // some inlining issue
    void hideMaterial(J3DModel *pModel, const char *pMaterialName) {
        J3DShapePacket* pckt = pModel->mMaterialPacket[getMaterialNo(pModel, pMaterialName)]._2C;
        pckt->mFlags |= 0x10;
    }
    #endif
};