#include "Game/System/ResourceHolder.hpp"
#include "Game/Animation/MaterialAnmBuffer.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MutexHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/J3DGraphLoader/J3DModelLoader.hpp>
#include <cstdio>
#include <cstring>
#include <revolution.h>

namespace {
    static const ArchiveName sModelExt[] = {".bdl", ".bmd", nullptr};
    static const ArchiveName sMotionExt[] = {".bck", ".bca", nullptr};
    static const ArchiveName sBtkExt[] = {".btk", nullptr};
    static const ArchiveName sBpkExt[] = {".bpk", nullptr};
    static const ArchiveName sBtpExt[] = {".btp", nullptr};
    static const ArchiveName sBlkExt[] = {".blk", nullptr};
    static const ArchiveName sBrkExt[] = {".brk", nullptr};
    static const ArchiveName sBasExt[] = {".bas", nullptr};
    static const ArchiveName sBmtExt[] = {".bmt", nullptr};
    static const ArchiveName sBvaExt[] = {".bva", nullptr};
    static const ArchiveName sBanmtExt[] = {".banmt", nullptr};

    // static const bool cIgnoreCheckPosFormatList = ;
    // static const bool cIgnoreCheckNrmCompressList = ;
    // static const bool cIgnoreCheckPlanetKLowSizeList = ;
    // static const bool cIgnoreCheckAll = ;

    // static const __ sPlanetLowSizeBorder = ;
};  // namespace

ResourceHolder::ResourceHolder(JKRArchive& rArchive)
    : mModelResTable(&mDefaultTable), mMotionResTable(&mDefaultTable), mBtkResTable(&mDefaultTable), mBpkResTable(&mDefaultTable),
      mBtpResTable(&mDefaultTable), mBlkResTable(&mDefaultTable), mBrkResTable(&mDefaultTable), mBasResTable(&mDefaultTable),
      mBmtResTable(&mDefaultTable), mBvaResTable(&mDefaultTable), mBanmtResTable(&mDefaultTable), mFileInfoTable(&mDefaultTable), mArchive(&rArchive),
      mMaterialBuf(nullptr), mBckCtrl(nullptr), mBackupMaterialData(nullptr), mTotalResourceSize(0) {
    mHeap = MR::getCurrentHeap();
    initializeArc(*mArchive);

    if (mModelResTable->mCount != 0 && isExistMaterialAnm()) {
        newMaterialAnmBuffer(reinterpret_cast< J3DModelData* >(mModelResTable->getRes(getModelName())));
    }

    if (mMotionResTable->mCount != 0) {
        newBckCtrl();
    }

    if (mModelResTable->mCount != 0) {
        backupInitMaterialData(reinterpret_cast< J3DModelData* >(mModelResTable->getRes(getModelName())));
    }
}

const char* ResourceHolder::getMotionName(u32 index) const {
    if (mMotionResTable->mCount > index) {
        return mMotionResTable->getResName(index);
    }
    return nullptr;
}

bool ResourceHolder::isExistMaterialAnm() const {
    return mBpkResTable->mCount != 0 || mBtpResTable->mCount != 0 || mBtkResTable->mCount != 0 || mBrkResTable->mCount != 0;
}

void ResourceHolder::newMaterialAnmBuffer(J3DModelData* pModelData) {
    MR::CurrentHeapRestorer heap(mHeap);
    mMaterialBuf = new MaterialAnmBuffer(this, pModelData, true);
}

void ResourceHolder::newBckCtrl() {
    MR::CurrentHeapRestorer heap(mHeap);
    mBckCtrl = new BckCtrl(this, mArchive->mLoaderName);
}

Mtx44& ResourceHolder::getInitEffectMtx(int materialIndex, int mtxIndex) const {
    return mBackupMaterialData[materialIndex * 8 + mtxIndex];
}

void ResourceHolder::initializeArc(JKRArchive& rArchive) {
    u32 fileInfoSize = rArchive.countResource();
    fileInfoSize -= initEachResTable(&mModelResTable, &rArchive, sModelExt);
    fileInfoSize -= initEachResTable(&mMotionResTable, &rArchive, sMotionExt);
    fileInfoSize -= initEachResTable(&mBlkResTable, &rArchive, sBlkExt);
    fileInfoSize -= initEachResTable(&mBtkResTable, &rArchive, sBtkExt);
    fileInfoSize -= initEachResTable(&mBpkResTable, &rArchive, sBpkExt);
    fileInfoSize -= initEachResTable(&mBtpResTable, &rArchive, sBtpExt);
    fileInfoSize -= initEachResTable(&mBrkResTable, &rArchive, sBrkExt);
    fileInfoSize -= initEachResTable(&mBasResTable, &rArchive, sBasExt);
    fileInfoSize -= initEachResTable(&mBmtResTable, &rArchive, sBmtExt);
    fileInfoSize -= initEachResTable(&mBvaResTable, &rArchive, sBvaExt);
    fileInfoSize -= initEachResTable(&mBanmtResTable, &rArchive, sBanmtExt);

    if (fileInfoSize > 0) {
        mFileInfoTable = new (mHeap, 0) ResTable();
        mFileInfoTable->newFileInfoTable(fileInfoSize);
    }

    mount(&rArchive, nullptr);
}

JKRFileFinder* ResourceHolder::getFileFinder(JKRArchive* pArchive, const char* pName) {
    if (pName == nullptr) {
        return pArchive->getFirstFile("/");
    }

    return pArchive->getFirstFile(pName);
}

