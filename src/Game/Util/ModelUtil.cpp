#include "Game/Util/ModelUtil.hpp"

#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DPacket.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"

namespace MR {
    u32 getMaterialNo(J3DModelData* pModelData, const char* pMaterialName) {
        return pModelData->mMaterialTable.mMaterialName->getIndex(pMaterialName);
    }

    u32 getMaterialNo(J3DModel* pModel, const char* pMaterialName) {
        return pModel->mModelData->mMaterialTable.mMaterialName->getIndex(pMaterialName);
    }

    J3DMaterial* getMaterial(J3DModelData* pModelData, const char* pMaterialName) {
        u16 idx = pModelData->mMaterialTable.mMaterialName->getIndex(pMaterialName);
        return pModelData->mMaterialTable.getMaterialNodePointer(idx);
    }

    J3DMaterial* getMaterial(J3DModelData* pModelData, int idx) {
        return pModelData->mMaterialTable.getMaterialNodePointer(idx);
    }

    J3DMaterial* getMaterial(J3DModel* pModel, int idx) {
        return pModel->mModelData->mMaterialTable.getMaterialNodePointer(idx);
    }

    // getMaterial(const LiveActor *, int)

    u16 getMaterialNum(J3DModel* pModel) {
        return pModel->mModelData->mMaterialTable.getMaterialNum();
    }

    // updateModelDiffDL
    // isEnvelope

    // some inlining issue
    void hideMaterial(J3DModel* pModel, const char* pMaterialName) {
        J3DShapePacket* pckt = pModel->mMatPacket[getMaterialNo(pModel, pMaterialName)].mpShapePacket;
        pckt->mFlags |= 0x10;
    }
};  // namespace MR
