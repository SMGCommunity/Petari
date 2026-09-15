#include "Game/Animation/MaterialAnmBuffer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphBase/J3DPacket.hpp>

template < typename T >
void modifyDiffFlag(u32* pFlags, const T* pAnm, J3DDiffFlag flag, bool enable, const char* pName, const char* pType) {
    for (u16 i = 0; i < pAnm->getUpdateMaterialNum(); i++) {
        u16 material = pAnm->getUpdateMaterialID(i);

        if (material != 0xFFFF) {
            if (enable) {
                pFlags[material] |= flag;
            } else {
                pFlags[material] &= ~flag;
            }
        }
    }
}

void modifyDiffFlagBrk(u32*, const J3DAnmTevRegKey*, bool, const char*);

MaterialAnmBuffer::MaterialAnmBuffer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData, bool useDiffFlag) : _0(), _4() {
    searchUpdateMaterialID(pResourceHolder, pModelData);
    u16 materialNum = pModelData->getMaterialNum();

    if (useDiffFlag) {
        _4 = new u32[materialNum];
        MR::zeroMemory(_4, sizeof(u32) * materialNum);
        setDiffFlag(pResourceHolder);
    }

    u16 anmNum = getAllocMaterialAnmNum(pModelData, useDiffFlag);
    _0 = new J3DMaterialAnm[anmNum];
    attachMaterialAnmBuffer(pModelData, useDiffFlag);
}

u32 MaterialAnmBuffer::getDiffFlag(s32 index) const {
    return _4[index];
}

u16 MaterialAnmBuffer::getAllocMaterialAnmNum(J3DModelData* pModelData, bool useDiffFlag) const {
    if (useDiffFlag) {
        return getDifferedMaterialNum(pModelData);
    }

    return pModelData->getMaterialNum();
}

void MaterialAnmBuffer::searchUpdateMaterialID(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) {
    const ResTable* pBpkTable = pResourceHolder->mBpkResTable;

    for (u32 i = 0; i < pBpkTable->mCount; i++) {
        static_cast< J3DAnmColorKey* >(pBpkTable->getRes(i))->searchUpdateMaterialID(pModelData);
    }

    const ResTable* pBtpTable = pResourceHolder->mBtpResTable;

    for (u32 i = 0; i < pBtpTable->mCount; i++) {
        static_cast< J3DAnmTexPattern* >(pBtpTable->getRes(i))->searchUpdateMaterialID(pModelData);
    }

    const ResTable* pBtkTable = pResourceHolder->mBtkResTable;

    for (u32 i = 0; i < pBtkTable->mCount; i++) {
        static_cast< J3DAnmTextureSRTKey* >(pBtkTable->getRes(i))->searchUpdateMaterialID(pModelData);
    }

    const ResTable* pBrkTable = pResourceHolder->mBrkResTable;

    for (u32 i = 0; i < pBrkTable->mCount; i++) {
        static_cast< J3DAnmTevRegKey* >(pBrkTable->getRes(i))->searchUpdateMaterialID(pModelData);
    }
}

void MaterialAnmBuffer::setDiffFlag(const ResourceHolder* pResourceHolder) {
    const char* pBpkName;
    const ResTable* pBpkTable = pResourceHolder->mBpkResTable;

    for (u32 iBpk = 0; iBpk < pBpkTable->mCount; iBpk++) {
        pBpkName = pBpkTable->getResName(iBpk);
        const J3DAnmColorKey* pAnm = static_cast< J3DAnmColorKey* >(pBpkTable->getRes(iBpk));
        MR::onDiffFlagBpk(_4, pAnm, pBpkName);
    }

    u32 iBtp;
    const ResTable* pBtpTable = pResourceHolder->mBtpResTable;

    for (iBtp = 0; iBtp < pBtpTable->mCount; iBtp++) {
        const char* pBtpName = pBtpTable->getResName(iBtp);
        const J3DAnmTexPattern* pAnm = static_cast< J3DAnmTexPattern* >(pBtpTable->getRes(iBtp));
        MR::onDiffFlagBtp(_4, pAnm, pBtpName);
    }

    u32 iBtk;
    const ResTable* pBtkTable = pResourceHolder->mBtkResTable;

    for (iBtk = 0; iBtk < pBtkTable->mCount; iBtk++) {
        const char* pBtkName = pBtkTable->getResName(iBtk);
        const J3DAnmTextureSRTKey* pAnm = static_cast< J3DAnmTextureSRTKey* >(pBtkTable->getRes(iBtk));
        MR::onDiffFlagBtk(_4, pAnm, pBtkName);
    }

    u32 iBrk;
    const ResTable* pBrkTable = pResourceHolder->mBrkResTable;

    for (iBrk = 0; iBrk < pBrkTable->mCount; iBrk++) {
        const char* pBrkName = pBrkTable->getResName(iBrk);
        const J3DAnmTevRegKey* pAnm = static_cast< J3DAnmTevRegKey* >(pBrkTable->getRes(iBrk));
        MR::onDiffFlagBrk(_4, pAnm, pBrkName);
    }
}