u32 ResourceHolder::initEachResTable(ResTable** pResTable, JKRArchive* pArchive, const ArchiveName* pExtNames) {
    u32 numResources = 0;
    for (s32 idx = 0; pExtNames[idx] != nullptr; idx++) {
        numResources += count(pArchive, pExtNames[idx], nullptr);
    }

    if (numResources != 0) {
        *pResTable = new (mHeap, 0) ResTable();
        (*pResTable)->newFileInfoTable(numResources);
    }

    return numResources;
}

s32 ResourceHolder::count(JKRArchive* pArchive, const char* pExtName, const char* pPath) {
    s32 num = 0;
    JKRFileFinder* finder = getFileFinder(pArchive, pPath);

    while (finder->mHasMoreFiles) {
        if (finder->mFileIsFolder) {
            if (finder->mName[0] != '.') {
                char path[128];
                sprintf(path, "%s%s%s", pPath, "/", finder->mName);
                num += count(pArchive, pExtName, path);
            }
        } else {
            if (pExtName == nullptr || strstr(finder->mName, pExtName) != nullptr) {
                num++;
            }
        }
        finder->findNextFile();
    }

    delete finder;
    return num;
}

void ResourceHolder::mount(JKRArchive* pArchive, char* pPath) {
    JKRFileFinder* finder = getFileFinder(pArchive, pPath);

    while (finder->mHasMoreFiles) {
        if (finder->mFileIsFolder) {
            if (finder->mName[0] != '.') {
                char path[128];
                snprintf(path, 128, "%s/%s", pPath, finder->mName);
                mount(pArchive, path);
            }
        } else {
            u32 fileID = pArchive->getFileAttribute(finder->mFileID);
            ResFileInfo* info = createAndRegisterObject(finder->mName, pArchive->getResource(finder->mFileID));
            info->_8 = pArchive->getResource(finder->mFileID);
            info->_4 = pArchive->getResSize(info->_8);
            info->_C = finder->mFileID;
            mTotalResourceSize += info->_4;
        }
        finder->findNextFile();
    }

    delete finder;
}

ResFileInfo* ResourceHolder::createAndRegisterObject(const char* pName, void* pData) {
    if (strstr(pName, ".btp") != nullptr) {
        return mBtpResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".bpk") != nullptr) {
        return mBpkResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".btk") != nullptr) {
        return mBtkResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".brk") != nullptr) {
        return mBrkResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".blk") != nullptr) {
        return mBlkResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".bck") != nullptr) {
        return mMotionResTable->add(pName, pData == nullptr ? nullptr : J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".bca") != nullptr) {
        return mMotionResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".bas") != nullptr) {
        return mBasResTable->add(pName, pData, true);
    }

    if (strstr(pName, ".bmt") != nullptr) {
        OSLockMutex(&MR::MutexHolder< 0 >::sMutex);
        ResFileInfo* ret = mBmtResTable->add(pName, J3DModelLoaderDataBase::loadMaterialTable(pData), true);
        OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);
        return ret;
    }

    if (strstr(pName, ".bva") != nullptr) {
        return mBvaResTable->add(pName, J3DAnmLoaderDataBase::load(pData), true);
    }

    if (strstr(pName, ".banmt") != nullptr) {
        return mBanmtResTable->add(pName, pData, true);
    }

    if (strstr(pName, ".bdl") != nullptr) {
        OSLockMutex(&MR::MutexHolder< 0 >::sMutex);
        J3DModelData* modelData = J3DModelLoaderDataBase::loadBinaryDisplayList(pData, J3DMLF_Material_UseIndirect | J3DMLF_UseUniqueMaterials);
        OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);
        MR::initEnvelopeAndEnvMapOrProjMapModelData(modelData);
        if (MR::isUseFur(modelData) || MR::strcasecmp(pName, "MarioShadow.bdl") == 0) {
            MR::downFracVtx(modelData);
        }
        return mModelResTable->add(pName, modelData, true);
    }

    if (strstr(pName, ".bmd") != nullptr) {
        J3DModelData* modelData =
            J3DModelLoaderDataBase::load(pData, J3DMLF_Material_PE_Full | J3DMLF_Material_UseIndirect | J3DMLF_UseUniqueMaterials | J3DMLF_21);
        modelData->newSharedDisplayList(0);
        return mModelResTable->add(pName, modelData, true);
    }

    return mFileInfoTable->add(pName, pData, false);
}

void ResourceHolder::backupInitMaterialData(const J3DModelData* pModelData) {
    u16 numMaterials = pModelData->getMaterialNum();
    mBackupMaterialData = new Mtx44[numMaterials * 8];

    for (u16 matIdx = 0; matIdx < numMaterials; matIdx++) {
        J3DMaterial* material = pModelData->getMaterialNodePointer(matIdx);
        for (u32 mtxIdx = 0; mtxIdx < 8; mtxIdx++) {
            J3DTexMtx* texMtx = material->getTexGenBlock()->getTexMtx(mtxIdx);
            if (texMtx != nullptr) {
                PSMTX44Copy(texMtx->getTexMtxInfo().mEffectMtx, mBackupMaterialData[matIdx * 8 + mtxIdx]);
            } else {
                PSMTX44Identity(mBackupMaterialData[matIdx * 8 + mtxIdx]);
            }
        }
    }
}
