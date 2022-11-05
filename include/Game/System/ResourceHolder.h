#pragma once

#include "Game/System/ResourceInfo.h"
#include "Game/Animation/BckCtrl.h"
#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRFileFinder.h>

class MaterialAnmBuffer;
class J3DModelData;

typedef const char* ArchiveName;

namespace {
    const char* sModelExt[2] = { ".bdl", ".bmd"};
    const char* sMotionExt[2] = { ".bck", ".bca" };
    const char* sBtkExt = ".btk";
    const char* sBpkExt = ".bpk";
    const char* sBtpExt = ".btp";
    const char* sBlkExt = ".blk";
    const char* sBrkExt = ".brk";
    const char* sBasExt = ".bas";
    const char* sBmtExt = ".bmt";
    const char* sBvaExt = ".bva";
    const char* sBanmtExt = ".banmt";
};

class ResourceHolder {
public:
    ResourceHolder(JKRArchive &);

    const char* getMotionName(u32) const;
    bool isExistMaterialAnm() const;
    void newMaterialAnmBuffer(J3DModelData *);
    void newBckCtrl();

    void initializeArc(JKRArchive &);
    static JKRFileFinder* getFindFinder(JKRArchive *, const char *);
    u32 initEachResTable(ResTable **, JKRArchive *, const ArchiveName *);

    void mount(JKRArchive *, char *);

    ResTable* mModelResTable;           // _0
    ResTable* mMotionResTable;          // _4
    ResTable* mBtkResTable;             // _8
    ResTable* mBpkResTable;             // _C
    ResTable* mBtpResTable;             // _10
    ResTable* mBlkResTable;             // _14
    ResTable* mBrkResTable;             // _18
    ResTable* mBasResTable;             // _1C
    ResTable* mBmtResTable;             // _20
    ResTable* mBvaResTable;             // _24
    ResTable* mBanmtResTable;           // _28
    ResTable* mFileInfoTable;           // _2C
    ResTable mDefaultTable;             // _30
    MaterialAnmBuffer* mMaterialBuf;    // _38
    BckCtrl* mBckCtrl;                  // _3C
    void* mBackupMaterialData;          // _40
    JKRArchive* mArchive;               // _44
    JKRHeap* mHeap;                     // _48
    u32 _4C;
};