#pragma once

#include "Game/Animation/BckCtrl.hpp"
#include "Game/System/ResourceInfo.hpp"
#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

class MaterialAnmBuffer;
class J3DModelData;

typedef const char* ArchiveName;

class ResourceHolder {
public:
    ResourceHolder(JKRArchive&);

    const char* getMotionName(u32) const;
    bool isExistMaterialAnm() const;
    void newMaterialAnmBuffer(J3DModelData*);
    void newBckCtrl();

    Mtx44& getInitEffectMtx(int, int) const;
    void initializeArc(JKRArchive&);
    static JKRFileFinder* getFileFinder(JKRArchive*, const char*);
    u32 initEachResTable(ResTable**, JKRArchive*, const ArchiveName*);

    static s32 count(JKRArchive*, const char*, const char*);
    void mount(JKRArchive*, char*);

    ResFileInfo* createAndRegisterObject(const char*, void*);
    void backupInitMaterialData(const J3DModelData*);

    // bool checkCorrectMotionsAndModel(J3DModelData*, const char*) const;

    bool isCreatedAtSameHeap(ResourceHolder* pOther) const {
        return mHeap == pOther->mHeap;
    }

    // void makeStringAnmInfoAll(char*, u32) const;
    // bool verifyBckData(J3DModelData*, const char*) const;
    // bool verifyBvaData(J3DModelData*, const char*) const;
    // bool verifyBtpData(J3DModelData*, const char*) const;
    // bool verifyModelData(J3DModelData*, const char*) const;
    // bool verifyFileSize(u32, const char*) const;

    static int findBckIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mMotionResTable->getResIndex(pName);
    }
    static int findBpkIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mBpkResTable->getResIndex(pName);
    }
    static int findBtkIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mBtkResTable->getResIndex(pName);
    }
    static int findBtpIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mBtpResTable->getResIndex(pName);
    }
    static int findBrkIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mBrkResTable->getResIndex(pName);
    }
    static int findBvaIndex(const ResourceHolder* pResHolder, const char* pName) {
        return pResHolder->mBvaResTable->getResIndex(pName);
    }

    const ResTable* getBvaResTable() const {
        return mBvaResTable;
    }

    const char* getModelName() const {
        return mModelResTable->getResName(static_cast<u32>(0));
    }

    /* 0x00 */ ResTable* mModelResTable;
    /* 0x04 */ ResTable* mMotionResTable;
    /* 0x08 */ ResTable* mBtkResTable;
    /* 0x0C */ ResTable* mBpkResTable;
    /* 0x10 */ ResTable* mBtpResTable;
    /* 0x14 */ ResTable* mBlkResTable;
    /* 0x18 */ ResTable* mBrkResTable;
    /* 0x1C */ ResTable* mBasResTable;
    /* 0x20 */ ResTable* mBmtResTable;
    /* 0x24 */ ResTable* mBvaResTable;
    /* 0x28 */ ResTable* mBanmtResTable;
    /* 0x2C */ ResTable* mFileInfoTable;
    /* 0x30 */ ResTable mDefaultTable;
    /* 0x38 */ MaterialAnmBuffer* mMaterialBuf;
    /* 0x3C */ BckCtrl* mBckCtrl;
    /* 0x40 */ Mtx44* mBackupMaterialData;
    /* 0x44 */ JKRArchive* mArchive;
    /* 0x48 */ JKRHeap* mHeap;
    /* 0x4C */ u32 mTotalResourceSize;
};
