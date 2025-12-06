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

    void initializeArc(JKRArchive&);
    static JKRFileFinder* getFindFinder(JKRArchive*, const char*);
    u32 initEachResTable(ResTable**, JKRArchive*, const ArchiveName*);

    void mount(JKRArchive*, char*);

    ResTable* mModelResTable;         // 0x0
    ResTable* mMotionResTable;        // 0x4
    ResTable* mBtkResTable;           // 0x8
    ResTable* mBpkResTable;           // 0xC
    ResTable* mBtpResTable;           // 0x10
    ResTable* mBlkResTable;           // 0x14
    ResTable* mBrkResTable;           // 0x18
    ResTable* mBasResTable;           // 0x1C
    ResTable* mBmtResTable;           // 0x20
    ResTable* mBvaResTable;           // 0x24
    ResTable* mBanmtResTable;         // 0x28
    ResTable* mFileInfoTable;         // 0x2C
    ResTable mDefaultTable;           // 0x30
    MaterialAnmBuffer* mMaterialBuf;  // 0x38
    BckCtrl* mBckCtrl;                // 0x3C
    void* mBackupMaterialData;        // 0x40
    JKRArchive* mArchive;             // 0x44
    JKRHeap* mHeap;                   // 0x48
    u32 _4C;
};