u16 MaterialAnmBuffer::getDifferedMaterialNum(const J3DModelData* pModelData) const {
    u16 materialNum = pModelData->getMaterialNum();
    u16 count = 0;

    for (u16 i = 0; i < materialNum; i++) {
        if (_4[i] != 0) {
            count++;
        }
    }

    return count;
}

void MaterialAnmBuffer::attachMaterialAnmBuffer(J3DModelData* pModelData, bool useDiffFlag) {
    u32 anmIndex = 0;

    for (u16 i = 0; i < pModelData->getMaterialNum(); i++) {
        if (!useDiffFlag || _4[i] != 0) {
            pModelData->getMaterialNodePointer(i)->mMaterialAnm = &_0[anmIndex];
            anmIndex++;
        }
    }
}

void modifyDiffFlagBrk(u32* pFlags, const J3DAnmTevRegKey* pAnm, bool enable, const char* pName) {
    for (u16 i = 0; i < pAnm->getCRegUpdateMaterialNum(); i++) {
        u16 material = pAnm->getCRegUpdateMaterialID(i);

        if (material != 0xFFFF) {
            if (enable) {
                pFlags[material] |= J3DDiffFlag_TevReg;
            } else {
                pFlags[material] &= ~J3DDiffFlag_TevReg;
            }
        }
    }

    for (u16 i = 0; i < pAnm->getKRegUpdateMaterialNum(); i++) {
        u16 material = pAnm->getKRegUpdateMaterialID(i);

        if (material != 0xFFFF) {
            if (enable) {
                pFlags[material] |= J3DDiffFlag_TevReg;
            } else {
                pFlags[material] &= ~J3DDiffFlag_TevReg;
            }
        }
    }
}

namespace MR {
    void onDiffFlagBpk(u32* pFlags, const J3DAnmColorKey* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, J3DDiffFlag_MatColor, true, pName, "bpk");
    }

    void offDiffFlagBpk(u32* pFlags, const J3DAnmColorKey* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, J3DDiffFlag_MatColor, false, pName, "bpk");
    }

    void onDiffFlagBtp(u32* pFlags, const J3DAnmTexPattern* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, static_cast< J3DDiffFlag >(0x20000), true, pName, "btp");
    }

    void offDiffFlagBtp(u32* pFlags, const J3DAnmTexPattern* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, static_cast< J3DDiffFlag >(0x20000), false, pName, "btp");
    }

    void onDiffFlagBtk(u32* pFlags, const J3DAnmTextureSRTKey* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, static_cast< J3DDiffFlag >(0x200), true, pName, "btk");
    }

    void offDiffFlagBtk(u32* pFlags, const J3DAnmTextureSRTKey* pAnm, const char* pName) {
        modifyDiffFlag(pFlags, pAnm, static_cast< J3DDiffFlag >(0x200), false, pName, "btk");
    }

    void onDiffFlagBrk(u32* pFlags, const J3DAnmTevRegKey* pAnm, const char* pName) {
        modifyDiffFlagBrk(pFlags, pAnm, true, pName);
    }

    void offDiffFlagBrk(u32* pFlags, const J3DAnmTevRegKey* pAnm, const char* pName) {
        modifyDiffFlagBrk(pFlags, pAnm, false, pName);
    }
}  // namespace MR
