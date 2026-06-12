#pragma once

#include "JSystem/JAudio2/JAIStreamDataMgr.hpp"

struct BinaryStreamFileTable {
    /* 0x0 */ char mIdentifier[4];
    /* 0x4 */ u32 mNumFiles;
    /* 0x8 */ int mFilePathOffsets[];
};

struct JAUStreamFileTable {
    JAUStreamFileTable();
    void init(void const*);
    u32 getNumFiles() const;
    const char* getFilePath(int) const;

    bool isValid() {
        return mData != nullptr;
    }

    /* 0x0 */ const BinaryStreamFileTable* mData;
};

struct JAUStreamDataMgr_StreamFileTable : public JAIStreamDataMgr, JAUStreamFileTable {
    virtual s32 getStreamFileEntry(JAISoundID);
    virtual ~JAUStreamDataMgr_StreamFileTable() {
    }
};
