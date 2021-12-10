#include "Game/Util/ModelUtil.h"

#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/J3DGraphBase/J3DShapePacket.h"
#include "JSystem/JUtility/JUTNameTab.h"

namespace MR {
    u32 getMaterialNo(J3DModelData *pModelData, const char *pMaterialName) {
        return pModelData->mMaterialTable._C->getIndex(pMaterialName);
    }

    u32 getMaterialNo(J3DModel *pModel, const char *pMaterialName) {
        return pModel->mModelData->mMaterialTable._C->getIndex(pMaterialName);
    }

    J3DMaterial* getMaterial(J3DModelData *pModelData, const char *pMaterialName) {
        u16 idx = pModelData->mMaterialTable._C->getIndex(pMaterialName);
        return pModelData->mMaterialTable._8[idx];
    }

    J3DMaterial* getMaterial(J3DModelData *pModelData, int idx) {
        return pModelData->mMaterialTable._8[(u16)idx];
    }

    J3DMaterial* getMaterial(J3DModel* pModel, int idx) {
        return pModel->mModelData->mMaterialTable._8[(u16)idx];
    }

    // getMaterial(const LiveActor *, int)

    u16 getMaterialNum(J3DModel *pModel) {
        return pModel->mModelData->mMaterialTable._4;
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