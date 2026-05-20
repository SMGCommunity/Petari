#include "Game/Util/ModelUtil.hpp"

#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include "JSystem/J3DGraphBase/J3DPacket.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"

namespace {
    bool isUseLightChanNo(J3DMaterial* pMaterial, int channel, int index) {
        u16 channelID = pMaterial->mColorBlock->getColorChan(channel)->mColorChanID;
        if (((channelID >> 1) & 0x1) == 0) {
            return false;
        }

        u8 useLightChan = (channelID >> 2 & 0xF | channelID >> 7 & 0xF0);
        return useLightChan & (1 << index);
    }
}  // namespace

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

    s32 getMaterialNum(J3DModel* pModel) {
        return pModel->mModelData->mMaterialTable.getMaterialNum();
    }

    // updateModelDiffDL
    // isEnvelope

    // some inlining issue
    void hideMaterial(J3DModel* pModel, const char* pMaterialName) {
        J3DShapePacket* pckt = pModel->mMatPacket[getMaterialNo(pModel, pMaterialName)].mpShapePacket;
        pckt->mFlags |= 0x10;
    }

    void getLightNum(J3DMaterial* pMaterial, s32* pChan1, s32* pChan2, s32* pChan3, s32* pChan4) {
        for (int i = 0; i < 8; i++) {
            if (isUseLightChanNo(pMaterial, 0, i)) {
                *pChan1 = *pChan1 + 1;
            }

            if (isUseLightChanNo(pMaterial, 1, i)) {
                *pChan2 = *pChan2 + 1;
            }
        }

        for (int i = 0; i < 8; i++) {
            if (isUseLightChanNo(pMaterial, 2, i)) {
                *pChan3 = *pChan3 + 1;
            }

            if (isUseLightChanNo(pMaterial, 3, i)) {
                *pChan4 = *pChan4 + 1;
            }
        }
    }

    s32 getLightNum(J3DMaterial* pMaterial) {
        s32 chan1 = 0;
        s32 chan2 = 0;
        s32 chan3 = 0;
        s32 chan4 = 0;
        getLightNum(pMaterial, &chan1, &chan2, &chan3, &chan4);
        return chan1 + chan2 + chan3 + chan4;
    }

    s32 getLightNumMax(const LiveActor* pActor) {
        return getLightNumMax(getJ3DModelData(pActor));
    }

    s32 getLightNumMax(J3DModelData* pModelData) {
        int maxNum = 0;
        for (int i = 0; i < pModelData->mMaterialTable.mMaterialNum; i++) {
            int currentNum = getLightNum(pModelData->mMaterialTable.mMaterialNodePointer[static_cast< u16 >(i)]);

            if (maxNum < currentNum) {
                maxNum = currentNum;
            }
        }

        return maxNum;
    }
};  // namespace MR
